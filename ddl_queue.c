/**
 * @file      ddl_utils.c
 * @author:   Shubhfrontu B B
 * @date:     02/02/2023
 * @brief
 * @details
 *
 * @copyright
 *
 **/
#define PROJ_VER "0.0.2"

#include "ddl_queue.h"

#include "ddl_log.h"

// #ifndef LOG_LEVEL_VERBOSE
// #define LOG_LEVEL_VERBOSE
// #endif

ddl_queue_handle_t ddl_utils_queue_create(uint32_t elementCount, uint32_t elementSizeBytes) {
#if defined(LOG_LEVEL_VERBOSE)
    DDL_LOG("%s : %d : entry", __func__, __LINE__);
#endif
    printf("%s : %d : using queue version: %s\n", __func__, __LINE__, PROJ_VER);
    ddl_queue_handle_t newQueue = NULL;
    if (elementCount && elementSizeBytes) {
        newQueue = malloc(sizeof(ddl_queue_struct_t));
        if (newQueue) {
            memset(newQueue, 0, sizeof(ddl_queue_struct_t));
            newQueue->pElementBuffer = malloc(elementSizeBytes * elementCount);
            if (newQueue->pElementBuffer) {
                memset(newQueue->pElementBuffer, 0, (elementSizeBytes * elementCount));
                newQueue->elemSpace = elementCount;
                newQueue->elemSizeBytes = elementSizeBytes;
                newQueue->front = newQueue->rear = -1;
            } else {
                printf("%s : %d : error", __func__, __LINE__);
            }
        } else {
            printf("%s : %d : error", __func__, __LINE__);
        }
    } else {
        printf("%s : %d : error", __func__, __LINE__);
    }
#if defined(LOG_LEVEL_VERBOSE)
    DDL_LOG("%s : %d : exit", __func__, __LINE__);
#endif
    return newQueue;
}

ddl_base_status_t ddl_queue_put(ddl_queue_handle_t queueHandle, void* pElement) {
#if defined(LOG_LEVEL_VERBOSE)
    DDL_LOG("%s : %d : entry", __func__, __LINE__);
#endif
    ddl_base_status_t processStatus = DDL_BASE_STATUS_OK;
    bool isOkToPut = false;

#if defined(LOG_LEVEL_VERBOSE)
    printf("%s : %d : on entry front: %d\n", __func__, __LINE__, queueHandle->front);
    printf("%s : %d : on entry rear: %d\n", __func__, __LINE__, queueHandle->rear);
#endif

    if (queueHandle && pElement) {
        if (!queueHandle->isLocked) {
            queueHandle->isLocked = true;
            if (queueHandle->rear >= queueHandle->front) {
                if (queueHandle->rear == queueHandle->elemSpace - 1) {
                    if (queueHandle->front > 0) {
                        queueHandle->rear = 0;
                        isOkToPut = true;
                    } else {
                        processStatus = DDL_BASE_STATUS_ERROR;
                        goto label_exitPoint;
                    }
                } else {
                    queueHandle->rear++;
                    isOkToPut = true;
                }
            } else if (queueHandle->rear + 1 < queueHandle->front) {
                queueHandle->rear++;
                isOkToPut = true;
            } else {
                processStatus = DDL_BASE_STATUS_ERROR;
                goto label_exitPoint;
            }

            if (isOkToPut) {
#if defined(LOG_LEVEL_VERBOSE)
                printf("%s : %d : copy to %lu from %lu, %d bytes\n", __func__, __LINE__,
                       (uint8_t*)(queueHandle->pElementBuffer + queueHandle->rear * queueHandle->elemSizeBytes),
                       (uint8_t*)pElement, queueHandle->elemSizeBytes);
#endif

                memcpy((uint8_t*)(queueHandle->pElementBuffer + queueHandle->rear * queueHandle->elemSizeBytes),
                       (uint8_t*)pElement, queueHandle->elemSizeBytes);
                if (queueHandle->front == -1) {
                    queueHandle->front = 0;
                } else {
                    /* code */
                }
            } else {
                /* code */
            }

            if (queueHandle->front == -1) {
                queueHandle->front = 0;
            }
        } else {
            processStatus = DDL_BASE_STATUS_BUSY;
        }
    } else {
        processStatus = DDL_BASE_STATUS_ERROR;
    }
label_exitPoint:
    queueHandle->isLocked = false;

#if defined(LOG_LEVEL_VERBOSE)
    printf("%s : %d : %s on exit front: %d\n", __func__, __LINE__, queueHandle->front);
    printf("%s : %d : %s on exit rear: %d\n", __func__, __LINE__, queueHandle->rear);
#endif

#if defined(LOG_LEVEL_VERBOSE)
    DDL_LOG("%s : %d : exit", __func__, __LINE__);
#endif
    return processStatus;
}

