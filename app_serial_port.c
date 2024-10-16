/**
 * @file      app_serial_port.c
 * @author:   Shubhendu B B
 * @date:     16/10/2024
 * @brief
 * @details
 *
 * @copyright
 *
 **/
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

//
#include <Windows.h>

//
#include "ddl_log.h"
#include "ddl_queue.h"
#include "ddl_serial.h"
#include "error.h"

#define TAG "APP_SERIAL_PORT"

uint8_t gBuff_rxStreamQueue[128];
ddl_queue_handle_t ghQueue_rxStream;

#if defined _WINDOWS_
HANDLE ghComPort;  // Handle for the COM port
// Thread function prototypes
DWORD WINAPI ThreadFunction1(LPVOID lpParam);
DWORD WINAPI app_thread_rx_byte(LPVOID lpParam);
#endif

int ddl_serial_port_init(void) {
    int exeStatus = NO_ERROR;

    ghQueue_rxStream =
        ddl_queue_create_static(sizeof(uint8_t), sizeof(gBuff_rxStreamQueue), gBuff_rxStreamQueue);
    if (NULL == ghQueue_rxStream) {
        DDL_LOGE(TAG, "Error in queue creation.");
        exeStatus = ERROR_NOT_ENOUGH_MEMORY;
        goto label_exitPoint;
    }

#if defined _WINDOWS_
    // Create two threads for continuous functions
    HANDLE thread1 = CreateThread(NULL, 0, ThreadFunction1, NULL, 0, NULL);
    HANDLE thread2 = CreateThread(NULL, 0, app_thread_rx_byte, NULL, 0, NULL);

    // Optionally check if threads were created successfully
    if (thread1 == NULL || thread2 == NULL) {
        DDL_LOGI(TAG, "Failed to create threads");
        exeStatus = ERROR_NOT_ENOUGH_MEMORY;
        goto label_exitPoint;
    }

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
        goto label_exitPoint;
    } else {
        DDL_LOGI(TAG, "Opening serial port successful");
    }
#endif
label_exitPoint:
    return exeStatus;
}

int ddl_serial_port_deinit(void) {
#if defined _WINDOWS_
    if (ghComPort != INVALID_HANDLE_VALUE) {
        CloseHandle(ghComPort);  // Close the COM port handle
    }
#endif
    return 0;
}

int ddl_serial_port_tx_byte(uint8_t value) {
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

int ddl_serial_port_rx_byte(uint8_t* pValue) {
    uint8_t rxByte = 0;
    if (DDL_BASE_STATUS_OK == ddl_queue_recv(ghQueue_rxStream, &rxByte)) {
        *pValue = rxByte;
        return 0;  // Indicate success
    }
    return -1;  // Indicate failure
}

// Continuous task for the first thread
DWORD WINAPI ThreadFunction1(LPVOID lpParam) {
    while (1) {
        // Your continuous task for thread 1
        DDL_LOGI(TAG, "Thread 1 is running.");
        ddl_serial_task(NULL);  // Execute the serial task and return its status
        Sleep(10);              // Sleep for 1 second
    }
    return 0;
}

// Continuous task for the second thread
DWORD WINAPI app_thread_rx_byte(LPVOID lpParam) {
    uint8_t byteReceived;
    DWORD bytesRead;
    while (1) {
        // Your continuous task for thread 2
        DDL_LOGI(TAG, "Thread 2 is running.");
        // Attempt to read data from the COM port
        if (ReadFile(ghComPort, &byteReceived, sizeof(byteReceived), &bytesRead, NULL)) {
            if (bytesRead > 0) {
                // Send received bytes to the queue
                for (size_t i = 0; i < bytesRead; i++) {
                    ddl_queue_send(ghQueue_rxStream, &byteReceived);
                }
            }
        } else {
            DDL_LOGI(TAG, "Error reading from serial port: %d", GetLastError());
        }
        Sleep(10);  // Sleep for 1 second
    }
    return 0;
}