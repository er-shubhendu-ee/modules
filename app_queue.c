/**
 * @file      app_queue.c
 * @author:   Shubhendu B B
 * @date:     18/05/2023
 * @brief
 * @details
 *
 * @copyright
 *
 **/

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define PROJ_VER "0.0.2"

#define APP_QUEUE_LOG_LEVEL_MAX (int)0xFF

#include "app_queue.h"

storageQueueHandle_t gStorageQueueHandle;

storageQueueHandle_t app_helper_StorageQueueInit(void) {
#if defined(LOG_LEVEL_VERBOSE)
    DDL_LOG("%s : %d : entry", __func__, __LINE__);
#endif
    printf("%s : %d : using queue version: %s\n", __func__, __LINE__, PROJ_VER);

    gStorageQueueHandle = (storageQueueHandle_t)malloc(sizeof(storageQueue_t));

    if (gStorageQueueHandle) {
        memset((uint8_t *)gStorageQueueHandle, 0, sizeof(storageQueue_t));
        gStorageQueueHandle->elemSpace = LOG_STORAGE_COUNT_MAX;
        gStorageQueueHandle->elemSizeBytes = sizeof(logStorageStruct_t);
        gStorageQueueHandle->front = gStorageQueueHandle->rear = -1;

        for (size_t i = 0; i < gStorageQueueHandle->elemSpace; i++) {
            gStorageQueueHandle->storage[i].logLevel = APP_QUEUE_LOG_LEVEL_MAX;
        }
    }

    return gStorageQueueHandle;
}

int app_helper_QueuePut(uint64_t logTimeStamp, int logLevel, int logId) {
#if defined(LOG_LEVEL_VERBOSE)
    DDL_LOG("%s : %d : entry", __func__, __LINE__);
#endif
    int processStatus = 0;
    bool isOkToPutToRear = false;

#if defined(LOG_LEVEL_VERBOSE)
    printf("%s : %d : on entry front: %d\n", __func__, __LINE__, gStorageQueueHandle->front);
    printf("%s : %d : on entry rear: %d\n", __func__, __LINE__, gStorageQueueHandle->rear);
#endif

    logStorageStruct_t tempLogStorageStruct = {.timeStamp = logTimeStamp, .logLevel = logLevel, .idCode = logId};

    if (gStorageQueueHandle->isLocked) {
        processStatus = 1;
    }

    gStorageQueueHandle->isLocked = true;

    if (gStorageQueueHandle->elemInQueue == gStorageQueueHandle->elemSpace) {
        for (size_t i = 0; i < gStorageQueueHandle->elemSpace; i++) {
            if (gStorageQueueHandle->storage[i].logLevel > tempLogStorageStruct.logLevel) {
                memcpy(&gStorageQueueHandle->storage[i], &tempLogStorageStruct, gStorageQueueHandle->elemSizeBytes);
                processStatus = 0;
                break;
            }
            processStatus = -1;
        }
        goto label_exitPoint;
    }

    if (gStorageQueueHandle->rear >= gStorageQueueHandle->front) {
        if (gStorageQueueHandle->rear == gStorageQueueHandle->elemSpace - 1) {
            if (gStorageQueueHandle->front > 0) {
                gStorageQueueHandle->rear = 0;
                isOkToPutToRear = true;
            } else {
                processStatus = -1;
                goto label_exitPoint;
            }
        } else {
            gStorageQueueHandle->rear++;
            isOkToPutToRear = true;
        }
    } else if (gStorageQueueHandle->rear + 1 < gStorageQueueHandle->front) {
        gStorageQueueHandle->rear++;
        isOkToPutToRear = true;
    } else {
        processStatus = -1;
        goto label_exitPoint;
    }

    if (isOkToPutToRear) {
#if defined(LOG_LEVEL_VERBOSE)
        printf("%s : %d : copy to %lu from %lu, %d bytes\n", __func__, __LINE__,
               (uint8_t *)(gStorageQueueHandle->pElementBuffer +
                           gStorageQueueHandle->rear * gStorageQueueHandle->elemSizeBytes),
               (uint8_t *)pElement, gStorageQueueHandle->elemSizeBytes);
#endif
        memcpy((uint8_t *)(&gStorageQueueHandle->storage[gStorageQueueHandle->rear]), (uint8_t *)&tempLogStorageStruct,
               gStorageQueueHandle->elemSizeBytes);
        gStorageQueueHandle->elemInQueue++;
        processStatus = 0;

        if (gStorageQueueHandle->front == -1) {
            gStorageQueueHandle->front = 0;
        } else {
            /* code */
        }
    } else {
        for (size_t i = 0; i < gStorageQueueHandle->elemSpace; i++) {
            if (gStorageQueueHandle->storage[i].logLevel > tempLogStorageStruct.logLevel) {
                memcpy(&gStorageQueueHandle->storage[i], &tempLogStorageStruct, gStorageQueueHandle->elemSizeBytes);
                if (gStorageQueueHandle->elemInQueue < gStorageQueueHandle->elemSpace) {
                    gStorageQueueHandle->elemInQueue++;
                }
                processStatus = 0;
                break;
            }
            processStatus = -1;
        }
    }

label_exitPoint:
    gStorageQueueHandle->isLocked = false;

#if defined(LOG_LEVEL_VERBOSE)
    printf("%s : %d : %s on exit front: %d\n", __func__, __LINE__, gStorageQueueHandle->front);
    printf("%s : %d : %s on exit rear: %d\n", __func__, __LINE__, gStorageQueueHandle->rear);
#endif

#if defined(LOG_LEVEL_VERBOSE)
    DDL_LOG("%s : %d : exit", __func__, __LINE__);
#endif
    return processStatus;
}

