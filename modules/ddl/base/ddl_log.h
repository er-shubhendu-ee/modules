#include <stdio.h>

#include "ddl_base.h"

#define LOG_LEVEL_NONE 0
#define LOG_LEVEL_ERROR 1
#define LOG_LEVEL_INFO 2
#define LOG_LEVEL_DEBUG 3
#define LOG_LEVEL_VERBOSE 4

#define DDL_LOGD(TAG, fmt, ...) \
    printf("\n\033[37m" TAG " : %s: %d: " fmt "\033[0m\n", __func__,__LINE__, ##__VA_ARGS__);
#define DDL_LOGI(TAG, fmt, ...) \
    printf("\n\033[32m" TAG " : %s: %d: " fmt "\033[0m\n", __func__,__LINE__, ##__VA_ARGS__);
#define DDL_LOGW(TAG, fmt, ...) \
    printf("\n\033[33m" TAG " : %s: %d: " fmt "\033[0m\n", __func__,__LINE__, ##__VA_ARGS__);
#define DDL_LOGE(TAG, fmt, ...) \
    printf("\n\033[31m" TAG " : %s: %d: " fmt "\033[0m\n", __func__,__LINE__, ##__VA_ARGS__);