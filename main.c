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

void main(int argc, char *argv []) {
    for ( size_t i = 0; i < 11; i++ ) {
        for ( size_t j = 0; j < 2; j++ ) {
            printf("%3.2f,", dataSet [ i ][ j ]);
        }
        printf("\n");
    }

    float a = (float) 0;
    float b = (float) 0;
    float c = (float) 0;
    float d = (float) 0;
    float sumX = (float) 0;
    float sumX2 = (float) 0;
    float sumX3 = (float) 0;
    float sumY = (float) 0;
    float sumXY = (float) 0;
    int indexI = 0;

    /* Calculating Required Sum */
    for ( indexI = 1; indexI <= SAMPLE_COUNT; indexI++ ) {
        sumX = sumX + dataSet [ indexI ][ 0 ];
        sumX2 = sumX2 + powf(dataSet [ indexI ][ 0 ], 2);
        sumX3 = sumX3 + powf(dataSet [ indexI ][ 0 ], 3);
        sumY = sumY + dataSet [ indexI ][ 1 ];
        sumXY = sumXY + dataSet [ indexI ][ 0 ] * dataSet [ indexI ][ 1 ];
    }
    /* Calculating a and b */
    c = (SAMPLE_COUNT * sumXY - sumX * sumY) / (SAMPLE_COUNT * sumX3 - pow(sumX, 3));
    b = (SAMPLE_COUNT * sumXY - sumX * sumY) / (SAMPLE_COUNT * sumX2 - pow(sumX, 2));
    a = (sumY - b * sumX) / SAMPLE_COUNT;
    /* Displaying value of a and b */
    printf("Values are: a=%0.4f, b = %0.4f and c = %0.4f", a, b, c);
    printf("\nEquation of best fit is: y = %0.4f + %0.4fx + %0.4fx^2", a, b, c);
}
