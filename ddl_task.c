/**
 * @file      ddl_task.c
 * @author:   Shubhendu B B
 * @date:     18/10/2023
 * @brief
 * @details
 *
 * @copyright
 *
**/

#include <error.h>
#include <string.h>

#include "config.h"
#include "ddl_task.h"
#include "ddl_log.h"
#include "ddl_queue.h"

#include "module_1.h"

#define TAG "DDL_TASK"
#define DDL_TASK_LOG_LEVEL CONFIG_LOG_LEVEL
#define DDL_TASK_MEMORY_MODEL_IS_DYNAMIC CONFIG_MEMORY_MODEL_IS_DYNAMIC


typedef struct taskEntryStruct {
    struct taskEntryStruct *pPreviousEntry;
    struct taskEntryStruct *pNextEntry;
    struct {
        ddl_task_t task;
    }thisEntry;
}task_entry_t;

static ddl_queue_handle_t eventQueueHandle = NULL;
static ddl_evt_t eventQueue [ CONFIG_DDL_EVT_QUEUE_SIZE ] = { 0 };
static task_entry_t taskList [ CONFIG_DDL_TASK_LIST_SIZE ] = { 0 };
static int taskEntryLast = 0;

static int process_evt(ddl_evt_t *const pEvent);

int ddl_task_init(const ddl_task_t *const pTaskList) {
    int processStatus = NO_ERROR;
    /* Init code goes here */
    if ( !pTaskList ) {
#if DDL_TASK_LOG_LEVEL>=DDL_LOG_LEVEL_INFO
        DDL_LOGW(TAG, "Task list empty.");
#endif       
        goto label_exitPoint;
    }

    memset((void *) eventQueue, 0, sizeof(eventQueue));
    memset((void *) taskList, 0, sizeof(taskList));

#if (!DDL_TASK_MEMORY_MODEL_IS_DYNAMIC)
    eventQueueHandle = ddl_queue_create_static(sizeof(ddl_evt_t), CONFIG_DDL_EVT_QUEUE_SIZE, (uint8_t *) eventQueue);
#else
    eventQueueHandle = ddl_queue_create(sizeof(ddl_evt_t), CONFIG_DDL_EVT_QUEUE_SIZE);
#endif

    if ( !eventQueueHandle ) {
#if DDL_TASK_LOG_LEVEL>=DDL_LOG_LEVEL_ERROR
        DDL_LOGE(TAG, "Failed to get eventQueueHandle");
#endif
        processStatus = ERROR_INVALID_HANDLE;
        goto label_exitPoint;
    }

    for ( size_t taskListIndex = 0; *(pTaskList + taskListIndex); taskListIndex++ ) {
        ddl_task_reg_module(*(pTaskList + taskListIndex));
    }

    /* Init code finish */
label_exitPoint:
    if ( NO_ERROR == processStatus ) {
#if DDL_TASK_LOG_LEVEL>=DDL_LOG_LEVEL_VERBOSE
        DDL_LOGI(TAG, "Task initialized.");
#endif
    } else {
#if DDL_TASK_LOG_LEVEL>=DDL_LOG_LEVEL_ERROR
        DDL_LOGE(TAG, "Task init failed.");
#endif
    }

    return processStatus;
}


int ddl_task_idle(void) {
    int processStatus = NO_ERROR;
    ddl_base_status_t queueReceived = DDL_BASE_STATUS_OK;
    ddl_evt_t currentEvent = { 0 };

    while ( DDL_BASE_STATUS_OK == queueReceived ) {
        queueReceived = ddl_queue_recv(eventQueueHandle, (void *) &currentEvent);
        if ( (DDL_BASE_STATUS_OK == queueReceived) ) {
#if DDL_TASK_LOG_LEVEL>=DDL_LOG_LEVEL_VERBOSE
            process_evt(&currentEvent);
#endif
        } else {
#if DDL_TASK_LOG_LEVEL>=DDL_LOG_LEVEL_VERBOSE
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


int ddl_task_reg_module(ddl_task_t moduleMain) {
    int processStatus = NO_ERROR;

    if ( 0 == taskEntryLast ) {
        taskList [ 0 ].thisEntry.task = moduleMain;
        taskEntryLast++;
#if DDL_TASK_LOG_LEVEL>=DDL_LOG_LEVEL_VERBOSE
        DDL_LOGI(TAG, "Module main registration success.");
#endif
        goto label_exitPoint;
    }

    if ( CONFIG_DDL_TASK_LIST_SIZE > taskEntryLast ) {
        taskList [ taskEntryLast ].pPreviousEntry = &taskList [ taskEntryLast - 1 ];
        taskList [ taskEntryLast ].thisEntry.task = moduleMain;
#if DDL_TASK_LOG_LEVEL>=DDL_LOG_LEVEL_VERBOSE
        DDL_LOGI(TAG, "Module main registration success.");
#endif
        goto label_exitPoint;
    } else {
        processStatus = ERROR_NOT_ENOUGH_MEMORY;
#if DDL_TASK_LOG_LEVEL>=DDL_LOG_LEVEL_ERROR
        DDL_LOGI(TAG, "Module main registration success.");
#endif
        goto label_exitPoint;
    }

label_exitPoint:
    return processStatus;
}



int ddl_task_post_evt(ddl_evt_t *pEvent) {
    int processStatus = NO_ERROR;

    ddl_queue_send((ddl_queue_handle_t) eventQueueHandle, (ddl_evt_t *) pEvent);

    return processStatus;
}


static int process_evt(ddl_evt_t *const pEvent) {
    int processStatus = NO_ERROR;

    switch ( pEvent->eventType ) {
        case DDL_EVT_TYPE_ENTRY:
#if DDL_TASK_LOG_LEVEL>=DDL_LOG_LEVEL_VERBOSE
            DDL_LOGI(TAG, "Entry.");
#endif
            break;

        case DDL_EVT_TYPE_TEST_1:
#if DDL_TASK_LOG_LEVEL>=DDL_LOG_LEVEL_VERBOSE
            DDL_LOGI(TAG, "DDL_EVT_TYPE_TEST_1.");
            if ( pEvent->dataBuffLen ) {
                DDL_LOGI(TAG, "Received data, length: %d", pEvent->dataBuffLen);
                for ( size_t dataBuffIndex = 0; dataBuffIndex < pEvent->dataBuffLen; dataBuffIndex++ ) {
                    DDL_LOGI(TAG, "data buffer[%d]: 0x%4.2X", dataBuffIndex, *((uint8_t *) pEvent->pDataBuff + dataBuffIndex));
                }
            }

            memset(pEvent->pDataBuff, 0, pEvent->dataBuffLen);

#endif
            break;

        case DDL_EVT_TYPE_EXIT:
#if DDL_TASK_LOG_LEVEL>=DDL_LOG_LEVEL_VERBOSE
            DDL_LOGI(TAG, "Exit.");
#endif
            break;

        default:
#if DDL_TASK_LOG_LEVEL>=DDL_LOG_LEVEL_ERROR
            DDL_LOGE(TAG, "Invalid event received, unable to process.");
#endif
            break;
    }

    return processStatus;
}