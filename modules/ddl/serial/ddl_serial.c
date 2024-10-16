/**
 * @file      ddl_serial.c
 * @author:   Shubhendu B B
 * @date:     13/10/2024
 * @brief
 * @details
 *
 * @copyright
 *
 **/

#include "ddl_serial.h"

#include <Windows.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "ddl_log.h"
#include "ddl_queue.h"
#include "error.h"

#define TAG "DDL_SERIAL"           // Tag for logging from this module
#define LOG_LEVEL LOG_LEVEL_DEBUG  // Define the logging level
#define MAX_QUEUE_SIZE 128         // Define maximum size for queues

static ddl_serial_event_cb_t gSerialEvent_cb = NULL;  // Callback function pointer

// Buffers for transmission and reception queues
static uint8_t txQueueBuffer[MAX_QUEUE_SIZE * sizeof(uint8_t)];
static uint8_t rxQueueBuffer[MAX_QUEUE_SIZE * sizeof(uint8_t)];

// Queue handles for transmission and reception
static ddl_queue_handle_t hSerialQueueTx = NULL;
static ddl_queue_handle_t hSerialQueueRx = NULL;

#if defined _WINDOWS_
HANDLE ghComPort;  // Handle for the COM port
#endif

int ddl_serial_init(void) {
    int exeStatus = NO_ERROR;

#if defined _WINDOWS_
    ghComPort = CreateFile("\\\\.\\COM1",                 // Open the specified COM port
                           GENERIC_READ | GENERIC_WRITE,  // Read/Write
                           0,                             // No Sharing
                           NULL,                          // No Security
                           OPEN_EXISTING,                 // Open existing port
                           0,                             // Non Overlapped I/O
                           NULL);                         // Null for Comm Devices

    if (ghComPort == INVALID_HANDLE_VALUE) {
        DDL_LOGI(TAG, "Error in opening serial port: %d", GetLastError());
        exeStatus = ERROR_OPEN_FAILED;  // Set appropriate error code
        return exeStatus;
    } else {
        DDL_LOGI(TAG, "Opening serial port successful");
    }
#endif

    ddl_serial_port_init();

    // Create the transmission queue
    hSerialQueueTx = ddl_queue_create_static(sizeof(uint8_t), MAX_QUEUE_SIZE, txQueueBuffer);
    if (hSerialQueueTx == NULL) {
        exeStatus = ERROR_NOT_ENOUGH_MEMORY;  // Handle queue creation failure
        goto label_exitPoint;
    }

    // Create the reception queue
    hSerialQueueRx = ddl_queue_create_static(sizeof(uint8_t), MAX_QUEUE_SIZE, rxQueueBuffer);
    if (hSerialQueueRx == NULL) {
        exeStatus = ERROR_NOT_ENOUGH_MEMORY;  // Handle queue creation failure
        goto label_exitPoint;
    }

label_exitPoint:
    // Clean up resources if initialization fails
    if (exeStatus != NO_ERROR) {
        if (hSerialQueueTx) {
            ddl_queue_delete(hSerialQueueTx);
            hSerialQueueTx = NULL;
        }
        if (hSerialQueueRx) {
            ddl_queue_delete(hSerialQueueRx);
            hSerialQueueRx = NULL;
        }
    }
    return exeStatus;  // Return status code
}

void ddl_serial_register_cb(ddl_serial_event_cb_t callback) {
    gSerialEvent_cb = callback;  // Registers a callback function for serial events
}

int ddl_serial_send(uint8_t* pDataBuff, size_t dataBuffLen, size_t* pBytesSentCount) {
    int exeStatus = NO_ERROR;

    // Validate input parameters
    if (pDataBuff == NULL || dataBuffLen == 0) {
        exeStatus = ERROR_INVALID_PARAMETER;  // Invalid parameters
        return exeStatus;                     // Early return on error
    }

    // Enqueue the data to the transmission queue
    for (size_t i = 0; i < dataBuffLen; i++) {
        exeStatus = ddl_queue_send(hSerialQueueTx, &pDataBuff[i]);
        if (exeStatus != NO_ERROR) {
            break;  // Exit on error
        } else if (pBytesSentCount) {
            *((size_t*)pBytesSentCount) += 1;
        }
    }

    // Notify the registered callback if data was sent successfully
    if (exeStatus == NO_ERROR && gSerialEvent_cb != NULL) {
        gSerialEvent_cb(DDL_SERIAL_EVENT_TX);
    }

    return exeStatus;  // Return status code
}

