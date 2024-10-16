/**
 * @file      main.c
 * @author:   Shubhendu B B
 * @date:     13/10/2024
 * @brief
 * @details
 *
 * @copyright
 *
 **/

#include <Windows.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "app.h"
#include "ddl_evt.h"
#include "ddl_log.h"
#include "ddl_queue.h"
#include "ddl_serial.h"
#include "error.h"
#include "module_template_1.h"

#define TAG "MAIN"  // Tag for logging from this module

int main(void) {
    int exeStatus = NO_ERROR;  // Initialize execution status

    DDL_LOGI(TAG, "Initializing application...");  // Log initialization
    app_init();                                    // Initialize the application

    // Main loop: continuously process application tasks until an error occurs
    while (NO_ERROR == exeStatus) {
        exeStatus = app_task(NULL);  // Execute the application task
    }

    DDL_LOGI(TAG, "Exiting application with status: %d", exeStatus);  // Log exit status
    return exeStatus;  // Return status to the operating system
}
