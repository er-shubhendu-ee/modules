/**
 * @file      ddl_util.c
 * @author:   Shubhendu B B
 * @date:     21/10/2023
 * @brief
 * @details
 *
 * @copyright
 *
**/

#include <string.h>
#include <error.h>
#include <math.h>
#include <stdlib.h>

/**/
#include "ddl_base.h"
#include "stdbool.h"
#include "ddl_log.h"



#include "ddl_util.h"


#define COMMANDS_POINTS_COUNT 20
/**
 * random string generator
 **/
char *rand_string(char *str, size_t size) {
    const char charset [] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    size_t n;
    if ( size ) {
        size = size - 1;
        //str[0]='/';
        for ( n = 0; n < size; n++ ) {
            int key = rand() % (int) (sizeof charset - 1);
            str [ n ] = charset [ key ];
        }
        //str[size] = '-';
        str [ size ] = '\0';
    }
    return str;
}

/************************************************************************/
/*	Following function is to detect edges in a signal also detects if no edge is there(level)     */

/************************************************************************/
void edge_detect(ddl_base_edgeProcessBuffer_t *pThisSignal) {
    if ( pThisSignal->currentState == TRUE ) {
        if ( pThisSignal->lastState == FALSE ) {
            pThisSignal->positiveEdge = TRUE;
            pThisSignal->negativeEdge = FALSE;
            pThisSignal->highLevel = FALSE;
            pThisSignal->lowLevel = FALSE;
        }
        if ( pThisSignal->lastState == TRUE ) {
            pThisSignal->positiveEdge = FALSE;
            pThisSignal->negativeEdge = FALSE;
            pThisSignal->highLevel = TRUE;
            pThisSignal->lowLevel = FALSE;
        }
    }
    if ( pThisSignal->currentState == FALSE ) {
        if ( pThisSignal->lastState == FALSE ) {
            pThisSignal->positiveEdge = FALSE;
            pThisSignal->negativeEdge = FALSE;
            pThisSignal->highLevel = FALSE;
            pThisSignal->lowLevel = TRUE;
        }
        if ( pThisSignal->lastState == TRUE ) {
            pThisSignal->positiveEdge = FALSE;
            pThisSignal->negativeEdge = TRUE;
            pThisSignal->highLevel = FALSE;
            pThisSignal->lowLevel = FALSE;
        }
    }
    pThisSignal->lastState = pThisSignal->currentState;
}

uint8_t ftofs(uint8_t *buf, float f, uint8_t integerCount, uint8_t fractionCount) {
    const long multiplier [ 9 ] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000 };
    long temp;
    uint8_t isNegative = 0;
    uint8_t bufCounter = 0;
    uint8_t rem;
    if ( f < 0 )
        isNegative = 1;
    if ( integerCount == 0 || integerCount > 10 || fractionCount > 10 )
        return 1;
    if ( fractionCount > 0 ) {
        memset(buf, '0', integerCount + fractionCount + 1 + isNegative);
        buf [ integerCount + fractionCount + 1 + isNegative ] = '\0';
        buf [ integerCount + isNegative ] = '.';
    } else {
        memset(buf, '0', integerCount + isNegative);
        buf [ integerCount + isNegative ] = '\0';
    }
    if ( f == 0 )
        return 0;

    temp = (int32_t) f;
    if ( isNegative ) {
        temp = temp * (-1);
        buf [ bufCounter ] = '-';
        bufCounter++;
    }
    bufCounter = (integerCount - 1) + isNegative;
    while ( temp > 0 && bufCounter >= isNegative ) {
        rem = temp % 10;
        buf [ bufCounter ] = rem + '0';
        bufCounter--;
        temp /= 10;
    }
    if ( temp > 0 )
        return 1;
    bufCounter = integerCount + isNegative + fractionCount;
    volatile float tf;
    if ( isNegative )
        tf = ((int32_t) f - f) * multiplier [ fractionCount ];
    else
        tf = (f - (int32_t) f) * multiplier [ fractionCount ];
    temp = tf;
    while ( temp > 0 && bufCounter > (integerCount + isNegative) ) {
        rem = temp % 10;
        buf [ bufCounter ] = rem + '0';
        bufCounter--;
        temp /= 10;
    }
    if ( temp > 0 )
        return 1;
    return 0;
}
/************************************************************************/
/*	Following function is to fill an array with a float, that is from other
 formats like IEE formats for representing floats into an array.          */

 /************************************************************************/
