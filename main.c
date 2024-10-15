/**
 * @file      main.c
 * @author:   Shubhendu B B
 * @date:     04/02/2023
 * @brief
 * @details
 *
 * @copyright
 *
 **/

#include "ddl_evt.h"
#include "ddl_log.h"
#include "ddl_queue.h"
#include "ddl_serial.h"
#include "error.h"
#include "module_template_1.h"
#include "test_event.h"

//
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define TAG "MAIN"

uint8_t dataBuff[] = {"hello\r\n"};

static void app_serial_event_cb(ddl_SerialEvent_t event);

int main(void) {
    int exeStatus = NO_ERROR;

    ddl_serial_init();
    ddl_serial_register_cb(app_serial_event_cb);
    ddl_serial_send(dataBuff, sizeof(dataBuff));

    while (NO_ERROR == exeStatus) {
        ddl_serial_task(NULL);
    }

    ddl_serial_deinit();
    return 0;
}

static void app_serial_event_cb(ddl_SerialEvent_t event) {
    switch (event) {
        case DDL_SERIAL_EVENT_NONE:  // No event
            DDL_LOGI(TAG, "DDL_SERIAL_EVENT_NONE");
            break;

        case DDL_SERIAL_EVENT_RX:  // Data received
            DDL_LOGI(TAG, "DDL_SERIAL_EVENT_RX");
            ddl_serial_recv(dataBuff, 1);
            DDL_LOGI(TAG, "Received data: 0x%2.2X", dataBuff[0]);
            break;

        case DDL_SERIAL_EVENT_TX:  // Data transmitted
            DDL_LOGI(TAG, "DDL_SERIAL_EVENT_TX");
            break;

        case DDL_SERIAL_EVENT_QUEUE_FULL_RX:  // RX queue is full
            DDL_LOGI(TAG, "DDL_SERIAL_EVENT_QUEUE_FULL_RX");
            break;

        case DDL_SERIAL_EVENT_QUEUE_EMPTY_RX:  // RX queue is empty
            DDL_LOGI(TAG, "DDL_SERIAL_EVENT_QUEUE_EMPTY_RX");
            break;

        case DDL_SERIAL_EVENT_QUEUE_EMPTY_TX:  // TX queue is empty
            DDL_LOGI(TAG, "DDL_SERIAL_EVENT_QUEUE_EMPTY_TX");
            break;

        case DDL_SERIAL_EVENT_QUEUE_FULL_TX:  // TX queue is full
            DDL_LOGI(TAG, "DDL_SERIAL_EVENT_QUEUE_FULL_TX");
            break;

        default:
            DDL_LOGI(TAG, "Undefined event");
            break;
    }
}