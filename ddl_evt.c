/**
 * @file      ddl_evt.c
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
#include "ddl_evt.h"
#include "ddl_log.h"
#include "ddl_queue.h"

#include "module_1.h"

#define TAG "DDL_EVT"
#define DDL_EVT_LOG_LEVEL CONFIG_LOG_LEVEL
#define DDL_EVT_HEAP_ALLOCATION_SIZE_BYTES 1024
#define DDL_EVT_TASK_LIST_SIZE CONFIG_DDL_EVT_TASK_LIST_SIZE


typedef struct taskEntryStruct {
    struct taskEntryStruct *pPreviousEntry;
    struct taskEntryStruct *pNextEntry;
    struct {
        ddl_evt_module_task_t task;
    }thisEntry;
}task_entry_t;

static ddl_queue_handle_t ddl_evtEventQueueHandle = NULL;
static ddl_evt_evt_t ddl_evtEventQueue [ CONFIG_DDL_EVT_EVENT_QUEUE_SIZE ] = { 0 };
static task_entry_t taskList [ DDL_EVT_TASK_LIST_SIZE ] = { 0 };
static int taskEntryLast = 0;

static int process_evt(ddl_evt_evt_t *const pEvent);

int ddl_evt_init(void) {
    int processStatus = NO_ERROR;
    /* Init code goes here */
    memset((void *) ddl_evtEventQueue, 0, sizeof(ddl_evtEventQueue));
    memset((void *) taskList, 0, sizeof(taskList));

    ddl_evtEventQueueHandle = ddl_queue_create_static(sizeof(ddl_evt_evt_t), CONFIG_DDL_EVT_EVENT_QUEUE_SIZE, (uint8_t *) ddl_evtEventQueue);

    if ( !ddl_evtEventQueueHandle ) {
#if DDL_EVT_LOG_LEVEL>=LOG_LEVEL_ERROR
        DDL_LOGE(TAG, "Failed to get ddl_evtEventQueueHandle");
#endif
        processStatus = ERROR_INVALID_HANDLE;
        goto label_exitPoint;
    }

    ddl_evt_reg_module(module_1_task);

    /* Init code finish */
label_exitPoint:
    if ( NO_ERROR == processStatus ) {
#if DDL_EVT_LOG_LEVEL>=LOG_LEVEL_VERBOSE
        DDL_LOGI(TAG, "Ddl_evt initialized.");
#endif
    } else {
#if DDL_EVT_LOG_LEVEL>=LOG_LEVEL_ERROR
        DDL_LOGE(TAG, "Ddl_evt init failed.");
#endif
    }

    return processStatus;
}


int ddl_evt_idle(void) {
    int processStatus = NO_ERROR;
    ddl_base_status_t queueReceived = DDL_BASE_STATUS_OK;
    ddl_evt_evt_t currentEvent = { 0 };

    while ( DDL_BASE_STATUS_OK == queueReceived ) {
        queueReceived = ddl_queue_recv(ddl_evtEventQueueHandle, (void *) &currentEvent);
        if ( (DDL_BASE_STATUS_OK == queueReceived) ) {
#if DDL_EVT_LOG_LEVEL>=LOG_LEVEL_VERBOSE
            process_evt(&currentEvent);
#endif
        } else {
#if DDL_EVT_LOG_LEVEL>=LOG_LEVEL_VERBOSE
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


int ddl_evt_reg_module(ddl_evt_module_task_t moduleMain) {
    int processStatus = NO_ERROR;

    if ( 0 == taskEntryLast ) {
        taskList [ 0 ].thisEntry.task = moduleMain;
        taskEntryLast++;
#if DDL_EVT_LOG_LEVEL>=LOG_LEVEL_VERBOSE
        DDL_LOGI(TAG, "Module main registration success.");
#endif
        goto label_exitPoint;
    }

    if ( DDL_EVT_TASK_LIST_SIZE > taskEntryLast ) {
        taskList [ taskEntryLast ].pPreviousEntry = &taskList [ taskEntryLast - 1 ];
        taskList [ taskEntryLast ].thisEntry.task = moduleMain;
#if DDL_EVT_LOG_LEVEL>=LOG_LEVEL_VERBOSE
        DDL_LOGI(TAG, "Module main registration success.");
#endif
        goto label_exitPoint;
    } else {
        processStatus = ERROR_NOT_ENOUGH_MEMORY;
#if DDL_EVT_LOG_LEVEL>=LOG_LEVEL_ERROR
        DDL_LOGI(TAG, "Module main registration success.");
#endif
        goto label_exitPoint;
    }

label_exitPoint:
    return processStatus;
}



int ddl_evt_post_evt(ddl_evt_evt_t *pDdl_evtEvent) {
    int processStatus = NO_ERROR;

    ddl_queue_send((ddl_queue_handle_t) ddl_evtEventQueueHandle, (ddl_evt_evt_t *) pDdl_evtEvent);

    return processStatus;
}


static int process_evt(ddl_evt_evt_t *const pEvent) {
    int processStatus = NO_ERROR;

    switch ( pEvent->eventType ) {
        case DDL_EVT_EVT_TYPE_ENTRY:
#if DDL_EVT_LOG_LEVEL>=LOG_LEVEL_VERBOSE
            DDL_LOGI(TAG, "Entry.");
#endif
            break;

        case DDL_EVT_EVT_TYPE_TEST_1:
#if DDL_EVT_LOG_LEVEL>=LOG_LEVEL_VERBOSE
            DDL_LOGI(TAG, "DDL_EVT_EVT_TYPE_TEST_1.");
            if ( pEvent->dataBuffLen ) {
                DDL_LOGI(TAG, "Received data, length: %d", pEvent->dataBuffLen);
                for ( size_t dataBuffIndex = 0; dataBuffIndex < pEvent->dataBuffLen; dataBuffIndex++ ) {
                    DDL_LOGI(TAG, "data buffer[%d]: 0x%4.2X", dataBuffIndex, *((uint8_t *) pEvent->pDataBuff + dataBuffIndex));
                }
            }

            memset(pEvent->pDataBuff, 0, pEvent->dataBuffLen);

#endif
            break;

        case DDL_EVT_EVT_TYPE_EXIT:
#if DDL_EVT_LOG_LEVEL>=LOG_LEVEL_VERBOSE
            DDL_LOGI(TAG, "Exit.");
#endif
            break;

        default:
#if DDL_EVT_LOG_LEVEL>=LOG_LEVEL_ERROR
            DDL_LOGE(TAG, "Invalid event received, unable to process.");
#endif
            break;
    }

    return processStatus;
}