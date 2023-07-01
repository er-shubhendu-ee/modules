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

typedef struct {
  uint8_t byte_1;
  uint16_t word_1;
  uint32_t dword_1;
} QueueElement_t;

#define QUEUE_ELEMENT_COUNT 5
QueueElement_t queueElementArray[QUEUE_ELEMENT_COUNT] = {0};

ddl_queue_handle_t queue, queue_2;

int main(void) {
  QueueElement_t queueElement = {0};
  queue =
      ddl_queue_queue_create_static(sizeof(QueueElement_t), QUEUE_ELEMENT_COUNT,
                                    (uint8_t *)&queueElementArray[0]);

  queue_2 =
      ddl_queue_queue_create_static(sizeof(QueueElement_t), QUEUE_ELEMENT_COUNT,
                                    (uint8_t *)&queueElementArray[0]);
  queueElement.byte_1 = 1;
  queueElement.word_1 = 11;
  queueElement.dword_1 = 123456789;
  if (ddl_queue_send(queue, &queueElement)) {
    printf("%s : %d : Error: Failed to send to queue.\r\n", __func__, __LINE__);
  }

  queueElement.byte_1 = 2;
  queueElement.word_1 = 22;
  queueElement.dword_1 = 123456789;
  if (ddl_queue_send(queue, &queueElement)) {
    printf("%s : %d : Error: Failed to send to queue.\r\n", __func__, __LINE__);
  }

  memset(&queueElement, 0, sizeof(queueElement));
  if (ddl_queue_recv(queue, &queueElement)) {
    printf("%s : %d : : Error: Failed to receive from queue.", __func__,
           __LINE__);
  }
  printf(
      "%s : %d : received value queueElement.byte_1: %d, queueElement.word_1: "
      "%d "
      "queueElement.dword_1: %d\r\n",
      __func__, __LINE__, queueElement.byte_1, queueElement.word_1,
      queueElement.dword_1);

  queueElement.byte_1 = 3;
  queueElement.word_1 = 33;
  queueElement.dword_1 = 123456789;
  if (ddl_queue_send(queue, &queueElement)) {
    printf("%s : %d : Error: Failed to send to queue.\r\n", __func__, __LINE__);
  }

  queueElement.byte_1 = 4;
  queueElement.word_1 = 44;
  queueElement.dword_1 = 123456789;
  if (ddl_queue_send(queue, &queueElement)) {
    printf("%s : %d : Error: Failed to send to queue.\r\n", __func__, __LINE__);
  }

  queueElement.byte_1 = 5;
  queueElement.word_1 = 55;
  queueElement.dword_1 = 123456789;
  if (ddl_queue_send(queue, &queueElement)) {
    printf("%s : %d : Error: Failed to send to queue.\r\n", __func__, __LINE__);
  }

  memset(&queueElement, 0, sizeof(queueElement));
  if (ddl_queue_recv(queue, &queueElement)) {
    printf("%s : %d : : Error: Failed to receive from queue.", __func__,
           __LINE__);
  }
  printf(
      "%s : %d : received value queueElement.byte_1: %d, queueElement.word_1: "
      "%d "
      "queueElement.dword_1: %d\r\n",
      __func__, __LINE__, queueElement.byte_1, queueElement.word_1,
      queueElement.dword_1);

  memset(&queueElement, 0, sizeof(queueElement));
  if (ddl_queue_recv(queue, &queueElement)) {
    printf("%s : %d : : Error: Failed to receive from queue.", __func__,
           __LINE__);
  }
  printf(
      "%s : %d : received value queueElement.byte_1: %d, queueElement.word_1: "
      "%d "
      "queueElement.dword_1: %d\r\n",
      __func__, __LINE__, queueElement.byte_1, queueElement.word_1,
      queueElement.dword_1);

  memset(&queueElement, 0, sizeof(queueElement));
  if (ddl_queue_recv(queue, &queueElement)) {
    printf("%s : %d : : Error: Failed to receive from queue.", __func__,
           __LINE__);
  }
  printf(
      "%s : %d : received value queueElement.byte_1: %d, queueElement.word_1: "
      "%d "
      "queueElement.dword_1: %d\r\n",
      __func__, __LINE__, queueElement.byte_1, queueElement.word_1,
      queueElement.dword_1);

  queueElement.byte_1 = 6;
  queueElement.word_1 = 66;
  queueElement.dword_1 = 123456789;
  if (ddl_queue_send(queue, &queueElement)) {
    printf("%s : %d : Error: Failed to send to queue.\r\n", __func__, __LINE__);
  }

  queueElement.byte_1 = 7;
  queueElement.word_1 = 77;
  queueElement.dword_1 = 123456789;
  if (ddl_queue_send(queue, &queueElement)) {
    printf("%s : %d : Error: Failed to send to queue.\r\n", __func__, __LINE__);
  }

  queueElement.byte_1 = 8;
  queueElement.word_1 = 88;
  queueElement.dword_1 = 123456789;
  if (ddl_queue_send(queue, &queueElement)) {
    printf("%s : %d : Error: Failed to send to queue.\r\n", __func__, __LINE__);
  }

  queueElement.byte_1 = 9;
  queueElement.word_1 = 99;
  queueElement.dword_1 = 123456789;
  if (ddl_queue_send(queue, &queueElement)) {
    printf("%s : %d : Error: Failed to send to queue.\r\n", __func__, __LINE__);
  }

  queueElement.byte_1 = 10;
  queueElement.word_1 = 1010;
  queueElement.dword_1 = 123456789;
  if (ddl_queue_send(queue, &queueElement)) {
    printf("%s : %d : Error: Failed to send to queue.\r\n", __func__, __LINE__);
  }

  memset(&queueElement, 0, sizeof(queueElement));
  if (ddl_queue_recv(queue, &queueElement)) {
    printf("%s : %d : : Error: Failed to receive from queue.", __func__,
           __LINE__);
  }
  printf(
      "%s : %d : received value queueElement.byte_1: %d, queueElement.word_1: "
      "%d "
      "queueElement.dword_1: %d\r\n",
      __func__, __LINE__, queueElement.byte_1, queueElement.word_1,
      queueElement.dword_1);

  queueElement.byte_1 = 11;
  queueElement.word_1 = 1111;
  queueElement.dword_1 = 123456789;
  if (ddl_queue_send(queue, &queueElement)) {
    printf("%s : %d : Error: Failed to send to queue.\r\n", __func__, __LINE__);
  }

  memset(&queueElement, 0, sizeof(queueElement));
  if (ddl_queue_recv(queue, &queueElement)) {
    printf("%s : %d : : Error: Failed to receive from queue.", __func__,
           __LINE__);
  }
  printf(
      "%s : %d : received value queueElement.byte_1: %d, queueElement.word_1: "
      "%d "
      "queueElement.dword_1: %d\r\n",
      __func__, __LINE__, queueElement.byte_1, queueElement.word_1,
      queueElement.dword_1);

  memset(&queueElement, 0, sizeof(queueElement));
  if (ddl_queue_recv(queue, &queueElement)) {
    printf("%s : %d : : Error: Failed to receive from queue.", __func__,
           __LINE__);
  }
  printf(
      "%s : %d : received value queueElement.byte_1: %d, queueElement.word_1: "
      "%d "
      "queueElement.dword_1: %d\r\n",
      __func__, __LINE__, queueElement.byte_1, queueElement.word_1,
      queueElement.dword_1);

  memset(&queueElement, 0, sizeof(queueElement));
  if (ddl_queue_recv(queue, &queueElement)) {
    printf("%s : %d : : Error: Failed to receive from queue.", __func__,
           __LINE__);
  }
  printf(
      "%s : %d : received value queueElement.byte_1: %d, queueElement.word_1: "
      "%d "
      "queueElement.dword_1: %d\r\n",
      __func__, __LINE__, queueElement.byte_1, queueElement.word_1,
      queueElement.dword_1);

  memset(&queueElement, 0, sizeof(queueElement));
  if (ddl_queue_recv(queue, &queueElement)) {
    printf("%s : %d : : Error: Failed to receive from queue.", __func__,
           __LINE__);
  }
  printf(
      "%s : %d : received value queueElement.byte_1: %d, queueElement.word_1: "
      "%d "
      "queueElement.dword_1: %d\r\n",
      __func__, __LINE__, queueElement.byte_1, queueElement.word_1,
      queueElement.dword_1);

  memset(&queueElement, 0, sizeof(queueElement));
  if (ddl_queue_recv(queue, &queueElement)) {
    printf("%s : %d : : Error: Failed to receive from queue.", __func__,
           __LINE__);
  }

  printf(
      "%s : %d : received value queueElement.byte_1: %d, queueElement.word_1: "
      "%d "
      "queueElement.dword_1: %d\r\n",
      __func__, __LINE__, queueElement.byte_1, queueElement.word_1,
      queueElement.dword_1);

  ddl_queue_delete(queue);

  return 0;
}