/**
 * @file      app_serial_port.c
 * @author:   Shubhendu B B
 * @date:     16/10/2024
 * @brief     Serial port handling application
 * @details   Provides functions to initialize, transmit, and receive data via a serial port.
 * @copyright
 **/

#include <Windows.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

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
HANDLE thread1, thread2;         // Store thread handles
bool shouldStopThreads = false;  // Flag to signal threads to stop
#endif

int ddl_serial_port_init(void) {
    int exeStatus = NO_ERROR;

    ghQueue_rxStream =
        ddl_queue_create_static(sizeof(uint8_t), sizeof(gBuff_rxStreamQueue), gBuff_rxStreamQueue);
    if (ghQueue_rxStream == NULL) {
        DDL_LOGE(TAG, "Error in queue creation.");
        return ERROR_NOT_ENOUGH_MEMORY;
    }

#if defined _WINDOWS_
    // Create threads and check for success
    thread1 = CreateThread(NULL, 0, ThreadFunction1, NULL, 0, NULL);
    thread2 = CreateThread(NULL, 0, app_thread_rx_byte, NULL, 0, NULL);
    if (thread1 == NULL || thread2 == NULL) {
        DDL_LOGI(TAG, "Failed to create threads");
        exeStatus = ERROR_NOT_ENOUGH_MEMORY;
        goto cleanup;
    }

    // Open the serial port
    ghComPort =
        CreateFile("\\\\.\\COM1", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (ghComPort == INVALID_HANDLE_VALUE) {
        DDL_LOGI(TAG, "Error in opening serial port: %d", GetLastError());
        exeStatus = ERROR_OPEN_FAILED;
        goto cleanup;
    } else {
        DDL_LOGI(TAG, "Opening serial port successful");
    }

    // Configure the COM port
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(ghComPort, &dcbSerialParams)) {
        DDL_LOGI(TAG, "Error getting state: %d", GetLastError());
        exeStatus = ERROR_OPEN_FAILED;
        goto cleanup;
    }

    // Set parameters (example values)
    dcbSerialParams.BaudRate = CBR_9600;    // Baud rate
    dcbSerialParams.ByteSize = 8;           // Data bits
    dcbSerialParams.StopBits = ONESTOPBIT;  // Stop bits
    dcbSerialParams.Parity = NOPARITY;      // Parity

    // Set the new state
    if (!SetCommState(ghComPort, &dcbSerialParams)) {
        DDL_LOGI(TAG, "Error setting state: %d", GetLastError());
        exeStatus = ERROR_OPEN_FAILED;
        goto cleanup;
    }

    // Set timeouts
    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout = 50;          // Maximum time between characters
    timeouts.ReadTotalTimeoutConstant = 50;     // Total timeout for read
    timeouts.ReadTotalTimeoutMultiplier = 10;   // Multiplier for total timeout
    timeouts.WriteTotalTimeoutConstant = 50;    // Total timeout for write
    timeouts.WriteTotalTimeoutMultiplier = 10;  // Multiplier for write
    if (!SetCommTimeouts(ghComPort, &timeouts)) {
        DDL_LOGI(TAG, "Error setting timeouts: %d", GetLastError());
        exeStatus = ERROR_OPEN_FAILED;
        goto cleanup;
    }
#endif

cleanup:
    // If there was an error, ensure to clean up resources
    if (exeStatus != NO_ERROR) {
        if (ghComPort != INVALID_HANDLE_VALUE) {
            CloseHandle(ghComPort);
        }
    }

    return exeStatus;
}

int ddl_serial_port_deinit(void) {
#if defined _WINDOWS_
    shouldStopThreads = true;                // Signal threads to stop
    WaitForSingleObject(thread1, INFINITE);  // Wait for thread 1 to finish
    WaitForSingleObject(thread2, INFINITE);  // Wait for thread 2 to finish

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
    DWORD bytesWritten;
    bool status = WriteFile(ghComPort, &byteTx, sizeof(byteTx), &bytesWritten, NULL);
    if (!status) {
        DDL_LOGI(TAG, "Error: 0x%8.8X", GetLastError());
        return -1;  // Indicate error
    }
    if (bytesWritten != sizeof(byteTx)) {
        DDL_LOGI(TAG, "Warning: Not all bytes written.");
        return -1;  // Indicate partial write
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
    while (!shouldStopThreads) {
        DDL_LOGI(TAG, "Thread 1 is running.");
        ddl_serial_task(NULL);  // Execute the serial task and return its status
        Sleep(10);              // Sleep for 1 second
    }
    return 0;
}

// Continuous task for the second thread
DWORD WINAPI app_thread_rx_byte(LPVOID lpParam) {
    uint8_t byteReceived[256];  // Temporary buffer for received bytes
    DWORD bytesRead;
    while (!shouldStopThreads) {
        DDL_LOGI(TAG, "Thread 2 is running.");
        // Attempt to read data from the COM port
        if (ReadFile(ghComPort, byteReceived, sizeof(byteReceived), &bytesRead, NULL)) {
            if (bytesRead > 0) {
                for (size_t i = 0; i < bytesRead; i++) {
                    ddl_queue_send(ghQueue_rxStream, &byteReceived[i]);
                }
            }
        } else {
            DDL_LOGI(TAG, "Error reading from serial port: %d", GetLastError());
        }
        Sleep(10);  // Sleep for 100 milliseconds
    }
    return 0;
}