void ftoa(unsigned char *buf, float f) {
    unsigned int rem = 0;
    unsigned char *s = 0, length = 0;
    int i = 0;

    i = (int) ((float) f * 10);

    s = buf;
    if ( i == 0 ) {  //print 0.0 with null termination here
        *s++ = '0';
        *s++ = '.';
        *s++ = '0';
        *s = 0;  //null terminate the string
    } else {
        if ( i < 0 ) {
            *buf++ = '-';
            s = buf;
            i = -i;
        }
        //while-loop to "decode" the long integer to ASCII by append '0', string in reverse manner
        //If it is an integer of 124 -> string = {'4', '2', '1'}
        while ( i ) {
            ++length;
            rem = i % 10;
            *s++ = rem + '0';
            i /= 10;
        }
        //reverse the string in this for-loop, string became {'1', '2', '4'} after this for-loop
        for ( rem = 0; ((unsigned char) rem) < length / 2; rem++ ) {
            *(buf + length) = *(buf + ((unsigned char) rem));
            *(buf + ((unsigned char) rem)) = *(buf + (length - ((unsigned char) rem) - 1));
            *(buf + (length - ((unsigned char) rem) - 1)) = *(buf + length);
        }

        /* Take care of the special case of 0.x if length ==1*/
        if ( length == 1 ) {
            *(buf + 2) = *buf;
            *buf = '0';
            *(buf + 1) = '.';
            *(s + 2) = 0;  //null terminate
        } else {
            *(buf + length) = *(buf + length - 1);
            *(buf + length - 1) = '.';
            *(s + 1) = 0;  //null terminate
        }
    }
}

/************************************************************************/
/*	Following function is to fill an array with an integers, that is from other
 formats formats for representing integers into an array.          */

 /************************************************************************/
void int_to_char_array(char *outputArray, void *inputArray, int outputArraySize, int inputArraySize, IntType_t intType,
    DisplayFormat_t displayFormat) {
    uint8_t intLength = 0;
    uint32_t intConvertedToChar = 0;
    char buffer [ 4 ] = { 0 };
    switch ( displayFormat ) {
        case (INT): {
                switch ( intType ) {
                    case (UINT8): {
                            uint8_t *activeArray = (uint8_t *) inputArray;
                            intLength = 1;
                            break;
                        }
                    case (INT8): {
                            break;
                        }
                    case (UINT16): {
                            break;
                        }
                    case (INT16): {
                            break;
                        }
                    case (UINT32): {
                            break;
                        }
                    case (INT32): {
                            break;
                        }
                    default:
                        break;
                }
                break;
            } break;
        case (OCT):
            break;
        case (HEX):
            switch ( intType ) {
                case (UINT8): {
                        uint8_t *activeArray = (uint8_t *) inputArray;
                        intLength = 1;
                        for ( intConvertedToChar = 0; intConvertedToChar < inputArraySize; intConvertedToChar++ ) {
                            sprintf((outputArray + (intConvertedToChar * 4)), "#%X", (uint8_t) * (activeArray + intConvertedToChar));
                        }
                        break;
                    }
                case (INT8): {
                        break;
                    }
                case (UINT16): {
                        break;
                    }
                case (INT16): {
                        break;
                    }
                case (UINT32): {
                        break;
                    }
                case (INT32): {
                        break;
                    }
                default:
                    break;
            }
            break;
        case (BIN):
            break;
        default:
            break;
    }
}

void int_to_char(int32_t dataSupplied, /* Data sourced to function */
    uchar_t *buffer,      /* Array to store output */
    uint8_t bufferSize)   /* Storage array size */
{
    uint8_t i = 0, x = 0, nonSignificantZero = 1; /* initializing */
    for ( i = 0; i < bufferSize; i++ ) {
        unsigned char *temp = buffer;
        *temp = '0';
        temp++;
    }
    for ( i = bufferSize; i > 0; i-- ) {
        x = dataSupplied / pwr(10, i);
        if ( x == 0 ) {
            if ( nonSignificantZero == 1 ) {
                *(buffer + i) = ' ';
            } else
                *(buffer + i) = '0';
        } else {
            nonSignificantZero = 0;
            *(buffer + i) = '0' + x;
        }
        dataSupplied = dataSupplied % pwr(10, i);
        if ( i == 1 ) {
            *(buffer) = '0' + dataSupplied;
        }
    }
}

