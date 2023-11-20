/**
 * @file: ddl_filter.c
 * @author:   Shubhendu B B
 * @date:     12/09/2023
 * @brief: digital filters
 * @details
 *
 * @copyright
 *
 **/


#include <stdlib.h>
#include <string.h>

#include "ddl_filter.h"


#define DATA_BUFF_TEMP_SIZE 100
static int dataBuffTemp [ DATA_BUFF_TEMP_SIZE ][ 2 ] = { {0}, {0} };

static int filter_median(int *pInputArray, int inputArraySize);
static int filter_mode(int *pInputArray, int inputArraySize);

static int filter_median(int *pInputArray, int inputArraySize) {
    if ( !pInputArray || !inputArraySize ) {
        return 0;
    }
    return (int) *((int *) pInputArray + (inputArraySize / 2));
}

static int filter_mode(int *pInputArray, int inputArraySize) {
    if ( !pInputArray || !inputArraySize ||
        (DATA_BUFF_TEMP_SIZE < inputArraySize) ) {
        return 0;
    }

    //   printf("\nInput Array to mode filter:\n");
    //   for (size_t i = 0; i < inputArraySize; i++) {
    //     printf("filterInputDataArray[%d]= %4.2d\n", i,
    //            (int)*((int *)pInputArray + i));
    //   }

    memset(dataBuffTemp, 0, sizeof(dataBuffTemp));

    int tempIndexUsed = 0;

    for ( size_t inputArrayIndexProcessed = 0;
        inputArrayIndexProcessed < inputArraySize;) {
        // printf("entry inputArrayIndexProcessed: %d\r\n",
        // inputArrayIndexProcessed);

        dataBuffTemp [ tempIndexUsed ][ 0 ] = inputArrayIndexProcessed;
        int element = (int) *((int *) pInputArray + inputArrayIndexProcessed);
        int elementWeight = 0;

        while ( element == (int) *((int *) pInputArray + inputArrayIndexProcessed) ) {
            inputArrayIndexProcessed++;
            elementWeight++;
        }

        dataBuffTemp [ tempIndexUsed ][ 1 ] = elementWeight;
        tempIndexUsed++;

        // printf("Element: %d, Weight: %d at index: %d\r\n", element,
        // elementWeight,
        //        dataBuffTemp[tempIndexUsed - 1][0]);
        // printf("exit inputArrayIndexProcessed: %d\r\n",
        // inputArrayIndexProcessed);
    }

    //   for (size_t i = 0; i < tempIndexUsed; i++) {
    //     printf("dataBuffTemp[%d]\r\n", i);
    //     for (size_t j = 0; j < 2; j++) {
    //       printf("[%d]: %d\r\n", j, dataBuffTemp[i][j]);
    //     }
    //   }

    int maxAtIndexTempBuff = 0;
    int resultIndex = dataBuffTemp [ 0 ][ 0 ];
    for ( size_t scanIndexTemp = 0; scanIndexTemp < tempIndexUsed - 1;
        scanIndexTemp++ ) {
        // printf("comparing [%d][1]:%d with [%d][1]:%d\r\n", maxAtIndexTempBuff,
        //        dataBuffTemp[maxAtIndexTempBuff][1], scanIndexTemp + 1,
        //        dataBuffTemp[scanIndexTemp + 1][1]);

        if ( dataBuffTemp [ maxAtIndexTempBuff ][ 1 ] <
            dataBuffTemp [ scanIndexTemp + 1 ][ 1 ] ) {
            maxAtIndexTempBuff = scanIndexTemp + 1;
            resultIndex = dataBuffTemp [ maxAtIndexTempBuff ][ 0 ];
        }
        // printf("result index: %d\r\n", resultIndex);
    }

    return (int) *((int *) pInputArray + resultIndex);
}

int ddl_filter_median(int *pInputArray, int inputArraySize) {
    if ( !pInputArray || !inputArraySize ) {
        return 0;
    }

    // sort the i/p
    ddl_util_timSort(pInputArray, inputArraySize);

    return filter_median(pInputArray, inputArraySize);
}

int ddl_filter_mode(int *pInputArray, int inputArraySize) {
    if ( !pInputArray || !inputArraySize ) {
        return 0;
    }

    // sort the i/p
    ddl_util_timSort(pInputArray, inputArraySize);

    return filter_mode(pInputArray, inputArraySize);
}

int ddl_filter_memo_avg(int *pInputArray, int inputArraySize) {
    int filteredValue_median = 0;
    int filteredValue_mode = 0;

    if ( !pInputArray || !inputArraySize ) {
        return 0;
    }

    // sort the i/p
    ddl_util_timSort(pInputArray, inputArraySize);

    filteredValue_median = filter_median(pInputArray, inputArraySize);

    filteredValue_mode = filter_mode(pInputArray, inputArraySize);

    // printf("mod-median difference: %4.2d\n",
    //     (filteredValue_median > filteredValue_mode)
    //     ? filteredValue_median - filteredValue_mode
    //     : filteredValue_mode - filteredValue_median);

    return ((filteredValue_median + filteredValue_mode) / 2);
}