/**
 * @file      main.c
 * @author:   Shubhendu B B
 * @date:     04/02/2023
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

#include "ddl_task.h"
#include "module_1.h"
#include "module_2.h"
#include "module_template.h"


#define TAG "MAIN"

#define DELAY_COUNT_SET 0x0000F

static int delayCount = 0;
static uint8_t dataBuff [ 10 ] = { 1,2,3,4,5 };
static ddl_task_t taskList [ 10 ] = { 0 };

int main(void) {
    *(taskList + 0) = module_1_task;
    *(taskList + 1) = module_2_task;
    *(taskList + 2) = module_template_task;

    ddl_task_init(taskList);

    while ( 1 ) {
        if ( DELAY_COUNT_SET <= delayCount ) {
            delayCount = 0;
            ddl_evt_t sampleEvent = { 0 };
            sampleEvent.eventType = DDL_EVT_TYPE_TEST_1;
            sampleEvent.pDataBuff = dataBuff;
            sampleEvent.dataBuffLen = (sizeof(dataBuff));

            ddl_task_post_evt(&sampleEvent);
        } else {
            delayCount++;
        }

        ddl_task_idle();
    }

    return 0;
}