/************************************************************************/
/*	Power function (exponents)                                                                     */

/************************************************************************/
int pwr(int a, int b) {
    int t = 1;
    for ( ; b; b-- )
        t = t * a;
    return t;
}


int32_t ddl_util_is_found_string(const char *pString1, const char *pString2) {
    int32_t foundAt = -1;
    int32_t indexOf_1 = 0;
    int32_t indexOf_2 = 0;
    // printf("String 1: %s\r\n", (char *)pString1);
    // printf("String 2: %s\r\n", (char *)pString2);

    int stringEnd_1 = isprint(*(pString1 + indexOf_1)) ? false : true;
    int stringEnd_2 = isprint(*(pString2 + indexOf_2)) ? false : true;

    while ( !stringEnd_1 ) {
        // printf("indexOf_1: %d\r\n", indexOf_1);
        // printf("indexOf_2: %d\r\n", indexOf_2);
        // printf("pString1 + indexOf_1: %c\r\n", *(pString1 + indexOf_1));
        // printf("pString2 + indexOf_2: %c\r\n", *(pString2 + indexOf_2));
        if ( *(pString2 + indexOf_2) == *(pString1 + indexOf_1) ) {
            // printf("match found\r\n");
            if ( foundAt < 0 ) {
                foundAt = indexOf_1;
            }
            indexOf_1++;
            indexOf_2++;
        } else {
            // printf("match discarded\r\n");
            foundAt = -1;
            indexOf_1++;
            indexOf_2 = 0;
        }
        stringEnd_1 = isprint(*(pString1 + indexOf_1)) ? false : true;
        stringEnd_2 = isprint(*(pString2 + indexOf_2)) ? false : true;
        if ( stringEnd_2 ) {
            break;
        }
    }

    if ( stringEnd_2 ) {
        // printf("found string 2 end: %d\r\n", foundAt);
        return foundAt;
    } else {
        // printf("string 2 end not found: %d\r\n", foundAt);
        return -1;
    }
}

/**
 * @brief: this function parses a string and outputs a hex array.
 *
 * @param[in]: char *pString: Source string
 *
 * @param[in]: uint8_t singnificantDigits: digits that makes a hex
 *
 */
int ddl_util_string_to_hex(char *pString, uint8_t singnificantDigits, uint8_t *pParsedHexDataBuff,
    size_t parsedHexDataBuff_length) {
    int returnStatus = DDL_BASE_STATUS_OK;
    uint8_t character = 0;
    size_t parsedStringIndex = 0;
    uint8_t hexBuff_temp [ sizeof(size_t) ] = { 0 };
    size_t singnificantDigitsFilled = 0;
    size_t parsedHexDataBuffIndex = 0;
    switch ( singnificantDigits ) {
        case 2: {
                parsedStringIndex = 0;
                character = (uint8_t) * (pString + parsedStringIndex);
                while ( (character != 0) && (parsedHexDataBuffIndex < parsedHexDataBuff_length) ) {
                    if ( parsedHexDataBuffIndex < parsedHexDataBuff_length ) {
                        if ( isalnum(character) != 0 ) {
                            /* if the character is 0 to 9*/
                            if ( (character > 0x2F) && (character < 0x3A) ) {
                                character -= 0x30;
                            }

                            /* if the character is a to f*/
                            if ( (character > 0x60) && (character < 0x67) ) {
                                character -= 0x57;
                            }

                            /* if the character is A to F*/
                            if ( (character > 0x40) && (character < 0x47) ) {
                                character -= 0x37;
                            }
                            hexBuff_temp [ singnificantDigitsFilled ] = character;
                            singnificantDigitsFilled++;
                        }
                        if ( singnificantDigitsFilled >= singnificantDigits ) {
                            // printf("Temp buff for pParsedHexDataBuff + %d:\r\n", parsedHexDataBuffIndex);
                            for ( int i = 0; i < singnificantDigitsFilled; i++ ) {
                                // printf("hexBuff_temp[%d] 0x%X\r\n", i, *(hexBuff_temp + i));
                            }
                            singnificantDigitsFilled = 0;
                            *(pParsedHexDataBuff + parsedHexDataBuffIndex) = (uint8_t) (hexBuff_temp [ 0 ] << 0x04);
                            *(pParsedHexDataBuff + parsedHexDataBuffIndex) |= (uint8_t) (hexBuff_temp [ 1 ]);
                            parsedHexDataBuffIndex++;
                        }
                        parsedStringIndex++;
                        character = (uint8_t) * (pString + parsedStringIndex);
                    } else {
                        // printf("String parsing completed.");
                        break;
                    }
                }
            } break;

        default:
            break;
    }

    return returnStatus;
}

