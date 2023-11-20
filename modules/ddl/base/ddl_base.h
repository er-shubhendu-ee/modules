/**
 * @file      ddl_base.h
 * @author:   Shubhendu B B
 * @date:     02/02/2023
 * @brief
 * @details
 *
 * @copyright
 *
 **/

#ifndef __DDL_BASE_H__
#define __DDL_BASE_H__

#include <stdbool.h>
#include <stdint.h>
#include <complex.h>
#include <inttypes.h>
#include <iso646.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <tgmath.h>
#include <wchar.h>
#include <wctype.h>

#ifndef BOOL
#define BOOL bool
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE !FALSE
#endif

#ifndef false
#define false 0
#endif

#ifndef true
#define true !false
#endif

#ifndef CR
#define CR 0x0D
#endif

#ifndef LF
#define LF 0x0A
#endif

#ifndef BIT_SHIFT
#define BIT_SHIFT(DWORD, BITPOSITION) ((DWORD) |= (1 << (BITPOSITION)))
#endif

 // #define bitclear(value,nbit) ((value) &= ~(1<<(nbit)))
 // #define bitflip(value,nbit)  ((value) ^=  (1<<(nbit)))
 // #define bitcheck(value,nbit) ((value) &   (1<<(nbit)))

#ifndef BYTE
#define BYTE uint8_t
#endif

#ifndef WORD
#define WORD uint16_t
#endif

#ifndef STRING
#define STRING(x) #x
#define STR(x) STRING(x)
#endif


#ifndef uchar_t
typedef unsigned char uchar_t;
#endif

#ifndef uint8_t
typedef unsigned char uint8_t;
#endif

#ifndef HIGH
#define HIGH 1u
#endif

#ifndef LOW
#define LOW 0u
#endif

#ifndef ON
#define ON 1u
#endif

#ifndef OFF
#define OFF 0u
#endif

#ifndef TRUE
#define TRUE 1u
#endif

#ifndef FALSE
#define FALSE 0u
#endif

#ifndef PASS
#define PASS 1u
#endif

#ifndef FAIL
#define FAIL 0u
#endif

/* 	#define Min_Data 0x0
    #define Max_Data 0xF */

typedef enum {
    CHAR8_T,
    UCHAR8_T,
    INTEGER,
    INT8_T,
    UINT8_T,
    INT16_T,
    UINT16_T,
    INT32_T,
    UINT32_T,
    INT64_T,
    UINT64_T,
    FLOAT,
    FLOAT64_T,
    LONG,
    LONG_LONG,
    DOUBLE,
    STRING
} ddl_base_type_t;

typedef enum {
    BIT0,
    BIT1,
    BIT2,
    BIT3,
    BIT4,
    BIT5,
    BIT6,
    BIT7
} ddl_base_bit_t;

#ifndef bool
#define bool _Bool
#endif

typedef union {
    unsigned char value;
    struct {
        unsigned B0 : 1;
        unsigned B1 : 1;
        unsigned B2 : 1;
        unsigned B3 : 1;
        unsigned B4 : 1;
        unsigned B5 : 1;
        unsigned B6 : 1;
        unsigned B7 : 1;
    };
} ddl_base_byte_t;

typedef union {
    uint16_t value;
    struct {
        ddl_base_byte_t byte_0;
        ddl_base_byte_t byte_1;
    };
    struct {
        unsigned B0 : 1;
        unsigned B1 : 1;
        unsigned B2 : 1;
        unsigned B3 : 1;
        unsigned B4 : 1;
        unsigned B5 : 1;
        unsigned B6 : 1;
        unsigned B7 : 1;
        unsigned B8 : 1;
        unsigned B9 : 1;
        unsigned B10 : 1;
        unsigned B11 : 1;
        unsigned B12 : 1;
        unsigned B13 : 1;
        unsigned B14 : 1;
        unsigned B15 : 1;
    };
} ddl_base_word_t;

typedef union {
    uint32_t value;
    struct {
        ddl_base_byte_t byte_0;
        ddl_base_byte_t byte_1;
        ddl_base_byte_t byte_2;
        ddl_base_byte_t byte_3;
    };
    struct {
        ddl_base_word_t word_0;
        ddl_base_word_t word_1;
    };
    struct {
        unsigned B0 : 1;
        unsigned B1 : 1;
        unsigned B2 : 1;
        unsigned B3 : 1;
        unsigned B4 : 1;
        unsigned B5 : 1;
        unsigned B6 : 1;
        unsigned B7 : 1;
        unsigned B8 : 1;
        unsigned B9 : 1;
        unsigned B10 : 1;
        unsigned B11 : 1;
        unsigned B12 : 1;
        unsigned B13 : 1;
        unsigned B14 : 1;
        unsigned B15 : 1;
        unsigned B16 : 1;
        unsigned B17 : 1;
        unsigned B18 : 1;
        unsigned B19 : 1;
        unsigned B20 : 1;
        unsigned B21 : 1;
        unsigned B22 : 1;
        unsigned B23 : 1;
        unsigned B24 : 1;
        unsigned B25 : 1;
        unsigned B26 : 1;
        unsigned B27 : 1;
        unsigned B28 : 1;
        unsigned B29 : 1;
        unsigned B30 : 1;
        unsigned B31 : 1;
    };
} ddl_base_dword_t;

