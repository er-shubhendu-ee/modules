/**
 * @file      event_root.c
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
#include "event_root.h"
#include "ddl_log.h"
#include "ddl_queue.h"

#include "module_1.h"

#define TAG "EVENT_ROOT"
#define EVENT_ROOT_LOG_LEVEL CONFIG_LOG_LEVEL
#define EVENT_ROOT_HEAP_ALLOCATION_SIZE_BYTES 1024
#define EVENT_ROOT_TASK_LIST_SIZE CONFIG_EVENT_ROOT_TASK_LIST_SIZE


typedef struct taskEntryStruct {
    struct taskEntryStruct *pPreviousEntry;
    struct taskEntryStruct *pNextEntry;
    struct {
        event_root_module_task_t task;
    }thisEntry;
}task_entry_t;

static ddl_queue_handle_t event_rootEventQueueHandle = NULL;
static event_root_evt_t event_rootEventQueue [ CONFIG_EVENT_ROOT_EVENT_QUEUE_SIZE ] = { 0 };
static task_entry_t taskList [ EVENT_ROOT_TASK_LIST_SIZE ] = { 0 };
static int taskEntryLast = 0;

static int process_evt(event_root_evt_t *const pEvent);

int event_root_init(void) {
    int processStatus = NO_ERROR;
    /* Init code goes here */
    memset((void *) event_rootEventQueue, 0, sizeof(event_rootEventQueue));
    memset((void *) taskList, 0, sizeof(taskList));

    event_rootEventQueueHandle = ddl_queue_create_static(sizeof(event_root_evt_t), CONFIG_EVENT_ROOT_EVENT_QUEUE_SIZE, (uint8_t *) event_rootEventQueue);

    if ( !event_rootEventQueueHandle ) {
#if EVENT_ROOT_LOG_LEVEL>=LOG_LEVEL_ERROR
        DDL_LOGE(TAG, "Failed to get event_rootEventQueueHandle");
#endif
        processStatus = ERROR_INVALID_HANDLE;
        goto label_exitPoint;
    }

    event_root_reg_module(module_1_task);

    /* Init code finish */
label_exitPoint:
    if ( NO_ERROR == processStatus ) {
#if EVENT_ROOT_LOG_LEVEL>=LOG_LEVEL_VERBOSE
        DDL_LOGI(TAG, "Event_root initialized.");
#endif
    } else {
#if EVENT_ROOT_LOG_LEVEL>=LOG_LEVEL_ERROR
        DDL_LOGE(TAG, "Event_root init failed.");
#endif
    }

    return processStatus;
}


int event_root_idle(void) {
    int processStatus = NO_ERROR;
    ddl_base_status_t queueReceived = DDL_BASE_STATUS_OK;
    event_root_evt_t currentEvent = { 0 };

    while ( DDL_BASE_STATUS_OK == queueReceived ) {
        queueReceived = ddl_queue_recv(event_rootEventQueueHandle, (void *) &currentEvent);
        if ( (DDL_BASE_STATUS_OK == queueReceived) ) {
#if EVENT_ROOT_LOG_LEVEL>=LOG_LEVEL_VERBOSE
            process_evt(&currentEvent);
#endif
        } else {
#if EVENT_ROOT_LOG_LEVEL>=LOG_LEVEL_VERBOSE
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


int event_root_reg_module(event_root_module_task_t moduleMain) {
    int processStatus = NO_ERROR;

    if ( 0 == taskEntryLast ) {
        taskList [ 0 ].thisEntry.task = moduleMain;
        taskEntryLast++;
#if EVENT_ROOT_LOG_LEVEL>=LOG_LEVEL_VERBOSE
        DDL_LOGI(TAG, "Module main registration success.");
#endif
        goto label_exitPoint;
    }

    if ( EVENT_ROOT_TASK_LIST_SIZE > taskEntryLast ) {
        taskList [ taskEntryLast ].pPreviousEntry = &taskList [ taskEntryLast - 1 ];
        taskList [ taskEntryLast ].thisEntry.task = moduleMain;
#if EVENT_ROOT_LOG_LEVEL>=LOG_LEVEL_VERBOSE
        DDL_LOGI(TAG, "Module main registration success.");
#endif
        goto label_exitPoint;
    } else {
        processStatus = ERROR_NOT_ENOUGH_MEMORY;
#if EVENT_ROOT_LOG_LEVEL>=LOG_LEVEL_ERROR
        DDL_LOGI(TAG, "Module main registration success.");
#endif
        goto label_exitPoint;
    }

label_exitPoint:
    return processStatus;
}



int event_root_post_evt(event_root_evt_t *pEvent_rootEvent) {
    int processStatus = NO_ERROR;

    ddl_queue_send((ddl_queue_handle_t) event_rootEventQueueHandle, (event_root_evt_t *) pEvent_rootEvent);

    return processStatus;
}


static int process_evt(event_root_evt_t *const pEvent) {
    int processStatus = NO_ERROR;

    switch ( pEvent->eventType ) {
        case EVENT_ROOT_EVT_TYPE_ENTRY:
#if EVENT_ROOT_LOG_LEVEL>=LOG_LEVEL_VERBOSE
            DDL_LOGI(TAG, "Entry.");
#endif
            break;

        case EVENT_ROOT_EVT_TYPE_TEST_1:
#if EVENT_ROOT_LOG_LEVEL>=LOG_LEVEL_VERBOSE
            DDL_LOGI(TAG, "EVENT_ROOT_EVT_TYPE_TEST_1.");
            if ( pEvent->dataBuffLen ) {
                DDL_LOGI(TAG, "Received data, length: %d", pEvent->dataBuffLen);
                for ( size_t dataBuffIndex = 0; dataBuffIndex < pEvent->dataBuffLen; dataBuffIndex++ ) {
                    DDL_LOGI(TAG, "data buffer[%d]: 0x%4.2X", dataBuffIndex, *((uint8_t *) pEvent->pDataBuff + dataBuffIndex));
                }
            }

            memset(pEvent->pDataBuff, 0, pEvent->dataBuffLen);

#endif
            break;

        case EVENT_ROOT_EVT_TYPE_EXIT:
#if EVENT_ROOT_LOG_LEVEL>=LOG_LEVEL_VERBOSE
            DDL_LOGI(TAG, "Exit.");
#endif
            break;

        default:
#if EVENT_ROOT_LOG_LEVEL>=LOG_LEVEL_ERROR
            DDL_LOGE(TAG, "Invalid event received, unable to process.");
#endif
            break;
    }

    return processStatus;
}