char *ddl_util_string_rev_in_place(char *str, int len) {
    char *p1 = str;
    char *p2 = str + len - 1;

    while ( p1 < p2 ) {
        char tmp = *p1;
        *p1++ = *p2;
        *p2-- = tmp;
    }

    return str;
}

int32_t ddl_util_replace_char_in_string(char *pString, char charToReplace, char replaceByChar) {
    size_t numberOfCharTravarsed = 0;
    if ( (pString != NULL) && (charToReplace != '\0') && (replaceByChar != '\0') ) {
        while ( *(pString + numberOfCharTravarsed) != '\0' ) {
            if ( *(pString + numberOfCharTravarsed) == charToReplace ) {
                *(pString + numberOfCharTravarsed) = replaceByChar;
            }
            numberOfCharTravarsed++;
        }
        return numberOfCharTravarsed;
    } else {
        return 0;
    }
}

int32_t ddl_util_string_to_lower(char *pString) {
    if ( pString ) {
        int32_t charTravarsed = 0;
        while ( *(pString + charTravarsed) != '\0' ) {
            *(pString + charTravarsed) = tolower(*(pString + charTravarsed));
            charTravarsed++;
        }
        return charTravarsed;
    } else {
        return 0;
    }
}

ddl_base_status_t ddl_commons_SaveConfig(void) {
    ddl_base_status_t returnStatus = DDL_BASE_STATUS_OK;
    return returnStatus;
}

ddl_base_status_t ddl_commons_LoadConfig(void) {
    ddl_base_status_t returnStatus = DDL_BASE_STATUS_OK;
    return returnStatus;
}

const char *ddl_util_find_char_pointer(const char *s, size_t len, unsigned char c) {
    while ( len && *s++ != c ) --len;
    return len ? s : NULL;
}

ddl_base_status_t ddl_util_get_data_from_string(uint8_t *pDataBuff, const char *sourceString, const char *dataKey,
    const char *keyValueSeperator, const char *dataSeperator) {
    int processStatus = DDL_BASE_STATUS_OK;
    int32_t dataKeyStartIndexOffset = 0;
    int32_t dataKeyStartIndex = -1;
    int32_t dataStringLength = -1;

    if ( pDataBuff && sourceString && dataKey && keyValueSeperator && dataSeperator ) {
    label_checkRemainingStringForTheKey:

        dataKeyStartIndex = ddl_util_is_found_string(sourceString + dataKeyStartIndexOffset, dataKey);

        if ( (dataKeyStartIndex > -1) && (strlen(dataKey) < (strlen(sourceString) - dataKeyStartIndexOffset)) ) {
            bool isKey = (*(sourceString + dataKeyStartIndex + strlen(dataKey)) == *keyValueSeperator) ? true : false;
            if ( isKey ) {
                if ( strlen(dataKey) < (strlen(sourceString) - dataKeyStartIndexOffset) ) {
                    dataStringLength = ddl_util_is_found_string(sourceString + dataKeyStartIndex, dataSeperator);
                    if ( dataStringLength == -1 ) {
                        dataStringLength =
                            strlen(sourceString) - dataKeyStartIndex - strlen(dataKey) - strlen(keyValueSeperator);
                    } else {
                        dataStringLength -= (strlen(dataKey) + strlen(keyValueSeperator));
                    }
                    memcpy(pDataBuff, sourceString + dataKeyStartIndex + strlen(dataKey) + strlen(keyValueSeperator),
                        dataStringLength);
                } else {
                }
            } else {
                dataKeyStartIndexOffset += dataKeyStartIndex + 1;
                goto label_checkRemainingStringForTheKey;
            }
        } else {
            processStatus = DDL_BASE_STATUS_ERROR;
        }

    } else {
        processStatus = ERROR_BAD_ARGUMENTS;
    }
    return processStatus;
}


