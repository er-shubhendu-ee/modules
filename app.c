/**
 * @file      app.c
 * @author:   Shubhendu B B
 * @date:     16/10/2024
 * @brief
 * @details
 *
 * @copyright
 *
 **/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include application-specific headers
#include "app.h"
#include "config.h"
#include "ddl_log.h"
#include "ddl_serial.h"

#define TAG "APP"  // Define a tag for logging purposes

// Function prototype for the serial event callback
static void app_serial_event_cb(ddl_SerialEvent_t event);

// Global buffer for received data
uint8_t gDataBuff[CONFIG_SERIAL_DATA_BUFF_SIZE] = {0};

/**
 * @brief      Initializes the application.
 *
 * This function initializes the serial interface and registers the
 * callback for serial events. It also sends an initial message.
 */
void app_init(void) {
    ddl_serial_init();                            // Initialize the serial communication
    ddl_serial_register_cb(app_serial_event_cb);  // Register the event callback function
    ddl_serial_send("Init ok.\r\n", sizeof("Init ok.\r\n"));  // Send initialization message
}

/**
 * @brief      Main application task.
 *
 * This function handles the main task of the application, which is
 * to manage serial communication.
 *
 * @param      pvParameters   Parameters for the task (not used).
 * @return     int           Status code from the serial task function.
 */
int app_task(void *pvParameters) {
    ddl_serial_task(NULL);  // Execute the serial task
}

/**
 * @brief      Deinitializes the application.
 *
 * This function cleans up resources allocated during initialization.
 */
void app_deinit(void) {
    ddl_serial_deinit();  // Deinitialize the serial communication
}

/**
 * @brief      Callback function for serial events.
 *
 * This function handles different serial events such as data reception,
 * transmission, and queue status.
 *
 * @param      event   The event type that occurred.
 */
static void app_serial_event_cb(ddl_SerialEvent_t event) {
    switch (event) {
        case DDL_SERIAL_EVENT_NONE:  // No event occurred
            DDL_LOGI(TAG, "DDL_SERIAL_EVENT_NONE");
            break;

        case DDL_SERIAL_EVENT_RX:  // Data has been received
            DDL_LOGI(TAG, "DDL_SERIAL_EVENT_RX");
            ddl_serial_recv(gDataBuff, 1);                          // Receive one byte of data
            DDL_LOGI(TAG, "Received data: 0x%2.2X", gDataBuff[0]);  // Log the received data
            break;

        case DDL_SERIAL_EVENT_TX:  // Data has been transmitted
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

        default:  // Handle unexpected events
            DDL_LOGI(TAG, "Undefined event");
            break;
    }
}
