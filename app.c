/**
 * @file      app.c
 * @author:   Shubhendu B B
 * @date:     13/10/2023
 * @brief
 * @details
 *
 * @copyright
 *
**/
#include <error.h>
#include <string.h>

#include "config.h"
#include "app.h"
#include "ddl_log.h"
#include "ddl_queue.h"

#include "module_1.h"

#define TAG "APP"
#define APP_LOG_LEVEL CONFIG_LOG_LEVEL
#define APP_HEAP_ALLOCATION_SIZE_BYTES 1024
#define APP_TASK_LIST_SIZE CONFIG_APP_TASK_LIST_SIZE


typedef struct taskEntryStruct {
    struct taskEntryStruct *pPreviousEntry;
    struct taskEntryStruct *pNextEntry;
    struct {
        app_module_task_t task;
    }thisEntry;
}task_entry_t;

static ddl_queue_handle_t appEventQueueHandle = NULL;
static app_evt_t appEventQueue [ CONFIG_APP_EVENT_QUEUE_SIZE ] = { 0 };
static task_entry_t taskList [ APP_TASK_LIST_SIZE ] = { 0 };
static int taskEntryLast = 0;

static int process_evt(app_evt_t *const pEvent);

int app_init(void) {
    int processStatus = NO_ERROR;
    /* Init code goes here */
    memset((void *) appEventQueue, 0, sizeof(appEventQueue));
    memset((void *) taskList, 0, sizeof(taskList));

    appEventQueueHandle = ddl_queue_create_static(sizeof(app_evt_t), CONFIG_APP_EVENT_QUEUE_SIZE, (uint8_t *) appEventQueue);

    if ( !appEventQueueHandle ) {
#if APP_LOG_LEVEL>=LOG_LEVEL_ERROR
        DDL_LOGE(TAG, "Failed to get appEventQueueHandle");
#endif
        processStatus = ERROR_INVALID_HANDLE;
        goto label_exitPoint;
    }

    app_reg_module(module_1_task);

    /* Init code finish */
label_exitPoint:
    if ( NO_ERROR == processStatus ) {
#if APP_LOG_LEVEL>=LOG_LEVEL_VERBOSE
        DDL_LOGI(TAG, "App initialized.");
#endif
    } else {
#if APP_LOG_LEVEL>=LOG_LEVEL_ERROR
        DDL_LOGE(TAG, "App init failed.");
#endif
    }

    return processStatus;
}


int app_idle(void) {
    int processStatus = NO_ERROR;
    ddl_base_status_t queueReceived = DDL_BASE_STATUS_OK;
    app_evt_t currentEvent = { 0 };

    while ( DDL_BASE_STATUS_OK == queueReceived ) {
        queueReceived = ddl_queue_recv(appEventQueueHandle, (void *) &currentEvent);
        if ( (DDL_BASE_STATUS_OK == queueReceived) ) {
#if APP_LOG_LEVEL>=LOG_LEVEL_VERBOSE
            process_evt(&currentEvent);
#endif
        } else {
#if APP_LOG_LEVEL>=LOG_LEVEL_VERBOSE
            DDL_LOGI(TAG, "Event queue empty.");
#endif
        }

    }

    task_entry_t *pCurrentEntry = NULL;

    pCurrentEntry = taskList;
    while ( pCurrentEntry ) {
        pCurrentEntry->thisEntry.task();
        pCurrentEntry = pCurrentEntry->pNextEntry;
    }


label_exitPoint:
    return processStatus;
}


int app_reg_module(app_module_task_t moduleMain) {
    int processStatus = NO_ERROR;

    if ( 0 == taskEntryLast ) {
        taskList [ 0 ].thisEntry.task = moduleMain;
        taskEntryLast++;
#if APP_LOG_LEVEL>=LOG_LEVEL_VERBOSE
        DDL_LOGI(TAG, "Module main registration success.");
#endif
        goto label_exitPoint;
    }

    if ( APP_TASK_LIST_SIZE > taskEntryLast ) {
        taskList [ taskEntryLast ].pPreviousEntry = &taskList [ taskEntryLast - 1 ];
        taskList [ taskEntryLast ].thisEntry.task = moduleMain;
#if APP_LOG_LEVEL>=LOG_LEVEL_VERBOSE
        DDL_LOGI(TAG, "Module main registration success.");
#endif
        goto label_exitPoint;
    } else {
        processStatus = ERROR_NOT_ENOUGH_MEMORY;
#if APP_LOG_LEVEL>=LOG_LEVEL_ERROR
        DDL_LOGI(TAG, "Module main registration success.");
#endif
        goto label_exitPoint;
    }

label_exitPoint:
    return processStatus;
}



int app_post_evt(app_evt_t *pAppEvent) {
    int processStatus = NO_ERROR;

    ddl_queue_send((ddl_queue_handle_t) appEventQueueHandle, (app_evt_t *) pAppEvent);

    return processStatus;
}


static int process_evt(app_evt_t *const pEvent) {
    int processStatus = NO_ERROR;

    switch ( pEvent->eventType ) {
        case APP_EVT_TYPE_ENTRY:
#if APP_LOG_LEVEL>=LOG_LEVEL_VERBOSE
            DDL_LOGI(TAG, "Entry.");
#endif
            break;

        case APP_EVT_TYPE_TEST_1:
#if APP_LOG_LEVEL>=LOG_LEVEL_VERBOSE
            DDL_LOGI(TAG, "APP_EVT_TYPE_TEST_1.");
            if ( pEvent->dataBuffLen ) {
                DDL_LOGI(TAG, "Received data, length: %d", pEvent->dataBuffLen);
                for ( size_t dataBuffIndex = 0; dataBuffIndex < pEvent->dataBuffLen; dataBuffIndex++ ) {
                    DDL_LOGI(TAG, "data buffer[%d]: 0x%4.2X", dataBuffIndex, *((uint8_t *) pEvent->pDataBuff + dataBuffIndex));
                }
            }

            memset(pEvent->pDataBuff, 0, pEvent->dataBuffLen);

#endif
            break;

        case APP_EVT_TYPE_EXIT:
#if APP_LOG_LEVEL>=LOG_LEVEL_VERBOSE
            DDL_LOGI(TAG, "Exit.");
#endif
            break;

        default:
#if APP_LOG_LEVEL>=LOG_LEVEL_ERROR
            DDL_LOGE(TAG, "Invalid event received, unable to process.");
#endif
            break;
    }

    return processStatus;
}