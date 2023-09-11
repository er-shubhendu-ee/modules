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
#include "search-sorts.h"

#define SAMPLE_VALUE_LOWEST 100
#define SAMPLE_VALUE_HIGHEST 150

#define SAMPLE_DATA_ARRAY_SIZE 10

static int fill_array_random(int* pIntArray, int intArraySize, int lowerBound,
                             int upperBound);

int filterInputDataArray[SAMPLE_DATA_ARRAY_SIZE] = {0};
int filterSampleWeight[SAMPLE_DATA_ARRAY_SIZE][SAMPLE_DATA_ARRAY_SIZE] = {0};
int filteredValue_median = 0;
int filteredValue_mode = 0;

void main(int argc, char* argv[]) {
  for (int iterSize = 0; iterSize < 10; iterSize++) {
    fill_array_random(filterInputDataArray, SAMPLE_DATA_ARRAY_SIZE,
                      SAMPLE_VALUE_LOWEST + iterSize,
                      SAMPLE_VALUE_HIGHEST + iterSize);
    // Function Call
    timSort(filterInputDataArray, SAMPLE_DATA_ARRAY_SIZE);

    // printf("\nSorted Array:\n");
    // for (size_t i = 0; i < SAMPLE_DATA_ARRAY_SIZE; i++) {
    //   printf("filterInputDataArray[%d]= %4.2d\n", i,
    //   filterInputDataArray[i]);
    // }

    /* Take the middle value (median filter)*/
    filteredValue_median =
        digital_filter_median(filterInputDataArray, SAMPLE_DATA_ARRAY_SIZE);

    /* Take the middle value (mode filter)*/
    filteredValue_mode =
        digital_filter_mode(filterInputDataArray, SAMPLE_DATA_ARRAY_SIZE);

    printf("\nfilteredValue(median filter): %4.2d\n", filteredValue_median);
    printf("filteredValue(mode filter): %4.2d\n", filteredValue_mode);
    printf("mod-median difference: %4.2d\n",
           (filteredValue_median > filteredValue_mode)
               ? filteredValue_median - filteredValue_mode
               : filteredValue_mode - filteredValue_median);
    printf("filteredValue(mode-median avg filter): %4.2d\n",
           (filteredValue_median + filteredValue_mode) / 2);
  }
}

static int fill_array_random(int* pIntArray, int intArraySize, int lowerBound,
                             int upperBound) {
  if (!pIntArray || !intArraySize) {
    return -1;
  }

  for (size_t i = 0; i < intArraySize; i++) {
    filterInputDataArray[i] =
        (rand() % (upperBound - lowerBound + 1)) + lowerBound;
  }

  //   printf("\nSample array:\r\n");
  //   for (size_t i = 0; i < intArraySize; i++) {
  //     printf("filterInputDataArray[%d]= %4.2d\n", i,
  //     filterInputDataArray[i]);
  //   }
}