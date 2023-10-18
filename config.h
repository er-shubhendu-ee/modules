/**
 * @file      config.h
 * @author:   Shubhendu B B
 * @date:     16/10/2023
 * @brief
 * @details
 *
 * @copyright
 *
**/

#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "ddl_log.h"

#define CONFIG_LOG_LEVEL DDL_LOG_LEVEL_VERBOSE 
#define CONFIG_MEMORY_MODEL_IS_DYNAMIC 1  // 0 = static memory, no malloc

#define CONFIG_DDL_EVT_QUEUE_SIZE 20
#define CONFIG_DDL_TASK_LIST_SIZE 10


#ifdef __cplusplus
extern "C" {
#endif




#ifdef __cplusplus
}
#endif




#endif /* @end  __CONFIG_H__*/