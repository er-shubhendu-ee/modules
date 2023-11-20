/**
 * @file      app.c
 * @author:   Shubhendu B B
 * @date:     18/05/2023
 * @brief
 * @details
 *
 * @copyright
 *
 **/

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>

#define PROJ_VER "0.0.2"

#define TAG "APP" 

#define APP_LOG_LEVEL_MAX (int)0xFF

#include "config.h"
#include "app.h"
#include "ddl_log.h"

#define APP_QUEUE_SIZE 10

static ddl_queue_handle_t gQueueHandle;
static app_logElementStruct_t queueBuff [ APP_QUEUE_SIZE ] = { 0 };


ddl_queue_handle_t app_queue_init(void) {

    gQueueHandle = ddl_queue_create_static(sizeof(app_logElementStruct_t), APP_QUEUE_SIZE, (uint8_t *) queueBuff);
    // gQueueHandle = ddl_queue_create(sizeof(app_logElementStruct_t), APP_QUEUE_SIZE);
    return gQueueHandle;
}

int app_queue_put(uint64_t logTimeStamp, int logLevel, int logId) {
    app_logElementStruct_t tempLogStorageStruct = { .timeStamp = logTimeStamp, .logLevel = logLevel, .idCode = logId };
    return ddl_queue_send(gQueueHandle, &tempLogStorageStruct);
}

int app_queue_get(app_logElementStruct_t *pLogStorageStructBuff, int logCount) {

    int elementCopiedCount = 0;
    int processStatus = 0;
    app_logElementStruct_t tempElement = { 0 };

    while ( logCount > elementCopiedCount ) {
        ddl_queue_recv(gQueueHandle, (void *) ((app_logElementStruct_t *) pLogStorageStructBuff + elementCopiedCount));
        elementCopiedCount++;
    }

#if defined(LOG_LEVEL_VERBOSE)
    DDL_LOGI(TAG, "%s : %d : exit");
#endif
    return processStatus;
}

int app_print_queue(void) {
    int i;
}