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
#include "data_process.h"
#include "ddl_log.h"

#define TAG "MAIN"

#define SAMPLE_COUNT 11
#define BUFFER_SIZE 100

#define SAMPLE_INPUT_VALUE 4.6

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




void main(int argc, char *argv []) {
    int indexI = 0;
    float inputValue = (float) 0;
    float buff_x [ BUFFER_SIZE ] = { (float) 0 };
    float buff_y [ BUFFER_SIZE ] = { (float) 0 };

    indexI = 0;
    while ( (float) inputValue < (float) 5 ) {
        buff_x [ indexI ] = inputValue;
        buff_y [ indexI ] = data_process_predict_y_value((float *) dataSet, SAMPLE_COUNT, (float) buff_x [ indexI ]);

        printf("\n%3.2f,%3.2f", buff_x [ indexI ], buff_y [ indexI ]);

        indexI++;
        inputValue += 0.05;
    }

    if ( ddl_util_plot_function_2d("title", "x label", "y label", buff_x, buff_y, indexI) ) {
        DDL_LOGE(TAG, "Failed to plot data.");
    } else {
        DDL_LOGI(TAG, "Plotting data success.");
    }
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
