/**
 * @file: ddl_filter.h
 * @author:   Shubhendu B B
 * @date:     12/09/2023
 * @brief
 * @details
 *
 * @copyright
 *
 **/

#ifndef __DDL_FILTER_H__
#define __DDL_FILTER_H__

#include "stdio.h"

#ifdef __cplusplus
extern "C" {
#endif
    int ddl_filter_median(int* pInputArray, int inputArraySize);
    int ddl_filter_mode(int* pInputArray, int inputArraySize);

    /**
     * @brief: mean mode average filter
     *
     * @param pInputArray
     * @param inputArraySize
     * @return int
     */
    int ddl_filter_memo_avg(int* pInputArray, int inputArraySize);

#ifdef __cplusplus
}
#endif

#endif /* @end  __DDL_FILTER_H__*/