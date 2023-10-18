/**
 * @file      module_1.c
 * @author:   Shubhendu B B
 * @date:     16/10/2023
 * @brief
 * @details
 *
 * @copyright
 *
**/
#include <error.h>

#include "config.h"
#include "module_1.h"
#include "ddl_log.h"

#define MODULE_LOG_LEVEL CONFIG_LOG_LEVEL

#define TAG "MODULE_1"

int module_1_task(void) {
    int processStatus = NO_ERROR;

#if MODULE_LOG_LEVEL>=LOG_LEVEL_VERBOSE
    DDL_LOGI(TAG, "Module 1 task.");
#endif

    return processStatus;
}