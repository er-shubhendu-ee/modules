/**
 * @file      data_process.h
 * @author:   Shubhendu B B
 * @date:     26/12/2023
 * @brief
 * @details
 *
 * @copyright
 *
**/


#ifndef __DATA_PROCESS_H__
#define __DATA_PROCESS_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>




#ifdef __cplusplus
extern "C" {
#endif


    int data_process_print_dataset(float *pDataSet, int sampleCount);
    int data_process_extract_x(float *pDataSet, float *pBuff_x, int sampleCount);
    int data_process_extract_y(float *pDataSet, float *pBuff_y, int sampleCount);
    float data_process_predict_y_value(float *pDataset, int sampleCount, float inputValue);


#ifdef __cplusplus
}
#endif




#endif /* @end  __DATA_PROCESS_H__*/