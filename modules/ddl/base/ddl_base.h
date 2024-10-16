/**
 * @file      ddl_base.h
 * @author:   Shubhendu B B
 * @date:     02/02/2023
 * @brief     Base definitions and type declarations
 * @details   This header file defines common types, macros, and data structures
 *            used throughout the project, including data type unions, status enums,
 *            and date-time structures.
 *
 * @copyright (c) 2023 Shubhendu B B. All rights reserved.
 **/

#ifndef __DDL_BASE_H__
#define __DDL_BASE_H__

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

// Define boolean types
#ifndef BOOL
#define BOOL bool
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE !FALSE
#endif

// Define carriage return and line feed constants
#ifndef CR
#define CR 0x0D
#endif

#ifndef LF
#define LF 0x0A
#endif

// Bit manipulation macros
#ifndef BIT_SHIFT
#define BIT_SHIFT(DWORD, BITPOSITION) ((DWORD) |= (1 << (BITPOSITION)))
#endif

// Type definitions for byte and word sizes
#ifndef BYTE
#define BYTE uint8_t
#endif

#ifndef WORD
#define WORD uint16_t
#endif

// String macros
#ifndef STRING
#define STRING(x) #x
#define STR(x) STRING(x)
#endif

// Define unsigned char type
#ifndef uchar_t
typedef unsigned char uchar_t;
#endif

// Type definitions for various sizes
#ifndef uint8_t
typedef unsigned char uint8_t;
#endif

// Define HIGH and LOW constants
#ifndef HIGH
#define HIGH 1u
#endif

#ifndef LOW
#define LOW 0u
#endif

// Define ON and OFF constants
#ifndef ON
#define ON 1u
#endif

#ifndef OFF
#define OFF 0u
#endif

// Define pass and fail constants
#ifndef PASS
#define PASS 1u
#endif

#ifndef FAIL
#define FAIL 0u
#endif

// Enumeration for base data types
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
    FLOAT_T,
    FLOAT64_T,
    LONG_T,
    LONG_LONG_T,
    DOUBLE_T,
    STRING_T
} ddl_base_type_t;

// Enumeration for bit positions
typedef enum { BIT0, BIT1, BIT2, BIT3, BIT4, BIT5, BIT6, BIT7 } ddl_base_bit_t;

// Type definition for boolean
#ifndef bool
#define bool _Bool
#endif

// Union for byte representation
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

// Union for word representation
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

// Union for double word representation
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

// Union for quad word representation
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

// Type definitions for additional sizes
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

// Enumeration for date formats
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

// Structure for time of day representation
typedef struct {
    uint8_t dd;                             // Day
    uint8_t mm;                             // Month
    uint8_t yy;                             // Year
    uint8_t day;                            // Day of the week
    uint8_t hr;                             // Hour
    uint8_t min;                            // Minute
    uint8_t sec;                            // Second
    uint8_t ms;                             // Millisecond
    unsigned timeFormat : 1;                // 0: 24-hour, 1: AM/PM
    unsigned meridianIndicator : 1;         // 0: AM, 1: PM
    ddl_base_date_format_t dateFormatType;  // Date format type
} ddl_base_tod_t;

// Structure for ADC data representation
typedef struct {
    uint8_t channelNumber;  // ADC channel number
    real32_t channelValue;  // ADC channel value
} ddl_base_adcData_t;

// Structure for edge processing buffer
typedef struct {
    unsigned lastState : 1;     // Last state
    unsigned currentState : 1;  // Current state
    unsigned positiveEdge : 1;  // Positive edge detected
    unsigned negativeEdge : 1;  // Negative edge detected
    unsigned level : 1;         // Current level
    unsigned highLevel : 1;     // High level detected
    unsigned lowLevel : 1;      // Low level detected
} ddl_base_edgeProcessBuffer_t;

// Enumeration for status codes
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
