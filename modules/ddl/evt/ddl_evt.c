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

#define TAG "DDL_EVT"
#define DDL_EVT_LOG_LEVEL CONFIG_LOG_LEVEL
#define DDL_EVT_HEAP_ALLOCATION_SIZE_BYTES 1024
#define DDL_EVT_TASK_LIST_SIZE CONFIG_DDL_EVT_MODULE_COUNT


typedef struct taskEntryStruct {
    struct taskEntryStruct *pPreviousEntry;
    struct taskEntryStruct *pNextEntry;
    struct {
        ddl_evt_module_t nextModule;
    }this;
}task_entry_t;

static ddl_queue_handle_t eventQueueHandle = NULL;
static ddl_evt_t eventQueue [ CONFIG_DDL_EVT_EVENT_QUEUE_SIZE ] = { 0 };
static task_entry_t taskList [ DDL_EVT_TASK_LIST_SIZE ] = { 0 };
static int taskEntryLast = 0;



int ddl_evt_init(void) {
    int processStatus = NO_ERROR;
    /* Init code goes here */
    memset((void *) eventQueue, 0, sizeof(eventQueue));
    memset((void *) taskList, 0, sizeof(taskList));

    eventQueueHandle = ddl_queue_create_static(sizeof(ddl_evt_t), CONFIG_DDL_EVT_EVENT_QUEUE_SIZE, (uint8_t *) eventQueue);

    if ( !eventQueueHandle ) {
#if DDL_EVT_LOG_LEVEL>=LOG_LEVEL_ERROR
        DDL_LOGE(TAG, "Failed to get eventQueueHandle");
#endif
        processStatus = ERROR_INVALID_HANDLE;
        goto label_exitPoint;
    }

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


int ddl_evt_consumer(void) {
    int processStatus = NO_ERROR;
    ddl_base_status_t queueReceived = DDL_BASE_STATUS_OK;
    ddl_evt_t currentEvent = { 0 };

    while ( DDL_BASE_STATUS_OK == queueReceived ) {
        queueReceived = ddl_queue_recv(eventQueueHandle, (void *) &currentEvent);
        if ( (DDL_BASE_STATUS_OK != queueReceived) ) {
#if DDL_EVT_LOG_LEVEL>=LOG_LEVEL_VERBOSE
            DDL_LOGI(TAG, "Event queue empty.");
#endif
            break;
        }

        if ( !currentEvent.nextModule ) {
#if DDL_EVT_LOG_LEVEL>=LOG_LEVEL_ERROR
            DDL_LOGI(TAG, "No nextModule to process for the received event %d.", currentEvent.eventType);
#endif
            continue;
        }

        currentEvent.nextModule(&currentEvent);
    }

label_exitPoint:
    return processStatus;
}


int ddl_evt_reg_module(ddl_evt_module_t moduleMain) {
    int processStatus = NO_ERROR;

    if ( 0 == taskEntryLast ) {
        taskList [ 0 ].this.nextModule = moduleMain;
        taskEntryLast++;
#if DDL_EVT_LOG_LEVEL>=LOG_LEVEL_VERBOSE
        DDL_LOGI(TAG, "Module main registration success.");
#endif
        goto label_exitPoint;
    }

    if ( DDL_EVT_TASK_LIST_SIZE > taskEntryLast ) {
        taskList [ taskEntryLast ].pPreviousEntry = &taskList [ taskEntryLast - 1 ];
        taskList [ taskEntryLast ].this.nextModule = moduleMain;
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



int ddl_evt_post(ddl_evt_t *pDdl_evtEvent) {
    int processStatus = NO_ERROR;

    ddl_queue_send((ddl_queue_handle_t) eventQueueHandle, (ddl_evt_t *) pDdl_evtEvent);

    return processStatus;
}

