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

#include "ddl_util.h"


#define SAMPLE_COUNT_MIN  0x000f
#define SAMPLE_COUNT_MAX  0x0FF0
#define SAMPLE_COUNT_MID  0x0800

#define SCALE_TO_MIN  -100
#define SCALE_TO_MAX  +100
#define SCALE_TO_MID  0

 // #define SAMPLE_VALUE (((0x03FF / 1)*2)+0)
#define SAMPLE_VALUE 0x07ff
#define ADJUST_TO_DECIMAL 4


void main(int argc, char *argv []) {
    float normalized = 0.0, scaled = 0.0;
    normalized = ddl_util_normalize((float) SAMPLE_COUNT_MIN, (float) SAMPLE_COUNT_MAX, (float) SAMPLE_COUNT_MID, (float) SAMPLE_VALUE, ADJUST_TO_DECIMAL);
    scaled = ddl_util_scale((float) SCALE_TO_MIN, (float) SCALE_TO_MAX, (float) SCALE_TO_MID, (float) normalized, ADJUST_TO_DECIMAL);
    // scaled = scaled + (((float) SCALE_TO_MAX - (float) SCALE_TO_MID) / (float) SAMPLE_COUNT_MAX);
    printf("Normalized value: %4.8f\r\n", normalized);
    printf("Scaled value(dec): %4.4f\r\n", scaled);
    printf("Scaled value(hex): 0x%4.4X\r\n", (int) scaled);
}

