/**
 * @file      test_event.c
 * @author:   Shubhendu B B
 * @date:     10/08/2024
 * @brief
 * @details
 *
 * @copyright
 *
 **/

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "ddl_evt.h"
#include "ddl_log.h"
#include "ddl_queue.h"
#include "module_template_1.h"

#define TAG "TEST_EVENT"

enum {
    LOOP_COUNT_SET_1 = 0x0000F,
    LOOP_COUNT_SET_2 = 0x0001F,
    LOOP_COUNT_SET_3 = 0x0002F,
    LOOP_COUNT_SET_4 = 0x0003F,
    LOOP_COUNT_MAX
} loop_count_set_t;

static int loopCount = 0;
static uint8_t dataBuff[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

int test_event(void) {
    ddl_evt_init();
    ddl_evt_reg_module(module_template_1_task);

    while (1) {
        if (LOOP_COUNT_MAX <= loopCount) {
            loopCount = 0;
        } else {
            ddl_evt_t sampleEvent = {0};
            sampleEvent.nextModule = module_template_1_task;

            switch (loopCount) {
                case LOOP_COUNT_SET_1:
                    sampleEvent.eventType = MODULE_TEMPLATE_1_EVT_TYPE_TEST_1;
                    sampleEvent.pDataBuff = dataBuff;
                    sampleEvent.dataBuffLen = (sizeof(dataBuff));
                    ddl_evt_post(&sampleEvent);
                    break;

                case LOOP_COUNT_SET_2:
                    sampleEvent.eventType = MODULE_TEMPLATE_1_EVT_TYPE_TEST_2;
                    ddl_evt_post(&sampleEvent);
                    break;

                case LOOP_COUNT_SET_3:
                    break;

                default:
                    break;
            }
            loopCount++;
        }

        ddl_evt_consumer();
    }

    return 0;
}