int app_helper_QueueGet(logStorageStruct_t *pLogStorageStructBuff, int logCount) {
#if defined(LOG_LEVEL_VERBOSE)
    DDL_LOG("%s : %d : entry");
#endif
    int processStatus = 0;

#if defined(LOG_LEVEL_VERBOSE)
    printf("%s : %d : on entry front: %d\n", __func__, __LINE__, gStorageQueueHandle->front);
    printf("%s : %d : on entry rear: %d\n", __func__, __LINE__, gStorageQueueHandle->rear);
#endif

    if (gStorageQueueHandle->isLocked) {
        processStatus = -1;
    }

    gStorageQueueHandle->isLocked = true;

    if (gStorageQueueHandle->front == -1) {
#if defined(LOG_LEVEL_VERBOSE)
        printf("%s : %d : queue empty", __func__, __LINE__);
#endif
        processStatus = -1;
        goto label_exitPoint;
    }

#if defined(LOG_LEVEL_VERBOSE)
    printf("%s : %d : copy to %lu from %lu, %d bytes\n", __func__, __LINE__, (uint8_t *)pBuffer,
           (uint8_t *)(gStorageQueueHandle->pElementBuffer +
                       gStorageQueueHandle->rear * gStorageQueueHandle->elemSizeBytes),
           gStorageQueueHandle->elemSizeBytes);
#endif

    memcpy((uint8_t *)pLogStorageStructBuff, (uint8_t *)&gStorageQueueHandle->storage[gStorageQueueHandle->front],
           gStorageQueueHandle->elemSizeBytes);
    gStorageQueueHandle->elemInQueue--;

    if (gStorageQueueHandle->front == gStorageQueueHandle->rear) {
        gStorageQueueHandle->front = gStorageQueueHandle->rear = -1;
    } else {
        if (gStorageQueueHandle->front == gStorageQueueHandle->elemSpace - 1) {
            gStorageQueueHandle->front = 0;
        } else {
            (gStorageQueueHandle->front)++;
        }
    }

label_exitPoint:
    gStorageQueueHandle->isLocked = false;

#if defined(LOG_LEVEL_VERBOSE)
    printf("%s : %d : on exit front: %d\n", __func__, __LINE__, gStorageQueueHandle->front);
    printf("%s : %d : on exit rear: %d\n", __func__, __LINE__, gStorageQueueHandle->rear);
#endif

#if defined(LOG_LEVEL_VERBOSE)
    DDL_LOG("%s : %d : exit");
#endif
    return processStatus;
}

int app_helper_PrintStorageQueue(void) {
    int i;
    if (0 == gStorageQueueHandle->elemInQueue) {
        printf("%s : %d : Empty Queue\r\n", __func__, __LINE__);
        return 0;
    } else {
        printf("%s : %d : Front: %d\r\n", __func__, __LINE__, gStorageQueueHandle->front);
        printf("%s : %d : Items:\r\n", __func__, __LINE__);
        for (i = gStorageQueueHandle->front; i <= gStorageQueueHandle->rear;
             i = (i + 1) % sizeof(gStorageQueueHandle->storage)) {
            printf("%s : %d : log time stamp = %d \r\n", __func__, __LINE__, gStorageQueueHandle->storage[i].timeStamp);
            printf("%s : %d : log level = %d \r\n", __func__, __LINE__, gStorageQueueHandle->storage[i].logLevel);
            printf("%s : %d : log id = %d \r\n", __func__, __LINE__, gStorageQueueHandle->storage[i].idCode);
        }
        printf("%s : %d : Rear: %d\r\n", __func__, __LINE__, gStorageQueueHandle->rear);
    }
}