/**
 * @file      main.c
 * @author:   Shubhendu B B
 * @date:     04/02/2023
 * @brief
 * @details
 *
 * @copyright
 *
 **/
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "ddl_queue.h"

ddl_queue_handle_t queue;

int main(void) {
  uint32_t value = 0;
  queue = ddl_queue_queue_create(5, sizeof(uint32_t));

  value = 1;
  ddl_queue_put(queue, &value);
  ddl_queue_print(queue);

  value = 2;
  ddl_queue_put(queue, &value);
  ddl_queue_print(queue);

  ddl_queue_get(queue, &value);
  printf("%s : %d : received value from queue: %d\r\n", __func__, __LINE__,
         value);
  ddl_queue_print(queue);

  value = 3;
  ddl_queue_put(queue, &value);
  ddl_queue_print(queue);

  value = 4;
  ddl_queue_put(queue, &value);
  ddl_queue_print(queue);

  value = 5;
  ddl_queue_put(queue, &value);
  ddl_queue_print(queue);

  ddl_queue_get(queue, &value);
  printf("%s : %d : received value from queue: %d\r\n", __func__, __LINE__,
         value);
  ddl_queue_print(queue);

  ddl_queue_get(queue, &value);
  printf("%s : %d : received value from queue: %d\r\n", __func__, __LINE__,
         value);
  ddl_queue_print(queue);

  ddl_queue_get(queue, &value);
  printf("%s : %d : received value from queue: %d\r\n", __func__, __LINE__,
         value);
  ddl_queue_print(queue);

  value = 6;
  ddl_queue_put(queue, &value);
  ddl_queue_print(queue);

  value = 7;
  ddl_queue_put(queue, &value);
  ddl_queue_print(queue);

  value = 8;
  ddl_queue_put(queue, &value);
  ddl_queue_print(queue);

  value = 9;
  ddl_queue_put(queue, &value);
  ddl_queue_print(queue);

  value = 10;
  ddl_queue_put(queue, &value);
  ddl_queue_print(queue);

  ddl_queue_get(queue, &value);
  printf("%s : %d : received value from queue: %d\r\n", __func__, __LINE__,
         value);
  ddl_queue_print(queue);

  value = 11;
  ddl_queue_put(queue, &value);
  ddl_queue_print(queue);

  ddl_queue_get(queue, &value);
  printf("%s : %d : received value from queue: %d\r\n", __func__, __LINE__,
         value);
  ddl_queue_print(queue);

  ddl_queue_get(queue, &value);
  printf("%s : %d : received value from queue: %d\r\n", __func__, __LINE__,
         value);
  ddl_queue_print(queue);

  ddl_queue_get(queue, &value);
  printf("%s : %d : received value from queue: %d\r\n", __func__, __LINE__,
         value);
  ddl_queue_print(queue);

  ddl_queue_get(queue, &value);
  printf("%s : %d : received value from queue: %d\r\n", __func__, __LINE__,
         value);
  ddl_queue_print(queue);

  ddl_queue_delete(queue);

  return 0;
}