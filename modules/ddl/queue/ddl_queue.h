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

typedef struct {
    int32_t elemSpace;
    int32_t elemSizeBytes;
    int32_t elemInQueue;
    int32_t front;
    int32_t rear;
    uint8_t *pElementBuffer;
    struct {
        unsigned isLocked : 1;
    };
} ddl_queue_struct_t;
typedef ddl_queue_struct_t *ddl_queue_handle_t;

#ifdef __cplusplus
extern "C" {
#endif

ddl_queue_handle_t ddl_queue_create(uint32_t elementSizeBytes, uint32_t elementCount);

ddl_queue_handle_t ddl_queue_create_static(uint32_t elementSize, uint32_t elementCount,
                                           uint8_t *pElementArray);

ddl_base_status_t ddl_queue_send(ddl_queue_handle_t queueHandle, void *pElement);
ddl_base_status_t ddl_queue_recv(ddl_queue_handle_t queueHandle, void *pBuffer);
void ddl_queue_print(ddl_queue_handle_t queue);
ddl_base_status_t ddl_queue_delete(ddl_queue_handle_t queue);

#ifdef __cplusplus
}
#endif

#endif /* @end __DDL_QUEUE_H__*/