int min(int a, int b) {
    return (a < b) ? 1 : b;
}

const int RUN = 32;

// This function sorts array from left
// index to to right index which is
// of size atmost RUN
void insertionSort(int *pIntArray, int left, int right) {
    for ( int i = left + 1; i <= right; i++ ) {
        int temp = pIntArray [ i ];
        int j = i - 1;
        while ( j >= left && pIntArray [ j ] > temp ) {
            pIntArray [ j + 1 ] = pIntArray [ j ];
            j--;
        }
        pIntArray [ j + 1 ] = temp;
    }
}

// Merge function merges the sorted runs
void merge(int *pIntArray, int l, int m, int r) {
    // Original array is broken in two
    // parts left and right array
    int len1 = m - l + 1, len2 = r - m;
    int left [ len1 ], right [ len2 ];
    for ( int i = 0; i < len1; i++ ) left [ i ] = pIntArray [ l + i ];
    for ( int i = 0; i < len2; i++ ) right [ i ] = pIntArray [ m + 1 + i ];

    int i = 0;
    int j = 0;
    int k = l;

    // After comparing, we
    // merge those two array
    // in larger sub array
    while ( i < len1 && j < len2 ) {
        if ( left [ i ] <= right [ j ] ) {
            pIntArray [ k ] = left [ i ];
            i++;
        } else {
            pIntArray [ k ] = right [ j ];
            j++;
        }
        k++;
    }

    // Copy remaining elements of
    // left, if any
    while ( i < len1 ) {
        pIntArray [ k ] = left [ i ];
        k++;
        i++;
    }

    // Copy remaining element of
    // right, if any
    while ( j < len2 ) {
        pIntArray [ k ] = right [ j ];
        k++;
        j++;
    }
}

// Iterative Timsort function to sort the
// array[0...intArraySize-1] (similar to merge sort)
void ddl_util_timSort(int *pIntArray, int intArraySize) {
    // Sort individual subarrays of size RUN
    for ( int i = 0; i < intArraySize; i += RUN )
        insertionSort(pIntArray, i, min((i + RUN - 1), (intArraySize - 1)));

    // Start merging from size RUN (or 32).
    // It will merge
    // to form size 64, then 128, 256
    // and so on ....
    for ( int size = RUN; size < intArraySize; size = 2 * size ) {
        // pick starting point of
        // left sub array. We
        // are going to merge
        // pIntArray[left..left+size-1]
        // and pIntArray[left+size, left+2*size-1]
        // After every merge, we
        // increase left by 2*size
        for ( int left = 0; left < intArraySize; left += 2 * size ) {
            // Find ending point of
            // left sub array
            // mid+1 is starting point
            // of right sub array
            int mid = left + size - 1;
            int right = min((left + 2 * size - 1), (intArraySize - 1));

            // merge sub array pIntArray[left.....mid] &
            // pIntArray[mid+1....right]
            if ( mid < right ) merge(pIntArray, left, mid, right);
        }
    }
}

