/**
 * @file      data_process.c
 * @author:   Shubhendu B B
 * @date:     26/12/2023
 * @brief
 * @details
 *
 * @copyright
 *
**/


#include "data_process.h"
#include "ddl_util.h"

#define SAMPLE_COUNT_MAX 50

float data_process_predict_y_value(float *pDataset, int sampleCount, float inputValue) {
    // declare locals
    int i = 0;
    float PVSpan = (float) 0;
    float InputSpan = (float) 0;
    float scaledValue = (float) 0;
    float buff_x [ SAMPLE_COUNT_MAX ] = { (float) 0 };
    float buff_y [ SAMPLE_COUNT_MAX ] = { (float) 0 };
    float valueMin = (float) 0;
    float valueMax = (float) 0;

#if (LOG_LEVEL>0)
    data_process_print_dataset((float *) pDataset, sampleCount);
#endif

    data_process_extract_x((float *) pDataset, buff_x, sampleCount);
    data_process_extract_y((float *) pDataset, buff_y, sampleCount);

    valueMin = ddl_util_find_min((float *) buff_x, sampleCount);
    valueMax = ddl_util_find_max((float *) buff_x, sampleCount);

    if ( (float) inputValue < valueMin ) {
        scaledValue = ddl_util_find_min((float *) buff_y, sampleCount);
        goto label_exitPoint;
    }

    if ( (float) inputValue > valueMax ) {
        scaledValue = ddl_util_find_max((float *) buff_y, sampleCount);
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



int data_process_print_dataset(float *pDataSet, int sampleCount) {
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


int data_process_extract_x(float *pDataSet, float *pBuff_x, int sampleCount) {
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

int data_process_extract_y(float *pDataSet, float *pBuff_y, int sampleCount) {
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
