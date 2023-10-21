/**
 * @file      event_root.h
 * @author:   Shubhendu B B
 * @date:     13/10/2023
 * @brief
 * @details
 *
 * @copyright
 *
**/

#ifndef __EVENT_ROOT_H__
#define __EVENT_ROOT_H__

#include <stdint.h>

typedef enum {
    EVENT_ROOT_EVT_TYPE_ENTRY = 0,
    EVENT_ROOT_EVT_TYPE_TEST_1,
    EVENT_ROOT_EVT_TYPE_TEST_2,
    EVENT_ROOT_EVT_TYPE_EXIT
}event_root_evt_type_t;

typedef struct {
    event_root_evt_type_t eventType;
    uint8_t *pDataBuff;
    size_t dataBuffLen;
}event_root_evt_t;

typedef int(*event_root_module_task_t)(void);

#ifdef __cplusplus
extern "C" {
#endif

    /*!
     * @fn         int event_root_init(void)
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
    int event_root_init(void);

    /*!
 * @fn         int event_root_idle(void)
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
    int event_root_idle(void);

    /*!
     * @fn         int event_root_reg_module(event_root_module_task_t moduleSignature)
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
    int event_root_reg_module(event_root_module_task_t moduleSignature);

    /*!
 * @fn         int event_root_post_evt(event_root_module_task_t taskToPost)
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
    int event_root_post_evt(event_root_evt_t *pEvent_rootEvent);


#ifdef __cplusplus
}
#endif




#endif /* @end  __EVENT_ROOT_H__*/