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
#define BUFFER_SIZE 100

float dataSet [][ 2 ] = { {0.27,-100},
                        {0.34,-96},
                        {1.68,-36},
                        {2.08,-20},
                        {2.41,-4},
                        {2.49,0},
                        {2.57,4},
                        {2.89,20},
                        {3.29,36},
                        {4.6,96},
                        {4.65,100} };

static int get_sample_count(float *pDataSet);
static int print_dataset(float *pDataSet, int sampleCount);
static int extract_x(float *pDataSet, float *pBuff_x, int sampleCount);
static int extract_y(float *pDataSet, float *pBuff_y, int sampleCount);
static float predict_y_value(float *pDataset, int sampleCount, float inputValue);

#define SAMPLE_INPUT_VALUE 4.6

void main(int argc, char *argv []) {
    int indexI = 0;
    float inputValue = (float) 0;
    float buff_x [ BUFFER_SIZE ] = { (float) 0 };
    float buff_y [ BUFFER_SIZE ] = { (float) 0 };

    indexI = 0;
    while ( (float) inputValue < (float) 5 ) {
        buff_x [ indexI ] = inputValue;
        buff_y [ indexI ] = predict_y_value((float *) dataSet, SAMPLE_COUNT, (float) buff_x [ indexI ]);

        printf("\n%3.2f,%3.2f", buff_x [ indexI ], buff_y [ indexI ]);

        indexI++;
        inputValue += 0.05;
    }

    ddl_util_plot_function_2d("title", "x label", "y label", buff_x, buff_y, indexI);
}

static float predict_y_value(float *pDataset, int sampleCount, float inputValue) {
    // declare locals
    int i = 0;
    float PVSpan = (float) 0;
    float InputSpan = (float) 0;
    float scaledValue = (float) 0;
    float buff_x [ SAMPLE_COUNT ] = { (float) 0 };
    float buff_y [ SAMPLE_COUNT ] = { (float) 0 };
    float valueMin = (float) 0;
    float valueMax = (float) 0;

#if (LOG_LEVEL>0)
    print_dataset((float *) pDataset, sampleCount);
#endif

    extract_x((float *) pDataset, buff_x, sampleCount);
    extract_y((float *) pDataset, buff_y, sampleCount);


    valueMin = ddl_util_find_min((float *) buff_x, SAMPLE_COUNT);
    valueMax = ddl_util_find_max((float *) buff_x, SAMPLE_COUNT);

    if ( (float) inputValue < valueMin ) {
        scaledValue = ddl_util_find_min((float *) buff_y, SAMPLE_COUNT);
        goto label_exitPoint;
    }

    if ( (float) inputValue > valueMax ) {
        scaledValue = ddl_util_find_max((float *) buff_y, SAMPLE_COUNT);
        goto label_exitPoint;
    }

    // loop until we get to the point in the arrays where the actual input value is
    while ( (inputValue > *((float *) buff_x + i)) && (*((float *) buff_x + i) != 0) ) i++;

    // if it is right on a point, return the display value with the offset
    if ( inputValue == *((float *) buff_x + i) ) {
        scaledValue = *((float *) buff_y + i);
        goto label_exitPoint;
    }

    // if it is between points of +ve range, calculate the display value
    PVSpan = *((float *) buff_y + i) - *((float *) buff_y + (i - 1));

    // calculate input change in this segment
    InputSpan = *((float *) buff_x + i) - *((float *) buff_x + (i - 1));

    // if there is an input change, calculate the PV (y=mx+b)
    if ( InputSpan > 0 ) {
        scaledValue = (((inputValue - *((float *) buff_x + (i - 1))) * PVSpan) / InputSpan) + *((float *) buff_y + (i - 1));
        goto label_exitPoint;
    }

    // else
    scaledValue = *((float *) buff_y + (i - 1));

label_exitPoint:
#if (LOG_LEVEL>0)
    printf("\tInput value\t|\tCalculated value\n");
    printf("\t%3.2f\t\t\t\t%3.2f", inputValue, scaledValue);
#endif
    return scaledValue;
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

#if (LOG_LEVEL>0)
    printf("\nExtracted X: \n");
#endif
    while ( indexI < sampleCount ) {
        *((float *) pBuff_x + indexI) = *((float *) pDataSet + (indexI * 2));
#if (LOG_LEVEL>0)
        printf("X[%d]: %3.2f,", indexI, *((float *) pBuff_x + indexI));
        printf("\n");
#endif
        indexI++;
    }
}

static int extract_y(float *pDataSet, float *pBuff_y, int sampleCount) {
    if ( !pDataSet ) {
        return 0;
    }

    int indexI = 0;
    int indexJ = 0;

#if (LOG_LEVEL>0)
    printf("\nExtracted X: \n");
#endif
    while ( indexI < sampleCount ) {
        *((float *) pBuff_y + indexI) = *((float *) pDataSet + (indexI * 2) + 1);
#if (LOG_LEVEL>0)
        printf("Y[%d]: %3.2f,", indexI, *((float *) pBuff_y + indexI));
        printf("\n");
#endif
        indexI++;
    }
}
