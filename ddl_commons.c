/**
 * @file      ddl_commons.c
 * @author:   Shubhendu B B
 * @date:     02/02/2023
 * @brief
 * @details
 *
 * @copyright
 *
 **/

#include <string.h>

 /**/
#include "ddl_base.h"
#include "ddl_commons.h"
#include "stdbool.h"
#include "ddl_log.h"

int32_t ddl_commons_IsFoundString(const char *pString1, const char *pString2) {
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
 * @param[out]: uint8_t *pParsedHexDataBuff: buffer to write in
 *
 * @param[out]: size_t parsedHexDataBuff_length: length of the buffer to write
 *
 */
ddl_base_status_t ddl_commons_StringToHex(char *pString, uint8_t singnificantDigits, uint8_t *pParsedHexDataBuff,
    size_t parsedHexDataBuff_length) {
    ddl_base_status_t returnStatus = DDL_BASE_STATUS_OK;
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

char *ddl_commons_StrReverseInPlace(char *str, int len) {
    char *p1 = str;
    char *p2 = str + len - 1;

    while ( p1 < p2 ) {
        char tmp = *p1;
        *p1++ = *p2;
        *p2-- = tmp;
    }

    return str;
}

int32_t ddl_commons_ReplaceCharInString(char *pString, char charToReplace, char replaceByChar) {
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

int32_t ddl_commons_StringToLower(char *pString) {
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

const char *ddl_commons_findCharPointer(const char *s, size_t len, unsigned char c) {
    while ( len && *s++ != c ) --len;
    return len ? s : NULL;
}

ddl_base_status_t ddl_commons_GetDataFromString(uint8_t *dataBuff, const char *sourceString, const char *dataKey,
    const char *keyValueSeperator, const char *dataSeperator) {
    ddl_base_status_t processStatus = DDL_BASE_STATUS_OK;
    int32_t dataKeyStartIndexOffset = 0;
    int32_t dataKeyStartIndex = -1;
    int32_t dataStringLength = -1;

    if ( dataBuff && sourceString && dataKey && keyValueSeperator && dataSeperator ) {
    label_checkRemainingStringForTheKey:

        dataKeyStartIndex = ddl_commons_IsFoundString(sourceString + dataKeyStartIndexOffset, dataKey);

        if ( (dataKeyStartIndex > -1) && (strlen(dataKey) < (strlen(sourceString) - dataKeyStartIndexOffset)) ) {
            bool isKey = (*(sourceString + dataKeyStartIndex + strlen(dataKey)) == *keyValueSeperator) ? true : false;
            if ( isKey ) {
                if ( strlen(dataKey) < (strlen(sourceString) - dataKeyStartIndexOffset) ) {
                    dataStringLength = ddl_commons_IsFoundString(sourceString + dataKeyStartIndex, dataSeperator);
                    if ( dataStringLength == -1 ) {
                        dataStringLength =
                            strlen(sourceString) - dataKeyStartIndex - strlen(dataKey) - strlen(keyValueSeperator);
                    } else {
                        dataStringLength -= (strlen(dataKey) + strlen(keyValueSeperator));
                    }
                    memcpy(dataBuff, sourceString + dataKeyStartIndex + strlen(dataKey) + strlen(keyValueSeperator),
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
        processStatus = DDL_BASE_ERROR_BAD_ARGUMENT;
    }
    return processStatus;
}
