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

#include "app.h"
#include "ddl_queue.h"

app_logElementStruct_t logStorageStructArrayTemp [ LOG_STORAGE_COUNT_MAX ] = { 0 };
ddl_queue_handle_t logStorageQueueHandle = NULL;

int main(void) {
    app_logElementStruct_t queueElement = { 0 };

    printf("%s : %d : started!\r\n", __func__, __LINE__);


    logStorageQueueHandle = app_queue_init();

    if ( logStorageQueueHandle == NULL ) {
        printf("%s : %d : Error creating queue.\r\n", __func__, __LINE__);
        return -1;
    }

    app_queue_put(1, 2, 3);
    app_queue_put(4, 5, 6);
    app_queue_put(7, 8, 9);
    app_queue_put(10, 11, 12);
    app_queue_put(13, 14, 15);
    app_queue_put(16, 17, 18);

    app_queue_get(logStorageStructArrayTemp, 1);

    app_queue_put(19, 20, 21);

    app_queue_get(logStorageStructArrayTemp, 1);

    app_queue_put(22, 23, 24);

    app_queue_get(logStorageStructArrayTemp, 1);

    app_queue_put(25, 26, 27);

    app_queue_get(logStorageStructArrayTemp, 1);

    app_queue_put(28, 29, 30);
    app_queue_put(31, 32, 33);

    printf("%s : %d : %d logs read success.\r\n", __func__, __LINE__,
        app_queue_get(logStorageStructArrayTemp, 1));

    printf("%s : %d : %d logs read success.\r\n", __func__, __LINE__,
        app_queue_get(logStorageStructArrayTemp, 3));

    memset(&queueElement, 0, sizeof(queueElement));
    if ( ddl_queue_recv(logStorageQueueHandle, &queueElement) ) {
        printf("%s : %d : : Error: Failed to receive from queue.", __func__,
            __LINE__);
    }

    // printf(
    //     "%s : %d : received value queueElement.byte_1: %d, queueElement.word_1: "
    //     "%d "
    //     "queueElement.dword_1: %d\r\n",
    //     __func__, __LINE__, queueElement.byte_1, queueElement.word_1,
    //     queueElement.dword_1);

    ddl_queue_delete(logStorageQueueHandle);

    return 0;
}