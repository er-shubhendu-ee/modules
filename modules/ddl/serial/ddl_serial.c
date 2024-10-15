#include "ddl_serial.h"

// Local headers
#include "ddl_log.h"
#include "ddl_queue.h"
#include "error.h"

// Global headers
#include <stddef.h>  // For size_t
#include <stdint.h>  // For uint8_t
#include <stdio.h>   // For printf

#define TAG "DDL_SERIAL"
#define LOG_LEVEL LOG_LEVEL_DEBUG
#define MAX_QUEUE_SIZE 128  // Define this based on your application needs

// Callback function pointer
static ddl_serial_event_cb_t gSerialEvent_cb = NULL;

// Static buffers for transmission and reception queues
static uint8_t txQueueBuffer[MAX_QUEUE_SIZE * sizeof(uint8_t)];
static uint8_t rxQueueBuffer[MAX_QUEUE_SIZE * sizeof(uint8_t)];

// Queue handles for transmission and reception
static ddl_queue_handle_t hSerialQueueTx = NULL;
static ddl_queue_handle_t hSerialQueueRx = NULL;

int ddl_serial_init(void) {
    int exeStatus = NO_ERROR;

    // Create the transmission queue using the static buffer
    hSerialQueueTx = ddl_queue_create_static(sizeof(uint8_t), MAX_QUEUE_SIZE, txQueueBuffer);
    if (hSerialQueueTx == NULL) {
#if LOG_LEVEL > LOG_LEVEL_NONE
        DDL_LOGE(TAG, "Error creating TX queue.");
#endif
        exeStatus = ERROR_NOT_ENOUGH_MEMORY;  // Memory allocation failure
        goto label_exitPoint;
    }

    // Create the reception queue using the static buffer
    hSerialQueueRx = ddl_queue_create_static(sizeof(uint8_t), MAX_QUEUE_SIZE, rxQueueBuffer);
    if (hSerialQueueRx == NULL) {
#if LOG_LEVEL > LOG_LEVEL_NONE
        DDL_LOGE(TAG, "Error creating RX queue.");
#endif
        exeStatus = ERROR_NOT_ENOUGH_MEMORY;  // Memory allocation failure
        goto label_exitPoint;
    }

label_exitPoint:
    if (exeStatus != NO_ERROR) {
        // Clean up resources if initialization fails
        if (hSerialQueueTx) {
            ddl_queue_delete(hSerialQueueTx);
            hSerialQueueTx = NULL;
        }
        if (hSerialQueueRx) {
            ddl_queue_delete(hSerialQueueRx);
            hSerialQueueRx = NULL;
        }
    }
    return exeStatus;  // Single exit point
}

void ddl_serial_register_cb(ddl_serial_event_cb_t callback) {
    gSerialEvent_cb = callback;  // Register the callback
}

int ddl_serial_send(uint8_t* pDataBuff, size_t dataBuffLen) {
    int exeStatus = NO_ERROR;

    if (pDataBuff == NULL || dataBuffLen == 0) {
        exeStatus = ERROR_INVALID_PARAMETER;  // Invalid parameters
#if LOG_LEVEL > LOG_LEVEL_NONE
        DDL_LOGE(TAG, "Invalid parameters for sending data.");
#endif
        return exeStatus;  // Early return on error
    }

    // Enqueue the data to the transmission queue
    for (size_t i = 0; i < dataBuffLen; i++) {
        exeStatus = ddl_queue_send(hSerialQueueTx, &pDataBuff[i]);
        if (exeStatus != NO_ERROR) {
#if LOG_LEVEL > LOG_LEVEL_VERBOSE
            DDL_LOGI(TAG, "TX queue full.");
#endif
            break;  // Exit on error
        }
    }

    // Call the registered callback if available
    if (exeStatus == NO_ERROR && gSerialEvent_cb != NULL) {
        gSerialEvent_cb(DDL_SERIAL_EVENT_TX);
    }

    return exeStatus;  // Single exit point
}

