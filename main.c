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


 // #define TEST_SINGLE_DIRECTION 1

#define TEST_BOTH_DIRECTION 1


 /* for 0A-50A */
#if defined(TEST_SINGLE_DIRECTION)
#define APP_CONFIG_ADC_COUNT_MIN 0x0062 // min
#define APP_CONFIG_ADC_COUNT_MID APP_CONFIG_ADC_COUNT_MIN // 0x0768 // mid
#define APP_CONFIG_ADC_COUNT_MAX 0x0E6F // max
// #define SAMPLE_VALUE 0x08DA // 7.5A

#define APP_ADC_SCALE_TO_MIN 0  
#define APP_ADC_SCALE_TO_MAX 50 
#define APP_ADC_SCALE_TO_MID APP_ADC_SCALE_TO_MIN


// #define SAMPLE_VALUE (APP_ADC_COUNT_OFFSET+APP_CONFIG_ADC_COUNT_MIN)
// #define SAMPLE_VALUE (APP_ADC_COUNT_OFFSET+APP_CONFIG_ADC_COUNT_MAX)
#define APP_CONFIG_ADC_COUNT_DIVISION_UNIT 4
#define SAMPLE_VALUE (APP_ADC_COUNT_OFFSET+APP_CONFIG_ADC_COUNT_MIN+((APP_CONFIG_ADC_COUNT_MAX-APP_CONFIG_ADC_COUNT_MIN)/APP_CONFIG_ADC_COUNT_DIVISION_UNIT)) // (((0x03FF / 1)*2)+0)
#endif

 /* for +-70A */
#if defined (TEST_BOTH_DIRECTION)
#define APP_CONFIG_ADC_COUNT_MIN 0x00DD // min
// #define APP_CONFIG_ADC_COUNT_MID 0x07DF // mid calculated
#define APP_CONFIG_ADC_COUNT_MID 0x07F8 // mid observed
#define APP_CONFIG_ADC_COUNT_MAX 0x0EE1 // max

#define APP_ADC_COUNT_OFFSET 0x0000 //0x000A
#define APP_ADC_COUNT_MIN  (APP_ADC_COUNT_OFFSET + APP_CONFIG_ADC_COUNT_MIN)
#define APP_ADC_COUNT_MID  (APP_ADC_COUNT_OFFSET + APP_CONFIG_ADC_COUNT_MID)
#define APP_ADC_COUNT_MAX  (APP_ADC_COUNT_OFFSET + APP_CONFIG_ADC_COUNT_MAX)

#define APP_ADC_SCALE_TO_MIN -50  //  -50
#define APP_ADC_SCALE_TO_MAX 50 // +50
#define APP_ADC_SCALE_TO_MID 0

// #define SAMPLE_VALUE (APP_ADC_COUNT_OFFSET+APP_CONFIG_ADC_COUNT_MIN)
// #define SAMPLE_VALUE (APP_ADC_COUNT_OFFSET+APP_CONFIG_ADC_COUNT_MID)
// #define SAMPLE_VALUE (APP_ADC_COUNT_OFFSET+APP_CONFIG_ADC_COUNT_MAX)
#define APP_CONFIG_ADC_COUNT_ADVANCED_UNIT 1
// #define SAMPLE_VALUE (APP_CONFIG_ADC_COUNT_MID+((APP_CONFIG_ADC_COUNT_MAX-APP_CONFIG_ADC_COUNT_MID)/APP_CONFIG_ADC_COUNT_ADVANCED_UNIT))
#define SAMPLE_VALUE (APP_CONFIG_ADC_COUNT_MID-((APP_CONFIG_ADC_COUNT_MID-APP_CONFIG_ADC_COUNT_MIN)/APP_CONFIG_ADC_COUNT_ADVANCED_UNIT))
#endif

#define APP_ADC_COUNT_OFFSET 0x0000 //0x000A
#define APP_ADC_COUNT_MIN  (APP_ADC_COUNT_OFFSET + APP_CONFIG_ADC_COUNT_MIN)
#define APP_ADC_COUNT_MID  (APP_ADC_COUNT_OFFSET + APP_CONFIG_ADC_COUNT_MID)
#define APP_ADC_COUNT_MAX  (APP_ADC_COUNT_OFFSET + APP_CONFIG_ADC_COUNT_MAX)

#define ADJUST_TO_DECIMAL 4


void main(int argc, char *argv []) {
    float normalized = 0.0, scaled = 0.0;
    normalized = ddl_util_normalize((float) APP_ADC_COUNT_MIN, (float) APP_ADC_COUNT_MAX, (float) APP_ADC_COUNT_MID, (float) SAMPLE_VALUE, ADJUST_TO_DECIMAL);
    scaled = ddl_util_scale((float) APP_ADC_SCALE_TO_MIN, (float) APP_ADC_SCALE_TO_MAX, (float) APP_ADC_SCALE_TO_MID, (float) normalized, ADJUST_TO_DECIMAL);

    printf("Min: 0x%4.4X\r\n", APP_ADC_COUNT_MIN);
    printf("Mid: 0x%4.4X\r\n", APP_ADC_COUNT_MAX);
    printf("Max: 0x%4.4X\r\n", APP_ADC_COUNT_MAX);
    printf("Input: 0x%4.4X\r\n", SAMPLE_VALUE);

    printf("Normalized value: %4.8f\r\n", normalized);
    printf("Scaled value(dec): %4.4f\r\n", scaled);
    printf("Scaled value(hex): 0x%4.4X\r\n", (int) scaled);
}