int ddl_serial_recv(uint8_t* pDataBuff, size_t dataBuffLen, size_t* pBytesReceivedCount) {
    int exeStatus = NO_ERROR;

    // Validate input parameters
    if (pDataBuff == NULL || dataBuffLen == 0) {
        exeStatus = ERROR_INVALID_PARAMETER;  // Invalid parameters
        return exeStatus;                     // Early return on error
    }

    // Dequeue data into the provided buffer
    for (size_t i = 0; i < dataBuffLen; i++) {
        exeStatus = ddl_queue_recv(hSerialQueueRx, &pDataBuff[i]);
        if (exeStatus != NO_ERROR) {
            break;  // Exit on error
        } else if (pBytesReceivedCount) {
            *((size_t*)pBytesReceivedCount) += 1;
        }
    }

    return exeStatus;  // Return status code
}

int ddl_serial_deinit(void) {
    int exeStatus = NO_ERROR;

#if defined _WINDOWS_
    if (ghComPort != INVALID_HANDLE_VALUE) {
        CloseHandle(ghComPort);  // Close the COM port handle
    }
#endif

    ddl_serial_port_deinit();

    // Delete the transmission queue if it exists
    if (hSerialQueueTx) {
        ddl_queue_delete(hSerialQueueTx);
        hSerialQueueTx = NULL;
    }

    // Delete the reception queue if it exists
    if (hSerialQueueRx) {
        ddl_queue_delete(hSerialQueueRx);
        hSerialQueueRx = NULL;
    }

    return exeStatus;  // Return the status of the deinitialization
}

void ddl_serial_task(void* pvParameters) {
    (void)pvParameters;  // Ignore unused parameter

    uint8_t txValue;  // Variable to hold the value to send
    uint8_t rxValue;  // Variable to hold the received value

    // Check if there is data in the TX queue and send it
    if (ddl_queue_recv(hSerialQueueTx, &txValue) == NO_ERROR) {
        ddl_serial_port_tx_byte(txValue);  // Transmit the byte
        // Notify the event callback for TX event
        if (gSerialEvent_cb) {
            gSerialEvent_cb(DDL_SERIAL_EVENT_TX);
        }
    }

    // Receive a byte from the serial port
    if (ddl_serial_port_rx_byte(&rxValue) == 0) {  // Assuming 0 indicates success
        // Enqueue the received byte to the RX queue
        if (ddl_queue_send(hSerialQueueRx, &rxValue) == NO_ERROR) {
            // Notify the event callback for RX event
            if (gSerialEvent_cb) {
                gSerialEvent_cb(DDL_SERIAL_EVENT_RX);
            }
        } else {
            // Notify the event callback for RX queue full event
            if (gSerialEvent_cb) {
                gSerialEvent_cb(DDL_SERIAL_EVENT_QUEUE_FULL_RX);
            }
        }
    }
}

// Weak attribute definition for transmission
__attribute__((weak)) int ddl_serial_port_init(void) { return 0; }

// Weak attribute definition for transmission
__attribute__((weak)) int ddl_serial_port_deinit(void) { return 0; }

// Weak attribute definition for transmission
__attribute__((weak)) int ddl_serial_port_tx_byte(uint8_t value) {
    DDL_LOGI(TAG, "Transmitting value: %02X", value);  // Log transmission
#if defined _WINDOWS_
    uint8_t byteTx = value;
    bool status = WriteFile(ghComPort, &byteTx, sizeof(byteTx), NULL, NULL);
    if (!status) {
        DDL_LOGI(TAG, "Error: 0x%8.8X", GetLastError());
    }
#endif
    return 0;  // Indicate success
}

// Weak attribute definition for reception
uint8_t testValue = 0;  // Simulated received value
__attribute__((weak)) int ddl_serial_port_rx_byte(uint8_t* pValue) {
    *pValue = testValue;  // Simulate receiving a value
    testValue++;
    return 0;  // Indicate success
}
