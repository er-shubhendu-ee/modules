/**
 * @file      ddl_util.h
 * @author:   Shubhendu B B
 * @date:     21/10/2023
 * @brief
 * @details
 *
 * @copyright
 *
**/

#ifndef __DDL_UTIL_H__
#define __DDL_UTIL_H__

#include <ctype.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "ddl_base.h"

/************************************************************************/
/*	Following macro will extract lower value of given data                */
/************************************************************************/

#ifndef LOWBYTE
#define LOWBYTE(v)   			((uint8_t) ((v) & 0xFFu))
#endif

/************************************************************************/
/*	Following macro will extract lower value of given data (16-bit)         */
/************************************************************************/
#if defined BUSWIDTH_8
#define HIGHBYTE(v)  			((uint8_t) ((uint16_t)(v) >> 8))	/* This macro will extract lower byte of given data (16-bit) */
#endif

#if defined BUSWIDTH_16
#define HIGHBYTE(v)  			((uint8_t) ((uint16_t)(v) >> 8))	/* This macro will extract lower byte of given data (16-bit) */
#endif

#if defined BUSWIDTH_32
#define HIGHBYTE(v)  			((uint8_t) ((uint32_t)(v) >> 24))	/* This macro will extract lowest byte of given data (32-bit) */
#endif

/************************************************************************/
/*	Following macro will extract a bit from given data of 32-bit				*/
/*	extracted bit value=GETBIT(32-bit data, bit position)
bit position is counted from right and assumed that zero-th bit is at far right */
/************************************************************************/
#if defined BUSWIDTH_8
#define GETBIT(data, bit)  		((((uint8_t) data) >> bit) & ((uint8_t) 0x01))
#endif

#if defined BUSWIDTH_16
#define GETBIT(data, bit)  		((((uint16_t) data) >> bit) & ((uint16_t) 0x01))
#endif

#if defined BUSWIDTH_32
#define GETBIT(data, bit)  		((((uint32_t) data) >> bit) & ((uint32_t) 0x01))
#endif
/************************************************************************/
/*	Following macro will set (1) a bit in a given data (32-bit) */
/************************************************************************/

#if defined BUSWIDTH_8
#define SETBIT(data, bit)		(((uint8_t) data) | (0x01<<((uint8_t)bit)))
#endif

#if defined BUSWIDTH_16
#define SETBIT(data, bit)		(((uint16_t) data) | (0x01<<((uint16_t)bit)))
#endif

#if defined BUSWIDTH_32
#define SETBIT(data, bit)		(((uint32_t) data) | (0x01<<((uint32_t)bit)))
#endif
/************************************************************************/
/*	Following macro will reset(0) a bit in a given data (32-bit) */
/************************************************************************/

#if defined BUSWIDTH_8
#define RESETBIT(data, bit)		(((uint8_t) data) & ~(((uint8_t) 0x01) << bit))
#endif

#if defined BUSWIDTH_16
#define RESETBIT(data, bit)		(((uint16_t) data) & ~(((uint16_t) 0x01) << bit))
#endif

#if defined BUSWIDTH_32
#define RESETBIT(data, bit)		(((uint32_t) data) & ~(((uint32_t) 0x01) << bit))
#endif

/************************************************************************/
/* inverting a bit in a given data                                      */
/************************************************************************/

//#if defined BUSWIDTH_8
#define INVERTBIT(data, bit)	((GETBIT(((uint8_t) data), ((uint8_t) bit))) ? (RESETBIT(((uint8_t) data), ((uint8_t) bit))) : (SETBIT(((uint8_t) data), ((uint8_t) bit))))
//#endif

/************************************************************************/
/*	Following macro will invert(1->0 or 0->1) a bit in a given data (32-bit) */
/************************************************************************/
#define INVERT(x)				(x =~ x)
#define	COMPLEMENT(x)			(INVERT(x))

    /**
    int types
    **/
typedef enum TagIntType {
    UINT8, INT8, UINT16, INT16, UINT32, INT32, UINT64, INT64
} IntType_t;

typedef enum TagDisplayFormat {
    INT, OCT, HEX, BIN
} DisplayFormat_t;


#ifdef __cplusplus
extern "C" {
#endif


    char *rand_string(char *, size_t);
    uint8_t ftofs(uint8_t *, float, uint8_t, uint8_t);
    void ftoa(unsigned char *, float);
    void int_to_char(int32_t, uchar_t *, uint8_t);
    void int_to_char_array(char *, void *, int, int, IntType_t, DisplayFormat_t);
    void edge_detect(ddl_base_edgeProcessBuffer_t *pThisSignal);
    int pwr(int a, int b);


    int ddl_util_string_to_hex(char *pString, uint8_t singnificantDigits, uint8_t *pParsedHexDataBuff,
        size_t parsedHexDataBuff_length);
    char *ddl_util_string_rev_in_place(char *str, int len);

    /*!
     * @fn         int32_t ddl_util_is_found_string(const char* pString1, const char* pString2)
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
    int32_t ddl_util_is_found_string(const char *pString1, const char *pString2);
    int32_t ddl_util_find_extension_start_index(char *pFileName);
    int32_t ddl_util_replace_char_in_string(char *pString, char charToReplace, char replaceByChar);
    int32_t ddl_util_string_to_lower(char *pString);
    const char *ddl_util_find_char_pointer(const char *s, size_t len, unsigned char c);

    /*
     * @param dataBuff[IN] Destination
     * @param sourceString[IN] sourceString
     */
    ddl_base_status_t ddl_util_get_data_from_string(uint8_t *dataBuff, const char *sourceString, const char *dataKey,
        const char *ketValueSeperator, const char *dataSeperator);


    void ddl_util_timSort(int arr [], int n);

    float __attribute__((inline))  ddl_util_normalize(float valueMin, float valueMax, float valueMid, float valueIn, int roundToDecimal);
    float __attribute__((inline))  ddl_util_scale(float valueMin, float valueMax, float valueMid, float normalizedValueIn, int roundToDecimal);
    float ddl_util_find_min(float *dataBuff, int dataBuffLen);
    float ddl_util_find_max(float *dataBuff, int dataBuffLen);
    int ddl_util_plot_function_2d(char *pTitleString, char *xLabel, char *yLabel, float *pDataBuff_x, float *pDataBuff_y, int dataPointCount);

#ifdef __cplusplus
}
#endif




#endif /* @end  __DDL_UTIL_H__*/