/**
 * @file      ddl_commons.h
 * @author:   Shubhendu B B
 * @date:     02/02/2023
 * @brief
 * @details
 *
 * @copyright
 *
 **/
#ifndef __DDL_COMMONS_H__
#define __DDL_COMMONS_H__

#include <ctype.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**/
#include "ddl_base.h"

#ifdef __cplusplus
extern "C" {
#endif

ddl_base_status_t ddl_commons_StringToHex(char* pString, uint8_t singnificantDigits, uint8_t* pParsedHexDataBuff,
                                          size_t parsedHexDataBuff_length);
char* ddl_commons_StrReverseInPlace(char* str, int len);

/*!
 * @fn         int32_t ddl_commons_IsFoundString(const char* pString1, const char* pString2)
 * @brief      compares two string and returns 0 based index of the first character of pString2 at first occurence in
 *              pString1 if pString2 is found inside pString1
 *
 * @param[in]  pString1  string 1, main string
 * @param[in]  pString2  string 2, sub string
 * @exception
 * @return          int32_t zero based start index of string 2 in string 1
 *
 * @details
 *
 **/
int32_t ddl_commons_IsFoundString(const char* pString1, const char* pString2);
int32_t ddl_commons_FindExtensionStartIndex(char* pFileName);
int32_t ddl_commons_ReplaceCharInString(char* pString, char charToReplace, char replaceByChar);
int32_t ddl_commons_StringToLower(char* pString);
const char* ddl_commons_findCharPointer(const char* s, size_t len, unsigned char c);

/*
 * @param dataBuff[IN] Destination
 * @param sourceString[IN] sourceString
 */
ddl_base_status_t ddl_commons_GetDataFromString(uint8_t* dataBuff, const char* sourceString, const char* dataKey,
                                                const char* ketValueSeperator, const char* dataSeperator);

#ifdef __cplusplus
}
#endif

#endif /* @end __DDL_COMMONS_H__ */