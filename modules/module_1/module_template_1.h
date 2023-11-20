/**
 * @file      module_template_1.h
 * @author:   Shubhendu B B
 * @date:     16/10/2023
 * @brief
 * @details
 *
 * @copyright
 *
**/

#ifndef __MODULE_TEMPLATE_1_H__
#define __MODULE_TEMPLATE_1_H__


#include <stdint.h>


typedef enum {
    MODULE_TEMPLATE_1_EVT_TYPE_INVALID = 0,
    MODULE_TEMPLATE_1_EVT_TYPE_ENTRY,
    MODULE_TEMPLATE_1_EVT_TYPE_TEST_1,
    MODULE_TEMPLATE_1_EVT_TYPE_TEST_2,
    MODULE_TEMPLATE_1_EVT_TYPE_TEST_3,
    MODULE_TEMPLATE_1_EVT_TYPE_EXIT
}module_template_1_evt_type_t;


#ifdef __cplusplus
extern "C" {
#endif

    int module_template_1_task(void *pEvent);


#ifdef __cplusplus
}
#endif




#endif /* @end  __MODULE_TEMPLATE_1_H__*/