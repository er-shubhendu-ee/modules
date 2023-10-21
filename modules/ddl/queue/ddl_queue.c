/**
 * @file      ddl_queue.c
 * @author:   Shubhfrontu B B
 * @date:     02/02/2023
 * @brief
 * @details
 *
 * @copyright
 *
 **/
#include <string.h>

#include "ddl_queue.h"


#define PROJ_VER "0.0.5"

 // #ifndef LOG_LEVEL_VERBOSE
 // #define LOG_LEVEL_VERBOSE
 // #endif

#define DDL_QUEUE_USE_STATIC 1

#if defined(DDL_QUEUE_USE_STATIC)
#define DDL_QUEUE_POOL_SIZE 10
#endif

#if defined(DDL_QUEUE_USE_STATIC)
#if defined(DDL_QUEUE_POOL_SIZE)
ddl_queue_struct_t ddl_queue_pool [ DDL_QUEUE_POOL_SIZE ];
#else
#error "DDL_QUEUE_POOL_SIZE not defined"
#endif
int nextAvailableSlotInPool = 0;
#endif

#if !defined(DDL_QUEUE_USE_STATIC)
ddl_queue_handle_t ddl_queue_create(uint32_t elementCount,
    uint32_t elementSizeBytes) {
    printf("%s : %d : using queue version: %s\n", __func__, __LINE__, PROJ_VER);
    ddl_queue_handle_t newQueue = NULL;
    if ( elementCount && elementSizeBytes ) {
        newQueue = malloc(sizeof(ddl_queue_struct_t));
        if ( newQueue ) {
            memset(newQueue, 0, sizeof(ddl_queue_struct_t));
            newQueue->pElementBuffer = malloc(elementSizeBytes * elementCount);
            if ( newQueue->pElementBuffer ) {
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

    return newQueue;
}
#endif

#if defined(DDL_QUEUE_USE_STATIC)
ddl_queue_handle_t ddl_queue_create_static(uint32_t elementSizeInBytes,
    uint32_t elementCount,
    uint8_t *pElementArray) {
    printf("%s : %d : using queue version: %s\n", __func__, __LINE__, PROJ_VER);
    ddl_queue_handle_t newQueue = NULL;
    if ( elementSizeInBytes && elementCount && pElementArray &&
        (DDL_QUEUE_POOL_SIZE > (nextAvailableSlotInPool + 1)) ) {
        newQueue = &ddl_queue_pool [ nextAvailableSlotInPool ];
        if ( newQueue ) {
            nextAvailableSlotInPool++;

            memset(newQueue, 0, sizeof(ddl_queue_struct_t));
            newQueue->pElementBuffer = pElementArray;

            memset(pElementArray, 0, (elementSizeInBytes * elementCount));
            newQueue->elemSpace = elementCount;
            newQueue->elemSizeBytes = elementSizeInBytes;
            newQueue->front = newQueue->rear = -1;

        } else {
            printf("%s : %d : error", __func__, __LINE__);
        }
    } else {
        printf("%s : %d : error", __func__, __LINE__);
    }

    return newQueue;
}
#endif

ddl_base_status_t ddl_queue_send(ddl_queue_handle_t queueHandle,
    void *pElement) {
    ddl_base_status_t processStatus = DDL_BASE_STATUS_OK;
    bool isOkToPut = false;

#if defined(LOG_LEVEL_VERBOSE)
    printf("%s : %d : on entry front: %d\n", __func__, __LINE__,
        queueHandle->front);
    printf("%s : %d : on entry rear: %d\n", __func__, __LINE__,
        queueHandle->rear);
#endif

    if ( queueHandle && pElement ) {
        if ( !queueHandle->isLocked ) {
            queueHandle->isLocked = true;
            if ( queueHandle->rear >= queueHandle->front ) {
                if ( queueHandle->rear == queueHandle->elemSpace - 1 ) {
                    if ( queueHandle->front > 0 ) {
                        queueHandle->rear = 0;
                        processStatus = DDL_BASE_STATUS_OK;
                        isOkToPut = true;
                    } else {
                        processStatus = DDL_BASE_STATUS_ERROR;
                        goto label_exitPoint;
                    }
                } else {
                    queueHandle->rear++;
                    processStatus = DDL_BASE_STATUS_OK;
                    isOkToPut = true;
                }
            } else if ( queueHandle->rear + 1 < queueHandle->front ) {
                queueHandle->rear++;
                processStatus = DDL_BASE_STATUS_OK;
                isOkToPut = true;
            } else {
                processStatus = DDL_BASE_STATUS_ERROR;
                goto label_exitPoint;
            }

            if ( isOkToPut ) {
#if defined(LOG_LEVEL_VERBOSE)
                printf("%s : %d : copy to %lu from %lu, %d bytes\n", __func__, __LINE__,
                    (uint8_t *) (queueHandle->pElementBuffer +
                        queueHandle->rear * queueHandle->elemSizeBytes),
                    (uint8_t *) pElement, queueHandle->elemSizeBytes);
#endif

                memcpy((uint8_t *) (queueHandle->pElementBuffer +
                    queueHandle->rear * queueHandle->elemSizeBytes),
                    (uint8_t *) pElement, queueHandle->elemSizeBytes);
                if ( queueHandle->front == -1 ) {
                    queueHandle->front = 0;
                } else {
                    /* code */
                }
            } else {
                processStatus = DDL_BASE_STATUS_ERROR;
                goto label_exitPoint;
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
    printf("%s : %d : on exit front: %d\n", __func__, __LINE__,
        queueHandle->front);
    printf("%s : %d : on exit rear: %d\n", __func__, __LINE__, queueHandle->rear);
#endif

    return processStatus;
}

ddl_base_status_t ddl_queue_recv(ddl_queue_handle_t queueHandle,
    void *pBuffer) {
    ddl_base_status_t processStatus = DDL_BASE_STATUS_OK;

#if defined(LOG_LEVEL_VERBOSE)
    printf("%s : %d : on entry front: %d\n", __func__, __LINE__,
        queueHandle->front);
    printf("%s : %d : on entry rear: %d\n", __func__, __LINE__,
        queueHandle->rear);
#endif

    if ( queueHandle && pBuffer ) {
        if ( !queueHandle->isLocked ) {
            queueHandle->isLocked = true;

            if ( queueHandle->front == -1 ) {
#if defined(LOG_LEVEL_VERBOSE)
                printf("%s : %d : queue empty", __func__, __LINE__);
#endif
                processStatus = DDL_BASE_STATUS_ERROR;
                goto label_exitPoint;
            }

#if defined(LOG_LEVEL_VERBOSE)
            printf("%s : %d : copy to %lu from %lu, %d bytes\n", __func__, __LINE__,
                (uint8_t *) pBuffer,
                (uint8_t *) (queueHandle->pElementBuffer +
                    queueHandle->rear * queueHandle->elemSizeBytes),
                queueHandle->elemSizeBytes);
#endif

            memcpy((uint8_t *) pBuffer,
                (uint8_t *) (queueHandle->pElementBuffer +
                    queueHandle->front * queueHandle->elemSizeBytes),
                queueHandle->elemSizeBytes);
            memset((uint8_t *) (queueHandle->pElementBuffer +
                queueHandle->front * queueHandle->elemSizeBytes),
                0, queueHandle->elemSizeBytes);

            if ( queueHandle->front == queueHandle->rear ) {
                queueHandle->front = queueHandle->rear = -1;
            } else {
                if ( queueHandle->front == queueHandle->elemSpace - 1 ) {
                    queueHandle->front = 0;
                } else {
                    (queueHandle->front)++;
                }
            }
            processStatus = DDL_BASE_STATUS_OK;

        } else {
            processStatus = DDL_BASE_STATUS_BUSY;
        }
    } else {
        processStatus = DDL_BASE_STATUS_ERROR;
    }

label_exitPoint:
    queueHandle->isLocked = false;

#if defined(LOG_LEVEL_VERBOSE)
    printf("%s : %d : on exit front: %d\n", __func__, __LINE__,
        queueHandle->front);
    printf("%s : %d : on exit rear: %d\n", __func__, __LINE__, queueHandle->rear);
#endif

    return processStatus;
}

ddl_base_status_t ddl_queue_delete(ddl_queue_handle_t queue) {
    ddl_base_status_t processStatus = DDL_BASE_STATUS_OK;

    if ( queue ) {
#if !defined(DDL_QUEUE_USE_STATIC)
        free(queue);
#else
        if ( (&ddl_queue_pool [ 0 ] > queue) ||
            (&ddl_queue_pool [ DDL_QUEUE_POOL_SIZE - 1 ] < queue) ) {
            processStatus = DDL_BASE_STATUS_ERROR;
            goto label_exitPoint;
        }

        int i = 0;
        while ( (size_t) ((ddl_queue_struct_t *) ddl_queue_pool + i) != (size_t) queue ) {
            i++;
        }

        memset((uint8_t *) &ddl_queue_pool [ i ], 0, sizeof(ddl_queue_struct_t));

#endif
    } else {
        processStatus = DDL_BASE_STATUS_ERROR;
    }

label_exitPoint:
    return processStatus;
}

void ddl_queue_print(ddl_queue_handle_t queue) {
    for ( size_t i = 0; i < queue->elemSpace; i++ ) {
        uint32_t *pValue = 0;
        pValue = (uint32_t *) ((uint8_t *) queue->pElementBuffer +
            (i * queue->elemSizeBytes));
        printf("%s : %d : queue[%d]= %d, \r\n", __func__, __LINE__, i, *pValue);
    }
}