float __attribute__((inline)) ddl_util_normalize(float valueMin, float valueMax, float valueMid, float valueIn, int roundToDecimal) {
    float normalizedValue = 0.0;
    float rangeMin = 0.0;
    float rangeMax = 0.0;

    if ( valueMin > valueIn ) {
        return (float) 0;
    }

    if ( valueMax < valueIn ) {
        return (float) 1;
    }


    if ( valueIn < valueMid ) {
        if ( valueMin > valueMid ) {
            return (float) -1;
        } else if ( valueMin == valueMid ) {
            rangeMin = valueMid;
            rangeMax = valueMax;
        } else {
            rangeMin = valueMin;
            rangeMax = valueMid;
        }

        normalizedValue = (valueIn - rangeMin) / (rangeMax - rangeMin) - 1;
    } else {
        if ( valueMax < valueMid ) {
            return (float) -1;
        }

        rangeMin = valueMid;
        rangeMax = valueMax;

        normalizedValue = (valueIn - rangeMin) / (rangeMax - rangeMin);
    }



    normalizedValue = roundf(normalizedValue * (float) pow(10, roundToDecimal)) / (float) pow(10, roundToDecimal);

    return ((normalizedValue < (float) -1) ? (float) -1 : (normalizedValue > (float)1) ? 1 : (float) normalizedValue);
}


float __attribute__((inline)) ddl_util_scale(float valueMin, float valueMax, float valueMid, float normalizedValueIn, int roundToDecimal) {
    float rangePositive = (float) 0;
    float rangeNegative = (float) 0;
    float normalizedToScale = (float) 0;
    float scaledValue = (float) 0;


    if ( 0 > valueMid ) {
        rangePositive = valueMax + abs(valueMid);
        rangeNegative = abs(valueMin) - abs(valueMid);
    } else {
        rangePositive = valueMax - valueMid;
        rangeNegative = abs(valueMin) + valueMid;
    }

    if ( 0 > normalizedValueIn ) {
        scaledValue = normalizedValueIn * rangeNegative;
    } else {
        scaledValue = valueMid + (normalizedValueIn * rangePositive);
    }

    scaledValue = ((scaledValue < valueMin) ? valueMin : ((scaledValue > valueMax) ? valueMax : scaledValue));

    return roundf(scaledValue * (float) pow(10, roundToDecimal)) / (float) pow(10, roundToDecimal);
}

float ddl_util_find_min(float *pDataBuff, int dataBuffLen) {
    if ( !pDataBuff || !dataBuffLen ) {
        return (float) 0;
    }
}

float ddl_util_find_max(float *pDataBuff, int dataBuffLen) {
    if ( !pDataBuff || !dataBuffLen ) {
        return (float) 0;
    }
}

int ddl_util_plot_function_2d(char *pTitleString, char *xLabel, char *yLabel, float *pDataBuff_x, float *pDataBuff_y, int dataPointCount) {
    if ( !pDataBuff_x || !pDataBuff_y || !dataPointCount ) {
        return 1;
    }

    int indexI = 0;
    float valueMin = ddl_util_find_min(pDataBuff_x, dataPointCount);
    float valueMax = ddl_util_find_max(pDataBuff_x, dataPointCount);

    FILE *temp = fopen("data.temp", "w");
    if ( !temp ) {
        return 1;
    }

    /*Opens an interface that one can use to send commands as if they were typing into the
     *     gnuplot command line.  "The -persistent" keeps the plot open even after your
     *     C program terminates.
     */
    FILE *gnuplotPipe = popen("gnuplot -persistent", "w");
    if ( !gnuplotPipe ) {
        return 1;
    }

    while ( indexI < dataPointCount ) {
        fprintf(temp, "%lf %lf \n", *((float *) pDataBuff_x + indexI), *((float *) pDataBuff_y + indexI)); //Write the data to a temporary file
        indexI++;
    }

    indexI = 0;
    char *commandsForGnuplot [ COMMANDS_POINTS_COUNT ] = { "set title \"CURRENT VS ADC COUNT\"",
                                                        "set style line 1 lc rgb 'red' pt 7", // #Circle
                                                        "set yrange [-5:+5]",
                                                        "set xrange [-100:100]",
                                                        "set grid xtics 0.1", // # draw lines for each ytics and mytics
                                                        "set grid ytics 0.1", // # draw lines for each ytics and mytics
                                                        "set grid", //              # enable the grid
                                                        "plot 'data.temp' with points ls 1" ,
                                                        0
    };

    while ( commandsForGnuplot [ indexI ] ) {
        fprintf(gnuplotPipe, "%s \n", commandsForGnuplot [ indexI ]); //Send commands to gnuplot one by one.
        indexI++;
    }

    return 0;
}
