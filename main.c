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

#include "ddl_queue.h"
#include "ddl_log.h"
#include "ddl_evt.h"


#define TAG "MAIN"

#define DELAY_COUNT_SET 0x0000F

int delayCount = 0;
uint8_t dataBuff [ 10 ] = { 1,2,3,4,5 };

int main(void) {
    ddl_evt_init();

    while ( 1 ) {
        if ( DELAY_COUNT_SET <= delayCount ) {
            delayCount = 0;
            ddl_evt_evt_t sampleEvent = { 0 };
            sampleEvent.eventType = DDL_EVT_EVT_TYPE_TEST_1;
            sampleEvent.pDataBuff = dataBuff;
            sampleEvent.dataBuffLen = (sizeof(dataBuff));

            ddl_evt_post_evt(&sampleEvent);
        } else {
            delayCount++;
        }

        ddl_evt_idle();
    }

    return 0;
}