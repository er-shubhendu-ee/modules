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

#include "ddl_serial.h"

int ddl_serial_port_rx_byte(uint8_t* pValue) {
    *pValue = 'A';
    return 0;  // Indicate success
}
