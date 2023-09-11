/**
 * @file: main.c
 * @author:   Shubhendu B B
 * @date:     12/09/2023
 * @brief
 * @details
 *
 * @copyright
 *
 **/

#include <stdlib.h>

#include "digital-filters.h"

#define SAMPLE_VALUE_LOWEST 100
#define SAMPLE_VALUE_HIGHEST 110

#define SAMPLE_DATA_ARRAY_SIZE 10

static int fill_array_random(int* pIntArray, int intArraySize);

int filterInputDataArray[SAMPLE_DATA_ARRAY_SIZE] = {0};

void main(int argc, char* argv[]) {
  fill_array_random(filterInputDataArray, SAMPLE_DATA_ARRAY_SIZE);
}

static int fill_array_random(int* pIntArray, int intArraySize) {
  if (!pIntArray || !intArraySize) {
    return -1;
  }

  for (size_t i = 0; i < SAMPLE_DATA_ARRAY_SIZE; i++) {
    filterInputDataArray[i] =
        (rand() % (SAMPLE_VALUE_HIGHEST - SAMPLE_VALUE_LOWEST + 1)) +
        SAMPLE_VALUE_LOWEST;
  }

  printf("\nSample array:\r\n");
  for (size_t i = 0; i < SAMPLE_DATA_ARRAY_SIZE; i++) {
    printf("filterInputDataArray[%d]= %4.2d\n", i, filterInputDataArray[i]);
  }

  // Function Call
  timSort(filterInputDataArray, SAMPLE_DATA_ARRAY_SIZE);

  printf("\nSorted Array:\n");
  for (size_t i = 0; i < SAMPLE_DATA_ARRAY_SIZE; i++) {
    printf("filterInputDataArray[%d]= %4.2d\n", i, filterInputDataArray[i]);
  }
}