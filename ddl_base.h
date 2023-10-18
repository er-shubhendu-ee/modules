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

#define BIT_SHIFT(DWORD, BITPOSITION) ((DWORD) |= (1 << (BITPOSITION)))
 // #define bitclear(byte,nbit) ((byte) &= ~(1<<(nbit)))
 // #define bitflip(byte,nbit)  ((byte) ^=  (1<<(nbit)))
 // #define bitcheck(byte,nbit) ((byte) &   (1<<(nbit)))

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

typedef enum ddl_base_defines_tagDataType {
    DDL_BASE_DEFINES_DATA_TYPE_BOOL,
    DDL_BASE_DEFINES_DATA_TYPE_CHAR,
    DDL_BASE_DEFINES_DATA_TYPE_STRING,
    DDL_BASE_DEFINES_DATA_TYPE_INT8,
    DDL_BASE_DEFINES_DATA_TYPE_INT16,
    DDL_BASE_DEFINES_DATA_TYPE_INT32,
    DDL_BASE_DEFINES_DATA_TYPE_UINT8,
    DDL_BASE_DEFINES_DATA_TYPE_UINT16,
    DDL_BASE_DEFINES_DATA_TYPE_UINT32,
    DDL_BASE_DEFINES_DATA_TYPE_FLOAT
} ddl_base_defines_dataType_t;

typedef enum {
    DDL_BASE_STATUS_OK = 0,
    DDL_BASE_STATUS_BUSY,

    DDL_BASE_STATUS_ERROR = -1,
} ddl_base_status_t;

/* error defines */
#define DDL_BASE_ERROR_START 0x0010
#define DDL_BASE_ERROR_BAD_ARGUMENT (DDL_BASE_ERROR_START + 1)
#define DDL_BASE_ERROR_BUFFER_OVERFLOW (DDL_BASE_ERROR_BAD_ARGUMENT + 1)
#define DDL_BASE_ERROR_DRIVER_INITIALIZATION (DDL_BASE_ERROR_BUFFER_OVERFLOW + 1)
#define DDL_BASE_ERROR_MAX 0x00FF

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif /* @end __DDL_BASE_H__ */