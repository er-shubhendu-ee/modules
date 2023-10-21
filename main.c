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
#include "event_root.h"


#define TAG "MAIN"

#define DELAY_COUNT_SET 0x0000F

int delayCount = 0;
uint8_t dataBuff [ 10 ] = { 1,2,3,4,5 };

int main(void) {
    event_root_init();

    while ( 1 ) {
        if ( DELAY_COUNT_SET <= delayCount ) {
            delayCount = 0;
            event_root_evt_t sampleEvent = { 0 };
            sampleEvent.eventType = EVENT_ROOT_EVT_TYPE_TEST_1;
            sampleEvent.pDataBuff = dataBuff;
            sampleEvent.dataBuffLen = (sizeof(dataBuff));

            event_root_post_evt(&sampleEvent);
        } else {
            delayCount++;
        }

        event_root_idle();
    }

    return 0;
}