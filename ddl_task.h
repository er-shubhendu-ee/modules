/**
 * @file      ddl_task.h
 * @author:   Shubhendu B B
 * @date:     18/10/2023
 * @brief
 * @details
 *
 * @copyright
 *
**/

#ifndef __APP_H__
#define __APP_H__

#include <stdint.h>

#include "ddl_evt.h"

typedef int(*ddl_task_t)(void);

#ifdef __cplusplus
extern "C" {
#endif

    /*!
     * @fn         int ddl_task_init(void)
     * @brief      Copies bytes from a source memory area to a destination memory area,
     *             where both areas may not overlap.
     *
     * @param[out] dest The memory area to copy to.
     * @param[in]  src  The memory area to copy from.
     * @exception
     * @return
     *
     * @details
     *
    **/
    int ddl_task_init(void);

    /*!
 * @fn         int ddl_task_idle(void)
 * @brief      Idle task when nothing is running on
 *
 * @param[out] dest The memory area to copy to.
 * @param[in]  src  The memory area to copy from.
 * @exception
 * @return
 *
 * @details
 *
**/
    int ddl_task_idle(void);

    /*!
     * @fn         int ddl_task_reg_module(ddl_task_t moduleSignature)
     * @brief      Copies bytes from a source memory area to a destination memory area,
     *             where both areas may not overlap.
     *
     * @param[out] dest The memory area to copy to.
     * @param[in]  src  The memory area to copy from.
     * @exception
     * @return
     *
     * @details
     *
    **/
    int ddl_task_reg_module(ddl_task_t moduleSignature);

    /*!
 * @fn         int ddl_task_post_evt(ddl_task_t taskToPost)
 * @brief      Exposes functionality to post task to be executed by the idle task.
 *
 * @param[out] dest The memory area to copy to.
 * @param[in]  src  The memory area to copy from.
 * @exception
 * @return
 *
 * @details
 *
**/
    int ddl_task_post_evt(ddl_evt_t *pAppEvent);


#ifdef __cplusplus
}
#endif




#endif /* @end  __APP_H__*/