ddl_base_status_t ddl_queue_get(ddl_queue_handle_t queueHandle, void* pBuffer) {
#if defined(LOG_LEVEL_VERBOSE)
    DDL_LOG("%s : %d : entry");
#endif
    ddl_base_status_t processStatus = DDL_BASE_STATUS_OK;

#if defined(LOG_LEVEL_VERBOSE)
    printf("%s : %d : on entry front: %d\n", __func__, __LINE__, queueHandle->front);
    printf("%s : %d : on entry rear: %d\n", __func__, __LINE__, queueHandle->rear);
#endif

    if (queueHandle && pBuffer) {
        if (!queueHandle->isLocked) {
            queueHandle->isLocked = true;

            if (queueHandle->front == -1) {
#if defined(LOG_LEVEL_VERBOSE)
                printf("%s : %d : queue empty", __func__, __LINE__);
#endif
                processStatus = DDL_BASE_STATUS_ERROR;
                goto label_exitPoint;
            }

#if defined(LOG_LEVEL_VERBOSE)
            printf("%s : %d : copy to %lu from %lu, %d bytes\n", __func__, __LINE__, (uint8_t*)pBuffer,
                   (uint8_t*)(queueHandle->pElementBuffer + queueHandle->rear * queueHandle->elemSizeBytes),
                   queueHandle->elemSizeBytes);
#endif

            memcpy((uint8_t*)pBuffer,
                   (uint8_t*)(queueHandle->pElementBuffer + queueHandle->front * queueHandle->elemSizeBytes),
                   queueHandle->elemSizeBytes);
            memset((uint8_t*)(queueHandle->pElementBuffer + queueHandle->front * queueHandle->elemSizeBytes), 0,
                   queueHandle->elemSizeBytes);

            if (queueHandle->front == queueHandle->rear) {
                queueHandle->front = queueHandle->rear = -1;
            } else {
                if (queueHandle->front == queueHandle->elemSpace - 1) {
                    queueHandle->front = 0;
                } else {
                    (queueHandle->front)++;
                }
            }

        } else {
            processStatus = DDL_BASE_STATUS_BUSY;
        }
    } else {
        processStatus = DDL_BASE_STATUS_ERROR;
    }

label_exitPoint:
    queueHandle->isLocked = false;

#if defined(LOG_LEVEL_VERBOSE)
    printf("%s : %d : on exit front: %d\n", __func__, __LINE__, queueHandle->front);
    printf("%s : %d : on exit rear: %d\n", __func__, __LINE__, queueHandle->rear);
#endif

#if defined(LOG_LEVEL_VERBOSE)
    DDL_LOG("%s : %d : exit");
#endif
    return processStatus;
}

void ddl_queue_print(ddl_queue_handle_t queue) {
    for (size_t i = 0; i < queue->elemSpace; i++) {
        uint32_t* pValue = 0;
        pValue = (uint32_t*)(queue->pElementBuffer + i * queue->elemSizeBytes);
        printf("%s : %d : %d, ", *pValue);
    }
    printf("%s : %d : \n", __func__, __LINE__);
}
