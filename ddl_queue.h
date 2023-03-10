/**
 * @file      ddl_utils.h
 * @author:   Shubhendu B B
 * @date:     02/02/2023
 * @brief
 * @details
 *
 * @copyright
 *
 **/

#ifndef __DDL_UTILS_H__
#define __DDL_UTILS_H__

#include "ddl_base.h"
#include "ddl_commons.h"

#define QUEUE_VERSION ((const char*)"version.txt")

typedef struct ddl_utils_tagQueueStruct {
    int32_t elemSpace;
    int32_t elemSizeBytes;
    int32_t elemInQueue;
    int32_t front;
    int32_t rear;
    uint8_t* pElementBuffer;
    struct {
        unsigned isLocked : 1;
    };
} ddl_utils_queueStruct_t;
typedef ddl_utils_queueStruct_t* ddl_utils_queueHandle_t;

#ifdef __cplusplus
extern "C" {
#endif

ddl_utils_queueHandle_t ddl_utils_queue_create(uint32_t elementCount, uint32_t elementSizeBytes);

void ddl_queue_print(ddl_utils_queueHandle_t queue);
ddl_base_status_t ddl_queue_put(ddl_utils_queueHandle_t queueHandle, void* pElement);
ddl_base_status_t ddl_queue_get(ddl_utils_queueHandle_t queueHandle, void* pBuffer);
void ddl_queue_print(ddl_utils_queueHandle_t queue);

WORD CRC16(const BYTE* nData, WORD wLength);

#ifdef __cplusplus
}
#endif

#endif /* @end __DDL_UTILS_H__*/