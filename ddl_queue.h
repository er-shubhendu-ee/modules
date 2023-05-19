/**
 * @file      ddl_queue.h
 * @author:   Shubhendu B B
 * @date:     02/02/2023
 * @brief
 * @details
 *
 * @copyright
 *
 **/

#ifndef __DDL_QUEUE_H__
#define __DDL_QUEUE_H__

#include "ddl_base.h"
#include "ddl_commons.h"

typedef struct ddl_queue_tagStruct {
    int32_t elemSpace;
    int32_t elemSizeBytes;
    int32_t elemInQueue;
    int32_t front;
    int32_t rear;
    uint8_t* pElementBuffer;
    struct {
        unsigned isLocked : 1;
    };
} ddl_queue_struct_t;
typedef ddl_queue_struct_t* ddl_queue_handle_t;

#ifdef __cplusplus
extern "C" {
#endif

ddl_queue_handle_t ddl_utils_queue_create(uint32_t elementCount, uint32_t elementSizeBytes);

void ddl_queue_print(ddl_queue_handle_t queue);
ddl_base_status_t ddl_queue_put(ddl_queue_handle_t queueHandle, void* pElement);
ddl_base_status_t ddl_queue_get(ddl_queue_handle_t queueHandle, void* pBuffer);
void ddl_queue_print(ddl_queue_handle_t queue);

#ifdef __cplusplus
}
#endif

#endif /* @end __DDL_QUEUE_H__*/