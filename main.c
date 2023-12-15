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

#define DATA_POINTS_COUNT 5
#define COMMANDS_POINTS_COUNT 20


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
// #define APP_CONFIG_ADC_COUNT_DIVISION_UNIT 4
// #define SAMPLE_VALUE (APP_ADC_COUNT_OFFSET+APP_CONFIG_ADC_COUNT_MIN+((APP_CONFIG_ADC_COUNT_MAX-APP_CONFIG_ADC_COUNT_MIN)/APP_CONFIG_ADC_COUNT_DIVISION_UNIT)) // (((0x03FF / 1)*2)+0)
#define SAMPLE_VALUE 0x00DA // 7.5A
#endif

 /* for +-70A */
#if defined (TEST_BOTH_DIRECTION)
#define APP_CONFIG_ADC_COUNT_MIN 0x00DD // min
// #define APP_CONFIG_ADC_COUNT_MID 0x07DF // mid calculated
#define APP_CONFIG_ADC_COUNT_MID 0x7FB // 0x07F8 // mid observed
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
// #define SAMPLE_VALUE (APP_CONFIG_ADC_COUNT_MID-((APP_CONFIG_ADC_COUNT_MID-APP_CONFIG_ADC_COUNT_MIN)/APP_CONFIG_ADC_COUNT_ADVANCED_UNIT))
#define SAMPLE_VALUE 0x0100
#endif

#define APP_ADC_COUNT_OFFSET 0x0000 //0x000A
#define APP_ADC_COUNT_MIN  (APP_ADC_COUNT_OFFSET + APP_CONFIG_ADC_COUNT_MIN)
#define APP_ADC_COUNT_MID  (APP_ADC_COUNT_OFFSET + APP_CONFIG_ADC_COUNT_MID)
#define APP_ADC_COUNT_MAX  (APP_ADC_COUNT_OFFSET + APP_CONFIG_ADC_COUNT_MAX)

#define ADJUST_TO_DECIMAL 4

static int plot_function(void);

void main(int argc, char *argv []) {
    float normalized = 0.0, scaled = 0.0;
    normalized = ddl_util_normalize((float) APP_ADC_COUNT_MIN, (float) APP_ADC_COUNT_MAX, (float) APP_ADC_COUNT_MID, (float) SAMPLE_VALUE, ADJUST_TO_DECIMAL);
    scaled = ddl_util_scale((float) APP_ADC_SCALE_TO_MIN, (float) APP_ADC_SCALE_TO_MAX, (float) APP_ADC_SCALE_TO_MID, (float) normalized, ADJUST_TO_DECIMAL);

    printf("Min: 0x%4.4X\r\n", APP_ADC_COUNT_MIN);
    printf("Mid: 0x%4.4X\r\n", APP_ADC_COUNT_MID);
    printf("Max: 0x%4.4X\r\n", APP_ADC_COUNT_MAX);
    printf("Input: 0x%4.4X\r\n", SAMPLE_VALUE);

    printf("Normalized value: %4.8f\r\n", normalized);
    printf("Scaled value(dec): %4.4f\r\n", scaled);
    printf("Scaled value(hex): 0x%4.4X\r\n", (int) scaled);

    plot_function();
}


static int plot_function(void) {
    char *commandsForGnuplot [ COMMANDS_POINTS_COUNT ] = { "set title \"TITLEEEEE\"",
                                                            "set yrange [0:100]",
                                                            "set xrange [0:10]",
                                                            "set grid xtics 1", // # draw lines for each ytics and mytics
                                                            "set grid ytics 1", // # draw lines for each ytics and mytics
                                                            "set grid", //              # enable the grid
                                                            "plot 'data.temp'" ,
                                                            0
    };
    double xvals [ DATA_POINTS_COUNT ] = { 1.0, 2.0, 3.0, 4.0, 5.0 };
    double yvals [ DATA_POINTS_COUNT ] = { 5.0 ,3.0, 1.0, 3.0, 5.0 };
    FILE *temp = fopen("data.temp", "w");
    /*Opens an interface that one can use to send commands as if they were typing into the
     *     gnuplot command line.  "The -persistent" keeps the plot open even after your
     *     C program terminates.
     */
    FILE *gnuplotPipe = popen("gnuplot -persistent", "w");
    int indexI = 0;
    while ( indexI < DATA_POINTS_COUNT ) {
        fprintf(temp, "%lf %lf \n", xvals [ indexI ], yvals [ indexI ]); //Write the data to a temporary file
        indexI++;
    }

    indexI = 0;
    while ( commandsForGnuplot [ indexI ] ) {
        fprintf(gnuplotPipe, "%s \n", commandsForGnuplot [ indexI ]); //Send commands to gnuplot one by one.
        indexI++;
    }

    return 0;
}
