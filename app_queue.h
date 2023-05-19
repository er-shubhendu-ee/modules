/**
 * @file      app_queue.h
 * @author:   Shubhendu B B
 * @date:     18/05/2023
 * @brief
 * @details
 *
 * @copyright
 *
 **/

#ifndef __APP_QUEUE_H__
#define __APP_QUEUE_H__

#include <stdint.h>
#include <stdio.h>

#define LOG_STORAGE_COUNT_MAX 5

typedef struct tagLogStorageStruct {
    int logLevel;
    uint64_t timeStamp;
    int idCode;
} logStorageStruct_t;

typedef struct tagStorageQueue {
    int32_t elemSpace;
    int32_t elemSizeBytes;
    int32_t elemInQueue;
    int32_t front;
    int32_t rear;
    logStorageStruct_t storage[LOG_STORAGE_COUNT_MAX];
    struct {
        unsigned isLocked : 1;
    };
} storageQueue_t;
typedef storageQueue_t *storageQueueHandle_t;

/**
 * @brief
 *
 * @return storageQueueHandle_t [out] non-null on success, null otherwise
 */
storageQueueHandle_t app_helper_StorageQueueInit(void);

/**
 * @brief
 *
 * @return int [out] 0 if success non-zero otherwise
 */
int app_helper_StorageQueueDeinit(void);

/**
 * @brief
 *
 * @param logLevel
 * @param logId
 * @return int
 */
int app_helper_QueuePut(uint64_t timeStamp, int logLevel, int logId);

/**
 * @brief
 *
 * @param pLogStorageStructBuff [in]    large enough buffer to hold log data count defined by logCount input
 * @param logCount              [in]    log count to be read from the log storage queue
 * @return int                  [out]   >=0 :: number of logs successfully read
 *                                      -1  :: error accessing the queue/arguments invalid
 */
int app_helper_QueueGet(logStorageStruct_t *pLogStorageStructBuff, int logCount);

/**
 * @brief
 *
 * @return int [out]    >= 0 :: number of elements successfully printed
 *                      -1   :: error accessing the queue
 */
int app_helper_PrintStorageQueue(void);

#endif /* @end __APP_QUEUE_H__ */