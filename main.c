/**
 * @file      main.c
 * @author:   Shubhendu B B
 * @date:     12/09/2023
 * @brief
 * @details
 *
 * @copyright
 *
 **/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ddl_util.h"

#define SAMPLE_COUNT 11

float dataSet [][ 2 ] = { {-100,0.27},
                        {-96,0.34},
                        {-36,1.68},
                        {-20,2.08},
                        {-4,2.41},
                        {0,2.49},
                        {4,2.57},
                        {20,2.89},
                        {36,3.29},
                        {96,4.6},
                        {100,4.65} };

static int get_sample_count(float *pDataSet);
static int print_dataset(float *pDataSet, int sampleCount);
static int extract_x(float *pDataSet, float *pBuff_x, int sampleCount);
static int extract_y(float *pDataSet, float *pBuff_y, int sampleCount);


void main(int argc, char *argv []) {
    float buff_x [ SAMPLE_COUNT ] = { -1.0,-2.0,-3.0,-4.0,-5.0,(float) 0 ,1.0,2.0,3.0,4.0,5.0,(float) 0 };
    float buff_y [ SAMPLE_COUNT ] = { -1.0,-2.0,-3.0,-4.0,-5.0,(float) 0 ,1.0,2.0,3.0,4.0,5.0,(float) 0 };
    int sampleCount = get_sample_count(dataSet);

    print_dataset(dataSet, sampleCount);
    extract_x(dataSet, buff_x, sampleCount);
    extract_y(dataSet, buff_y, sampleCount);
    ddl_util_plot_function_2d("title", "x label", "y label", buff_x, buff_y, SAMPLE_COUNT);
}


static int get_sample_count(float *pDataSet) {
    if ( !pDataSet ) {
        return 0;
    }

    int64_t startAddress = (int64_t) dataSet;
    int64_t endAddress = (int64_t) * (&dataSet + 1);
    int64_t sampleCount = (endAddress - startAddress) / (sizeof(float) * 2);

    return sampleCount;
}



static int print_dataset(float *pDataSet, int sampleCount) {
    if ( !pDataSet ) {
        return 0;
    }

    int indexI = 0;
    int indexJ = 0;

    while ( indexI < sampleCount ) {
        for ( indexJ = 0; indexJ < 2; indexJ++ ) {
            printf("%3.2f,", *(pDataSet + (indexI * 2) + indexJ));
        }
        printf("\n");
        indexI++;
    }
}


static int extract_x(float *pDataSet, float *pBuff_x, int sampleCount) {
    if ( !pDataSet ) {
        return 0;
    }

    int indexI = 0;
    int indexJ = 0;

    while ( indexI < sampleCount ) {
        *((float *) pBuff_x + indexI) = *((float *) pDataSet + (indexI * 2));
        printf("%3.2f,", *((float *) pBuff_x + indexI));
        printf("\n");
        indexI++;
    }
}

static int extract_y(float *pDataSet, float *pBuff_y, int sampleCount) {
    if ( !pDataSet ) {
        return 0;
    }

    int indexI = 0;
    int indexJ = 0;

    while ( indexI < sampleCount ) {
        *((float *) pBuff_y + indexI) = *((float *) pDataSet + (indexI * 2) + 1);
        printf("%3.2f,", *((float *) pBuff_y + indexI));
        printf("\n");
        indexI++;
    }
}
