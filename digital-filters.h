/**
 * @file: digital-filters.h
 * @author:   Shubhendu B B
 * @date:     12/09/2023
 * @brief
 * @details
 *
 * @copyright
 *
 **/

#ifndef __DIGITAL_FILTERS_H__
#define __DIGITAL_FILTERS_H__

#include "stdio.h"

#ifdef __cplusplus
extern "C" {
#endif

int digital_filter_median(int *pInputArray, int inputArraySize);
int digital_filter_mode(int *pInputArray, int inputArraySize);

#ifdef __cplusplus
}
#endif

#endif /* @end  __DIGITAL_FILTERS_H__*/