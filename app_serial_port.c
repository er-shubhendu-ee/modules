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
#include "ddl_serial.h"

#define TAG "APP_SERIAL_PORT"

#if defined _WINDOWS_
HANDLE ghComPort;  // Handle for the COM port
#endif

int ddl_serial_port_init(void) {
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
    return 0;
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
    *pValue = 'A';
    return 0;  // Indicate success
}