typedef union {
    uint64_t value;
    struct {
        ddl_base_byte_t byte_0;
        ddl_base_byte_t byte_1;
        ddl_base_byte_t byte_2;
        ddl_base_byte_t byte_3;
        ddl_base_byte_t byte_4;
        ddl_base_byte_t byte_5;
        ddl_base_byte_t byte_6;
        ddl_base_byte_t byte_7;
    };
    struct {
        ddl_base_word_t word_0;
        ddl_base_word_t word_1;
        ddl_base_word_t word_2;
        ddl_base_word_t word_3;
    };
    struct {
        ddl_base_dword_t dword_0;
        ddl_base_dword_t dword_1;
    };
    struct {
        unsigned B0 : 1;
        unsigned B1 : 1;
        unsigned B2 : 1;
        unsigned B3 : 1;
        unsigned B4 : 1;
        unsigned B5 : 1;
        unsigned B6 : 1;
        unsigned B7 : 1;
        unsigned B8 : 1;
        unsigned B9 : 1;
        unsigned B10 : 1;
        unsigned B11 : 1;
        unsigned B12 : 1;
        unsigned B13 : 1;
        unsigned B14 : 1;
        unsigned B15 : 1;
        unsigned B16 : 1;
        unsigned B17 : 1;
        unsigned B18 : 1;
        unsigned B19 : 1;
        unsigned B20 : 1;
        unsigned B21 : 1;
        unsigned B22 : 1;
        unsigned B23 : 1;
        unsigned B24 : 1;
        unsigned B25 : 1;
        unsigned B26 : 1;
        unsigned B27 : 1;
        unsigned B28 : 1;
        unsigned B29 : 1;
        unsigned B30 : 1;
        unsigned B31 : 1;
        unsigned B32 : 1;
        unsigned B33 : 1;
        unsigned B34 : 1;
        unsigned B35 : 1;
        unsigned B36 : 1;
        unsigned B37 : 1;
        unsigned B38 : 1;
        unsigned B39 : 1;
        unsigned B40 : 1;
        unsigned B41 : 1;
        unsigned B42 : 1;
        unsigned B43 : 1;
        unsigned B44 : 1;
        unsigned B45 : 1;
        unsigned B46 : 1;
        unsigned B47 : 1;
        unsigned B48 : 1;
        unsigned B49 : 1;
        unsigned B50 : 1;
        unsigned B51 : 1;
        unsigned B52 : 1;
        unsigned B53 : 1;
        unsigned B54 : 1;
        unsigned B55 : 1;
        unsigned B56 : 1;
        unsigned B57 : 1;
        unsigned B58 : 1;
        unsigned B59 : 1;
        unsigned B60 : 1;
        unsigned B61 : 1;
        unsigned B62 : 1;
        unsigned B63 : 1;
    };
} ddl_base_ddword_t;

#ifndef byte_t
#define byte_t unsigned char
#endif

#ifndef word_t
#define word_t unsigned char
#endif

#ifndef DoubleWord_t
#define DoubleWord_t unsigned char
#endif

#ifndef uchar8_t
#define uchar8_t byte_t
#endif

#ifndef uint32_t
#define uint32_t unsigned long int
#endif

#ifndef uint64_t
#define uint64_t unsigned long long int
#endif

#ifndef real32_t
#define real32_t float
#endif

#ifndef real64_t
#define real64_t double
#endif

typedef enum {
    DDMMYY,
    DDMMYYYY,
    DDMMMYY,
    DDMMMYYYY,
    MMDDYY,
    MMDDYYYY,
    MMMDDYY,
    MMMDDYYYY,
} ddl_base_date_format_t;

typedef struct {
    uint8_t dd;
    uint8_t mm;
    uint8_t yy;
    uint8_t day;
    uint8_t hr;
    uint8_t min;
    uint8_t sec;
    uint8_t ms;
    unsigned timeFormat : 1;         // default(0):24 hour; 1 = AM/PM
    unsigned meridianIndicator : 1;  // default(0):AM ; 1 = PM
    ddl_base_date_format_t dateFormatType;
} ddl_base_tod_t;

typedef struct {
    uint8_t channelNumber;
    real32_t channelValue;
} ddl_base_adcData_t;

typedef struct {
    unsigned lastState : 1;
    unsigned currentState : 1;
    unsigned positiveEdge : 1;
    unsigned negativeEdge : 1;
    unsigned level : 1;
    unsigned highLevel : 1;
    unsigned lowLevel : 1;
} ddl_base_edgeProcessBuffer_t;

typedef enum {
    DDL_BASE_STATUS_OK = 0,
    DDL_BASE_STATUS_BUSY,

    DDL_BASE_STATUS_ERROR = -1,
} ddl_base_status_t;

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif /* @end __DDL_BASE_H__ */