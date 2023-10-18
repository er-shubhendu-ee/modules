/**
 * @file      ddl_evt.h
 * @author:   Shubhendu B B
 * @date:     18/10/2023
 * @brief
 * @details
 *
 * @copyright
 *
**/

#ifndef __DDL_EVT_H__
#define __DDL_EVT_H__


#include <stdint.h>

typedef enum {
    DDL_EVT_TYPE_ENTRY = 0,
    DDL_EVT_TYPE_TEST_1,
    DDL_EVT_TYPE_TEST_2,
    DDL_EVT_TYPE_EXIT
}ddl_evt_type_t;

typedef struct {
    ddl_evt_type_t eventType;
    uint8_t *pDataBuff;
    size_t dataBuffLen;
}ddl_evt_t;

#ifdef __cplusplus
extern "C" {
#endif




#ifdef __cplusplus
}
#endif




#endif /* @end  __DDL_EVT_H__*/