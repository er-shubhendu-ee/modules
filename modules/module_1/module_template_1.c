
/**
 * @file      module_template_1.c
 * @author:   Shubhendu B B
 * @date:     31/10/2023
 * @brief
 * @details
 *
 * @copyright
 *
**/

#include <error.h>

#include "config.h"
#include "module_template_1.h"
#include "ddl_log.h"
#include "ddl_evt.h"
#include "ddl_queue.h"

#define TAG "MODULE_1"

#define MODULE_TEMPLATE_1_LOG_LEVEL CONFIG_LOG_LEVEL
#define MODULE_TEMPLATE_1_EVENT_QUEUE_SIZE 10

static ddl_queue_handle_t eventQueueHandle = NULL;
static ddl_evt_t eventQueue [ MODULE_TEMPLATE_1_EVENT_QUEUE_SIZE ] = { 0 };

int module_template_1_task(void *pEvent) {
    int processStatus = NO_ERROR;

#if MODULE_TEMPLATE_1_LOG_LEVEL>=LOG_LEVEL_VERBOSE
    DDL_LOGI(TAG, "Module template 1 task.");
#endif

    if ( !pEvent ) {
#if MODULE_TEMPLATE_1_LOG_LEVEL>=LOG_LEVEL_ERROR
        DDL_LOGI(TAG, "Invalid event received.");
#endif
        goto label_exitPoint;
    }

    ddl_evt_t *pEventMsg = (ddl_evt_t *) pEvent;

    switch ( pEventMsg->eventType ) {
        case MODULE_TEMPLATE_1_EVT_TYPE_ENTRY:
#if MODULE_TEMPLATE_1_LOG_LEVEL>=LOG_LEVEL_VERBOSE
            DDL_LOGI(TAG, "MODULE_TEMPLATE_1_EVT_TYPE_ENTRY");
#endif
            break;

        case  MODULE_TEMPLATE_1_EVT_TYPE_TEST_1:
#if MODULE_TEMPLATE_1_LOG_LEVEL>=LOG_LEVEL_VERBOSE
            DDL_LOGI(TAG, "MODULE_TEMPLATE_1_EVT_TYPE_TEST_1");
#endif
            if ( pEventMsg->dataBuffLen ) {
                DDL_LOGI(TAG, "Received data buffer len: %d", pEventMsg->dataBuffLen);
                for ( size_t indexI = 0; indexI < pEventMsg->dataBuffLen; indexI++ ) {
                    DDL_LOGI(TAG, "Buff[%d]: 0x%4.4X", indexI, *((uint8_t *) pEventMsg->pDataBuff + indexI));
                }
            }

            break;

        case MODULE_TEMPLATE_1_EVT_TYPE_TEST_2:
            {
#if MODULE_TEMPLATE_1_LOG_LEVEL >= LOG_LEVEL_VERBOSE
                DDL_LOGI(TAG, "MODULE_TEMPLATE_1_EVT_TYPE_TEST_2");
#endif
            }
            break;

        case MODULE_TEMPLATE_1_EVT_TYPE_TEST_3:
            {
#if MODULE_TEMPLATE_1_LOG_LEVEL >= LOG_LEVEL_VERBOSE
                DDL_LOGI(TAG, "MODULE_TEMPLATE_1_EVT_TYPE_TEST_2");
#endif
                ddl_evt_t sampleEvent = { 0 };
                sampleEvent.module = module_template_1_task;
                sampleEvent.eventType = MODULE_TEMPLATE_1_EVT_TYPE_EXIT;
                ddl_evt_post(&sampleEvent);
            }
            break;



        case  MODULE_TEMPLATE_1_EVT_TYPE_EXIT:
#if MODULE_TEMPLATE_1_LOG_LEVEL>=LOG_LEVEL_VERBOSE
            DDL_LOGI(TAG, "MODULE_TEMPLATE_1_EVT_TYPE_EXIT");
#endif
        default:
            break;
    }


label_exitPoint:
    return processStatus;
}