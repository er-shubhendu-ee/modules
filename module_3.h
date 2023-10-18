/**
 * @file      module_3.h
 * @author:   Shubhendu B B
 * @date:     16/10/2023
 * @brief
 * @details
 *
 * @copyright
 *
**/

#ifndef __MODULE_3_H__
#define __MODULE_3_H__


#include <stdint.h>


typedef enum {
    MODULE_3_EVT_TYPE_ENTRY = 0,
    MODULE_3_EVT_TYPE_TEST_1,
    MODULE_3_EVT_TYPE_TEST_2,
    MODULE_3_EVT_TYPE_EXIT
}module_3_evt_type_t;


typedef struct {
    module_3_evt_type_t eventType;
    uint8_t *pDataBuff;
    size_t dataBuffLen;
}module_3_evt_t;

#ifdef __cplusplus
extern "C" {
#endif

    int module_3_task(void);


#ifdef __cplusplus
}
#endif




#endif /* @end  __MODULE_3_H__*/