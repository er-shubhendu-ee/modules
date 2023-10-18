/**
 * @file      app.h
 * @author:   Shubhendu B B
 * @date:     13/10/2023
 * @brief
 * @details
 *
 * @copyright
 *
**/

#ifndef __APP_H__
#define __APP_H__

#include <stdint.h>

typedef enum {
    APP_EVT_TYPE_ENTRY = 0,
    APP_EVT_TYPE_TEST_1,
    APP_EVT_TYPE_TEST_2,
    APP_EVT_TYPE_EXIT
}app_evt_type_t;

typedef struct {
    app_evt_type_t eventType;
    uint8_t *pDataBuff;
    size_t dataBuffLen;
}app_evt_t;

typedef int(*app_module_task_t)(void);

#ifdef __cplusplus
extern "C" {
#endif

    /*!
     * @fn         int app_init(void)
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
    int app_init(void);

    /*!
 * @fn         int app_idle(void)
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
    int app_idle(void);

    /*!
     * @fn         int app_reg_module(app_module_task_t moduleSignature)
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
    int app_reg_module(app_module_task_t moduleSignature);

    /*!
 * @fn         int app_post_evt(app_module_task_t taskToPost)
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
    int app_post_evt(app_evt_t *pAppEvent);


#ifdef __cplusplus
}
#endif




#endif /* @end  __APP_H__*/