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





 /* for 0A-50A */
#define SAMPLE_VALUE_MIN 0x0062 // min
// #define SAMPLE_VALUE_MID 0x0768 // mid
#define SAMPLE_VALUE_MAX 0x0E6F // max
// #define SAMPLE_VALUE 0x08DA // 7.5A

 /* for +-70A */
// #define SAMPLE_VALUE_MIN 0x00DD // min
// #define SAMPLE_VALUE_MID 0x07DF // mid calculated
// #define SAMPLE_VALUE_MID 0x07F8 // mid observed
// #define SAMPLE_VALUE_MAX 0x0EE1 // max



#define APP_CONFIG_ADC_OFFSET_I_BAT 0x0000 //0x000A
#define APP_CONFIG_ADC_COUNT_MIN_I_BAT  (APP_CONFIG_ADC_OFFSET_I_BAT + SAMPLE_VALUE_MIN)
#define APP_CONFIG_ADC_COUNT_MID_I_BAT  (APP_CONFIG_ADC_OFFSET_I_BAT + SAMPLE_VALUE_MIN)
#define APP_CONFIG_ADC_COUNT_MAX_I_BAT  (APP_CONFIG_ADC_OFFSET_I_BAT + SAMPLE_VALUE_MAX)

 /* for 0A-50A */
#define APP_CONFIG_ADC_SCALE_TO_MIN_I_BAT 0  //  -50
#define APP_CONFIG_ADC_SCALE_TO_MAX_I_BAT 50 // +50
#define APP_CONFIG_ADC_SCALE_TO_MID_I_BAT 0

 /* for +-70A */
// #define APP_CONFIG_ADC_SCALE_TO_MIN_I_BAT -50  //  -50
// #define APP_CONFIG_ADC_SCALE_TO_MAX_I_BAT 50 // +50
// #define APP_CONFIG_ADC_SCALE_TO_MID_I_BAT 0

#define SAMPLE_VALUE_DIVISION_UNIT 4
#define SAMPLE_VALUE (APP_CONFIG_ADC_OFFSET_I_BAT+SAMPLE_VALUE_MIN+((SAMPLE_VALUE_MAX-SAMPLE_VALUE_MIN)/SAMPLE_VALUE_DIVISION_UNIT)) // (((0x03FF / 1)*2)+0)

#define ADJUST_TO_DECIMAL 4


void main(int argc, char *argv []) {
    float normalized = 0.0, scaled = 0.0;
    normalized = ddl_util_normalize((float) APP_CONFIG_ADC_COUNT_MIN_I_BAT, (float) APP_CONFIG_ADC_COUNT_MAX_I_BAT, (float) APP_CONFIG_ADC_COUNT_MID_I_BAT, (float) SAMPLE_VALUE, ADJUST_TO_DECIMAL);
    scaled = ddl_util_scale((float) APP_CONFIG_ADC_SCALE_TO_MIN_I_BAT, (float) APP_CONFIG_ADC_SCALE_TO_MAX_I_BAT, (float) APP_CONFIG_ADC_SCALE_TO_MID_I_BAT, (float) normalized, ADJUST_TO_DECIMAL);
    // scaled = scaled + (((float) SCALE_TO_MAX - (float) SCALE_TO_MID) / (float) SAMPLE_COUNT_MAX);
    printf("Normalized value: %4.8f\r\n", normalized);
    printf("Scaled value(dec): %4.4f\r\n", scaled);
    printf("Scaled value(hex): 0x%4.4X\r\n", (int) scaled);
}

