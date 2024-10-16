/**
 * @file      main.c
 * @author:   Shubhendu B B
 * @date:     04/02/2023
 * @brief     Main entry point for the application
 * @details   This file contains the main function, which initializes
 *            the application and enters the main task loop.
 *
 * @copyright (c) 2023 Shubhendu B B. All rights reserved.
 **/

// Include Windows-specific headers (if applicable)
#include <Windows.h>

// Standard library headers
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// Application-specific headers
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

    app_init();  // Initialize the application

    // Main loop: continuously process application tasks until an error occurs
    while (NO_ERROR == exeStatus) {
        app_task(NULL);  // Execute the application task
    }

    return 0;  // Return status to the operating system
}
