/**
 * @file      module_1.h
 * @author:   Shubhendu B B
 * @date:     16/10/2023
 * @brief
 * @details
 *
 * @copyright
 *
**/

#ifndef __MODULE_1_H__
#define __MODULE_1_H__


#include <stdint.h>


typedef enum {
    MODULE_1_EVT_TYPE_ENTRY = 0,
    MODULE_1_EVT_TYPE_TEST_1,
    MODULE_1_EVT_TYPE_TEST_2,
    MODULE_1_EVT_TYPE_EXIT
}module_1_evt_type_t;


typedef struct {
    module_1_evt_type_t eventType;
    uint8_t *pDataBuff;
    size_t dataBuffLen;
}module_1_evt_t;

#ifdef __cplusplus
extern "C" {
#endif

    int module_1_task(void);


#ifdef __cplusplus
}
#endif




#endif /* @end  __MODULE_1_H__*/