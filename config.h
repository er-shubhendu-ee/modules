/**
 * @file      config.h
 * @author:   Shubhendu B B
 * @date:     16/10/2023
 * @brief     Configuration header file
 * @details   This file contains configuration settings for the application,
 *            including logging levels, queue sizes, and buffer settings.
 *
 * @copyright (c) 2023 Shubhendu B B. All rights reserved.
 **/

#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "ddl_log.h"  // Include logging functionality

// Set the logging level for the application
#define CONFIG_LOG_LEVEL LOG_LEVEL_VERBOSE  // Verbose logging for detailed output

// Event configuration
#define CONFIG_DDL_EVT_EVENT_QUEUE_SIZE 20  // Size of the event queue
#define CONFIG_DDL_EVT_MODULE_COUNT 10      // Number of event modules
#define CONFIG_QUEUE_POOL_SIZE 10           // Size of the queue pool
#define CONFIG_QUEUE_USE_STATIC 1           // Use static allocation for queues

// Serial configuration
#define CONFIG_SERIAL_DATA_BUFF_SIZE 16  // Size of the serial data buffer

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif /* __CONFIG_H__ */
