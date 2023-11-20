/**
 * @file      ddl_evt.h
 * @author:   Shubhendu B B
 * @date:     13/10/2023
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
    DDL_EVT_IDLE,
    DDL_EVT_RUN,
    DDL_EVT_EXIT,
}ddl_evt_type_t;

/*!
 * @typedef		ddl_evt_module_t
 * @brief       nextModule event handler function
 * @details
 *
 **/
typedef int(*ddl_evt_module_t)(void *pEvent);

typedef struct {
    ddl_evt_type_t eventType;
    ddl_evt_module_t nextModule;
    uint8_t *pDataBuff;
    size_t dataBuffLen;
}ddl_evt_t;


#ifdef __cplusplus
extern "C" {
#endif

    /*!
     * @fn         int ddl_evt_init(void)
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
    int ddl_evt_init(void);

    /*!
 * @fn         int ddl_evt_consumer(void)
 * @brief      Idle nextModule when nothing is running on
 *
 * @param[out] dest The memory area to copy to.
 * @param[in]  src  The memory area to copy from.
 * @exception
 * @return
 *
 * @details
 *
**/
    int ddl_evt_consumer(void);

    /*!
     * @fn         int ddl_evt_reg_module(ddl_evt_module_t moduleSignature)
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
    int ddl_evt_reg_module(ddl_evt_module_t moduleSignature);

    /*!
 * @fn         int ddl_evt_post(ddl_evt_module_t taskToPost)
 * @brief      Exposes functionality to post nextModule to be executed by the idle nextModule.
 *
 * @param[out] dest The memory area to copy to.
 * @param[in]  src  The memory area to copy from.
 * @exception
 * @return
 *
 * @details
 *
**/
    int ddl_evt_post(ddl_evt_t *pDdl_evtEvent);

#ifdef __cplusplus
}
#endif

#endif /* @end  __DDL_EVT_H__*/