/**
 * @file      ddl_serial.h
 * @brief     Header file for serial communication handling.
 * @details   This file defines the API for serial communication events,
 *             including sending and receiving data as well as handling
 *             event callbacks.
 *
 * @note      Ensure proper initialization before using serial functions.
 */

#ifndef __DDL_SERIAL_H__
#define __DDL_SERIAL_H__

#include <stddef.h>  // For size_t
#include <stdint.h>  // For uint8_t

/**
 * @enum ddl_SerialEvent_t
 * @brief Enumeration of serial events.
 */
typedef enum {
    DDL_SERIAL_EVENT_NONE = 0,        ///< No event
    DDL_SERIAL_EVENT_RX,              ///< Data received
    DDL_SERIAL_EVENT_TX,              ///< Data transmitted
    DDL_SERIAL_EVENT_QUEUE_FULL_RX,   ///< RX queue is full
    DDL_SERIAL_EVENT_QUEUE_EMPTY_RX,  ///< RX queue is empty
    DDL_SERIAL_EVENT_QUEUE_EMPTY_TX,  ///< TX queue is empty
    DDL_SERIAL_EVENT_QUEUE_FULL_TX    ///< TX queue is full
} ddl_SerialEvent_t;

/**
 * @brief Function pointer type for event callbacks.
 * @param event The event that occurred.
 */
typedef void (*ddl_serial_event_cb_t)(ddl_SerialEvent_t event);

/**
 * @brief Register a callback function for serial events.
 * @param[in] callback Pointer to the callback function.
 *                    This function will be called when a serial event occurs.
 */
void ddl_serial_register_cb(ddl_serial_event_cb_t callback);

/**
 * @brief Initialize the serial interface.
 * @return 0 on success, negative value on failure.
 * @details This function sets up the serial interface for communication.
 */
int ddl_serial_init(void);

/**
 * @brief Deinitialize the serial interface.
 * @return 0 on success, negative value on failure.
 * @details This function cleans up resources used by the serial interface.
 */
int ddl_serial_deinit(void);

/**
 * @brief Send data through the serial interface.
 * @param[in] pDataBuff Pointer to the data buffer to send.
 * @param[in] dataBuffLen Length of the data buffer.
 * @return 0 on success, negative value on error.
 * @details This function enqueues data for transmission over the serial interface.
 */
int ddl_serial_send(uint8_t* pDataBuff, size_t dataBuffLen, size_t* pBytesSentCount);

/**
 * @brief Receive data from the serial interface.
 * @param[out] pDataBuff Pointer to the buffer where received data will be stored.
 * @param[in] dataBuffLen Length of the buffer.
 * @return 0 on success, negative value on error.
 * @details This function retrieves data received from the serial interface and stores it in the
 * provided buffer.
 */
int ddl_serial_recv(uint8_t* pDataBuff, size_t dataBuffLen, size_t* pBytesReceivedCount);

/**
 * @brief Task for handling serial data processing.
 * @param[in] pvParameters Parameters for the task, can be NULL.
 * @details This function processes incoming and outgoing serial data.
 */
void ddl_serial_task(void* pvParameters);

/**
 * @brief Transmit a single byte through the serial port.
 * @param[in] value The byte value to transmit.
 * @return 0 on success, negative value on error.
 * @details This function sends a single byte through the serial interface.
 */
int ddl_serial_port_tx_byte(uint8_t value);

/**
 * @brief Receive a single byte from the serial port.
 * @param[out] pValue Pointer to store the received byte.
 * @return 0 on success, negative value on error.
 * @details This function retrieves a single byte from the serial interface.
 */
int ddl_serial_port_rx_byte(uint8_t* pValue);

#endif /* __DDL_SERIAL_H__ */
