/**
 * @file      app.h
 * @author:   Shubhendu B B
 * @date:     18/05/2023
 * @brief
 * @details
 *
 * @copyright
 *
 **/

#ifndef __APP_H__
#define __APP_H__

#include <stdint.h>
#include <stdio.h>

#include "ddl_queue.h"

#define LOG_STORAGE_COUNT_MAX 10

typedef struct {
    int logLevel;
    uint64_t timeStamp;
    int idCode;
} app_logElementStruct_t;

/**
 * @brief
 *
 * @return storageQueueHandle_t [out] non-null on success, null otherwise
 */
ddl_queue_handle_t app_queue_init(void);

/**
 * @brief
 *
 * @return int [out] 0 if success non-zero otherwise
 */
int app_queue_deinit(void);

/**
 * @brief
 *
 * @param logLevel
 * @param logId
 * @return int
 */
int app_queue_put(uint64_t timeStamp, int logLevel, int logId);

/**
 * @brief
 *
 * @param pLogStorageStructBuff [in]    large enough buffer to hold log data count defined by logCount input
 * @param logCount              [in]    log count to be read from the log storage queue
 * @return int                  [out]   >=0 :: number of logs successfully read
 *                                      -1  :: error accessing the queue/arguments invalid
 */
int app_queue_get(app_logElementStruct_t *pLogStorageStructBuff, int logCount);

/**
 * @brief
 *
 * @return int [out]    >= 0 :: number of elements successfully printed
 *                      -1   :: error accessing the queue
 */
int app_print_queue(void);

#endif /* @end __APP_H__ */