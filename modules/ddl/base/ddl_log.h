#include <stdio.h>

#include "ddl_base.h"

// Define log levels
#define LOG_LEVEL_NONE 0
#define LOG_LEVEL_ERROR 1
#define LOG_LEVEL_INFO 2
#define LOG_LEVEL_DEBUG 3
#define LOG_LEVEL_VERBOSE 4

// Logging macros with color formatting for terminal output

/**
 * @brief Logs a debug message.
 *
 * @param TAG Identifier for the log message.
 * @param fmt Format string for the log message.
 * @param ... Additional arguments for the format string.
 */
#define DDL_LOGD(TAG, fmt, ...) \
    printf("\033[37m" TAG " : %s: %d: " fmt "\033[0m\n", __func__, __LINE__, ##__VA_ARGS__);

/**
 * @brief Logs an info message.
 *
 * @param TAG Identifier for the log message.
 * @param fmt Format string for the log message.
 * @param ... Additional arguments for the format string.
 */
#define DDL_LOGI(TAG, fmt, ...) \
    printf("\033[32m" TAG " : %s: %d: " fmt "\033[0m\n", __func__, __LINE__, ##__VA_ARGS__);

/**
 * @brief Logs a warning message.
 *
 * @param TAG Identifier for the log message.
 * @param fmt Format string for the log message.
 * @param ... Additional arguments for the format string.
 */
#define DDL_LOGW(TAG, fmt, ...) \
    printf("\033[33m" TAG " : %s: %d: " fmt "\033[0m\n", __func__, __LINE__, ##__VA_ARGS__);

/**
 * @brief Logs an error message.
 *
 * @param TAG Identifier for the log message.
 * @param fmt Format string for the log message.
 * @param ... Additional arguments for the format string.
 */
#define DDL_LOGE(TAG, fmt, ...) \
    printf("\033[31m" TAG " : %s: %d: " fmt "\033[0m\n", __func__, __LINE__, ##__VA_ARGS__);
