/**
 * @file      app.h
 * @author     Shubhendu B B
 * @date       16/10/2024
 * @brief      Application interface header file
 * @details    This file contains function declarations for initializing,
 *             executing tasks, and deinitializing the application.
 *
 * @copyright  (c) 2024 Shubhendu B B. All rights reserved.
 **/

#ifndef __APP_H__
#define __APP_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief      Initializes the application.
 *
 * This function should be called before any other application functions.
 */
void app_init(void);

/**
 * @brief      Application task function.
 *
 * @param      pvParameters   Parameters for the task.
 * @return     int           Status code of the task execution.
 */
int app_task(void *pvParameters);

/**
 * @brief      Deinitializes the application.
 *
 * This function should be called to clean up resources allocated
 * during initialization or task execution.
 */
void app_deinit(void);

#ifdef __cplusplus
}
#endif

#endif /* __APP_H__ */