int ddl_serial_recv(uint8_t* pDataBuff, size_t dataBuffLen) {
    int exeStatus = NO_ERROR;

    if (pDataBuff == NULL || dataBuffLen == 0) {
        exeStatus = ERROR_INVALID_PARAMETER;  // Invalid parameters
#if LOG_LEVEL > LOG_LEVEL_NONE
        DDL_LOGE(TAG, "Invalid parameters for receiving data.");
#endif
        return exeStatus;  // Early return on error
    }

    // Dequeue data from the reception queue into the provided buffer
    for (size_t i = 0; i < dataBuffLen; i++) {
        exeStatus = ddl_queue_recv(hSerialQueueRx, &pDataBuff[i]);
        if (exeStatus != NO_ERROR) {
#if LOG_LEVEL > LOG_LEVEL_VERBOSE
            DDL_LOGI(TAG, "RX queue empty.");
#endif
            break;  // Exit the loop on error
        }
    }

    return exeStatus;  // Single exit point
}

int ddl_serial_deinit(void) {
    int exeStatus = NO_ERROR;

    // Check if the transmission queue exists and delete it
    if (hSerialQueueTx) {
        ddl_queue_delete(hSerialQueueTx);
        hSerialQueueTx = NULL;
#if LOG_LEVEL > LOG_LEVEL_NONE
        DDL_LOGI(TAG, "TX queue successfully deleted.");
#endif
    } else {
#if LOG_LEVEL > LOG_LEVEL_NONE
        DDL_LOGW(TAG, "TX queue was already NULL.");
#endif
    }

    // Check if the reception queue exists and delete it
    if (hSerialQueueRx) {
        ddl_queue_delete(hSerialQueueRx);
        hSerialQueueRx = NULL;
#if LOG_LEVEL >= LOG_LEVEL_INFO
        DDL_LOGI(TAG, "RX queue successfully deleted.");
#endif
    } else {
#if LOG_LEVEL >= LOG_LEVEL_DEBUG
        DDL_LOGW(TAG, "RX queue was already NULL.");
#endif
    }

    return exeStatus;  // Return the status of the deinitialization
}

void ddl_serial_task(void* pvParameters) {
    (void)pvParameters;  // Ignore unused parameter

    uint8_t txValue;  // Variable to hold the value to send
    uint8_t rxValue;  // Variable to hold the received value

    // Check if there is data in the TX queue and send it
    if (ddl_queue_recv(hSerialQueueTx, &txValue) == NO_ERROR) {
        // Send the value using the serial port transmission function
        ddl_serial_port_tx(txValue);
#if LOG_LEVEL >= LOG_LEVEL_DEBUG
        DDL_LOGD(TAG, "Sent byte: %02X", txValue);
#endif

        // Notify the event callback for TX event
        if (gSerialEvent_cb) {
            gSerialEvent_cb(DDL_SERIAL_EVENT_TX);
        }
    }

    // Receive a byte from the serial port
    if (ddl_serial_port_rx(&rxValue) == 0) {  // Assuming 0 indicates success
        // Enqueue the received byte to the RX queue
        if (ddl_queue_send(hSerialQueueRx, &rxValue) == NO_ERROR) {
#if LOG_LEVEL >= LOG_LEVEL_DEBUG
            DDL_LOGD(TAG, "Received byte: %02X", rxValue);
#endif

            // Notify the event callback for RX event
            if (gSerialEvent_cb) {
                gSerialEvent_cb(DDL_SERIAL_EVENT_RX);
            }
        } else {
#if LOG_LEVEL > LOG_LEVEL_NONE
            DDL_LOGE(TAG, "Error enqueuing received data to RX queue.");
#endif
            // Notify the event callback for RX queue full event
            if (gSerialEvent_cb) {
                gSerialEvent_cb(DDL_SERIAL_EVENT_QUEUE_FULL_RX);
            }
        }
    } else {
#if LOG_LEVEL >= LOG_LEVEL_DEBUG
        DDL_LOGD(TAG, "No byte received from serial port.");
#endif
    }
}

// Weak attribute definition for transmission
__attribute__((weak)) int ddl_serial_port_tx(uint8_t value) {
    // Default implementation: just print the value
    printf("Transmitting value: %02X\n", value);
    return 0;  // Indicate success
}

// Weak attribute definition for reception
uint8_t testValue = 0;  // Simulated received value
__attribute__((weak)) int ddl_serial_port_rx(uint8_t* pValue) {
    // Default implementation: just print a message
    // Here, we'll simulate receiving a value (for demonstration purposes)
    printf("Receiving value: %02X\n", testValue);

    // Assign the simulated value to the pointer
    *pValue = testValue;
    testValue++;
    return 0;  // Indicate success
}
