/**
 * @file      main.c
 * @author:   Shubhendu B B
 * @date:     18/05/2023
 * @brief
 * @details
 *
 * @copyright
 *
 **/

#include "app_queue.h"

int main(void) {
    printf("%s : %d : started!\r\n", __func__, __LINE__);

    logStorageStruct_t logStorageStructArrayTemp[LOG_STORAGE_COUNT_MAX] = {0};
    storageQueueHandle_t logStorageQueueHandle = NULL;
    logStorageQueueHandle = app_helper_StorageQueueInit();

    if (logStorageQueueHandle == NULL) {
        printf("%s : %d : Error creating queue.\r\n", __func__, __LINE__);
        return -1;
    }

    app_helper_QueuePut(1, 10, 1);
    app_helper_QueuePut(2, 10, 2);
    app_helper_QueuePut(3, 10, 3);
    app_helper_QueuePut(4, 10, 4);
    app_helper_QueuePut(5, 10, 5);

    app_helper_QueueGet(logStorageStructArrayTemp, 1);

    app_helper_QueuePut(6, 10, 6);

    app_helper_QueueGet(logStorageStructArrayTemp, 1);

    app_helper_QueuePut(7, 10, 7);

    app_helper_QueueGet(logStorageStructArrayTemp, 1);

    app_helper_QueuePut(8, 10, 8);

    app_helper_QueueGet(logStorageStructArrayTemp, 1);

    app_helper_QueuePut(9, 10, 9);
    app_helper_QueuePut(10, 10, 10);

    app_helper_PrintStorageQueue();

    printf("%s : %d : %d logs read success.\r\n", __func__, __LINE__,
           app_helper_QueueGet(logStorageStructArrayTemp, 1));

    app_helper_PrintStorageQueue();

    printf("%s : %d : %d logs read success.\r\n", __func__, __LINE__,
           app_helper_QueueGet(logStorageStructArrayTemp, 3));

    app_helper_PrintStorageQueue();

    return 0;
}