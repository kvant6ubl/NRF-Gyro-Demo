#ifndef __SYSLOGGER_H__
#define __SYSLOGGER_H__

/**
 * @file syslogger.h
 * 
 * Additional compilation unit for logging, 
 * data conversation and service info.
 * 
 */ 

#include <mbed.h>
#include <stdio.h>
#include "ble/BLE.h"

#include <cstring>
#include <stdlib.h>
#include <malloc.h>

/* Maximum USB-TX buffer */
#define MAX_TX_BUFFER_SIZE 255

/* ITOA buffer for decimal to string conversation */
#define MAX_ITOA_BUFF 10

/* PC-USB connection Baudrate */
#define BAUDRATE 115200

/* Colors for debug info */
#define black "\033[0;30m"
#define red "\033[0;31m"
#define green "\033[0;32m"
#define yellow "\033[0;33m"
#define blue "\033[0;34m"
#define purple "\033[0;35m"
#define cyan "\033[0;36m"
#define white "\033[0;37m"
#define blackBold "\033[1;30m"
#define redBold "\033[1;31m"
#define greenBold "\033[1;32m"
#define yellowBold "\033[1;33m"
#define blueBold "\033[1;34m"
#define purpleBold "\033[1;35m"
#define cyanBold "\033[1;36m"
#define whiteBold "\033[1;37m"

/* BufferedSerial object */
static BufferedSerial debugOut(USBTX, USBRX, BAUDRATE);

/* Mutex object for used to synchronize the execution of threads */
static Mutex debugMutex;

/* Macros for dec to hex conversation */
#define TO_HEX(i) (i <= 9 ? '0' + i : 'A' - 10 + i)

/**
 * @brief Decimal to hex-string conversation
 * 
 * @param number Decimal <int> number
 *
 * @return Pointer to hex-char array
 */
inline char *dec_to_hex_str(int number)
{
    static char hex_str[5];
    if (hex_str == NULL)
        return NULL;

    if (number <= 0xFFFF)
    {
        hex_str[0] = TO_HEX(((number & 0xF000) >> 12));
        hex_str[1] = TO_HEX(((number & 0x0F00) >> 8));
        hex_str[2] = TO_HEX(((number & 0x00F0) >> 4));
        hex_str[3] = TO_HEX((number & 0x000F));
        hex_str[4] = '\0';
    }
    return hex_str;
}

/**
 * @brief Decimal to string conversation
 * 
 * @param number Decimal <int> number
 *
 * @return Pointer to char array
 */
inline char *dec_to_str(int num)
{
    char str_num[MAX_ITOA_BUFF];
    if (str_num == NULL)
        return NULL;

    itoa(num, str_num, 10);
    return str_num;
}

/**
 * @brief Simple log tool to terminal decimal number 
 * 
 * Representing decimal number with runtime ms addition
 * 
 * @param number Decimal <int> number
 *
 * @return None
 */
inline void nrf_dec_print(int number)
{
    char payload[MAX_TX_BUFFER_SIZE] = "I  (";
    if (payload == NULL)
        return;

    uint64_t ms_count = get_ms_count();

    char ms_count_str[20];
    if (ms_count_str == NULL)
        return;

    itoa(ms_count, ms_count_str, 10);
    strcat(ms_count_str, ")\t");
    strcat(payload, ms_count_str);

    char numb_str[20];
    if (numb_str == NULL)
        return;

    itoa(number, numb_str, 10);

    strcat(payload, numb_str);
    strcat(payload, "\r");
    strcat(payload, "\n");

    debugMutex.lock();
    debugOut.write(whiteBold, 5);
    debugOut.write(payload, strlen(payload));
    debugMutex.unlock();
}

/**
 * @brief Simple log tool to the terminal appropriate message 
 * 
 * Representing message with runtime ms addition
 * 
 * @param message Pointer for char message array
 *
 * @return None
 */
static void nrf_fast_log(const char *message)
{
    if (strlen(message) > MAX_TX_BUFFER_SIZE)
        return;

    // char payload[ strlen(message) + 8 ] = "I  (";
    char payload[MAX_TX_BUFFER_SIZE] = "I  (";

    if (payload == NULL)
        return;

    uint64_t ms_count = get_ms_count();

    char ms_count_str[20];
    if (ms_count_str == NULL)
        return;

    itoa(ms_count, ms_count_str, 10);
    strcat(ms_count_str, ")\t");

    strcat(payload, ms_count_str);
    strcat(payload, message);
    strcat(payload, "\r");
    strcat(payload, "\n");

    debugMutex.lock();
    debugOut.write(payload, strlen(payload));
    debugMutex.unlock();
}

/**
 * @brief Print service info
 * 
 */
inline void BOARD_INFO(void)
{
    debugMutex.lock();
    debugOut.write("\r\n\n\n", 6);
    debugOut.write(whiteBold, 8);
    debugOut.write("* * * * * * * * * * * * * * * * * * * * * * * * * * * *\r\n", 58);
    debugOut.write("* Board     : Nordic nRF52-DK                         *\r\n", 58);
    debugOut.write("* Chip      : NRF52832 64MHz, 64KB RAM, 512KB Flash   *\r\n", 58);
    debugOut.write("* Processor : Cortex-M4 r0p1, Little endian           *\r\n", 58);
    debugOut.write("* * * * * * * * * * * * * * * * * * * * * * * * * * * *\r\n", 58);
    debugOut.write("\r\n\n", 4);
    debugMutex.unlock();
}

/**
 * @brief Simple log to the terminal appropriate message 
 * 
 * Representing information [I] message with runtime ms addition
 * 
 * @param message Pointer for char message array
 *
 * @return None
 */
inline void LOGI(const char *message)
{
    debugMutex.lock();
    debugOut.write(blueBold, 8);
    debugMutex.unlock();
    nrf_fast_log(message);
}

/**
 * @brief Simple log to the terminal appropriate message 
 * 
 * Representing warning [W] message with runtime ms addition
 * 
 * @param message Pointer for char message array
 *
 * @return None
 */
inline void LOGW(const char *message)
{
    debugMutex.lock();
    debugOut.write(yellow, 8);
    debugMutex.unlock();
    nrf_fast_log(message);
}

/**
 * @brief Simple log to the terminal appropriate message 
 * 
 * Representing error [E] message with runtime ms addition
 * 
 * @param message Pointer for char message array
 *
 * @return None
 */
inline void LOGE(const char *message)
{
    debugMutex.lock();
    debugOut.write(redBold, 8);
    debugMutex.unlock();
    nrf_fast_log(message);
}

/**
 * @brief Printing BLE error while handling event queue 
 *  
 * @param error Error codes for the BLE API
 * @param msg   User message 
 * 
 * @return None
 */
inline void PRINT_BLE_ERROR(ble_error_t error, const char *msg)
{
    switch (error)
    {
    case BLE_ERROR_NONE:
        LOGE("BLE_ERROR_NONE: No error");
        break;
    case BLE_ERROR_BUFFER_OVERFLOW:
        LOGE("BLE_ERROR_BUFFER_OVERFLOW: The requested action would cause a buffer overflow and has been aborted");
        break;
    case BLE_ERROR_NOT_IMPLEMENTED:
        LOGE("BLE_ERROR_NOT_IMPLEMENTED: Requested a feature that isn't yet implement or isn't supported by the target HW");
        break;
    case BLE_ERROR_PARAM_OUT_OF_RANGE:
        LOGE("BLE_ERROR_PARAM_OUT_OF_RANGE: One of the supplied parameters is outside the valid range");
        break;
    case BLE_ERROR_INVALID_PARAM:
        LOGE("BLE_ERROR_INVALID_PARAM: One of the supplied parameters is invalid");
        break;
    case BLE_STACK_BUSY:
        LOGE("BLE_STACK_BUSY: The stack is busy");
        break;
    case BLE_ERROR_INVALID_STATE:
        LOGE("BLE_ERROR_INVALID_STATE: Invalid state");
        break;
    case BLE_ERROR_NO_MEM:
        LOGE("BLE_ERROR_NO_MEM: Out of Memory");
        break;
    case BLE_ERROR_OPERATION_NOT_PERMITTED:
        LOGE("BLE_ERROR_OPERATION_NOT_PERMITTED");
        break;
    case BLE_ERROR_INITIALIZATION_INCOMPLETE:
        LOGE("BLE_ERROR_INITIALIZATION_INCOMPLETE");
        break;
    case BLE_ERROR_ALREADY_INITIALIZED:
        LOGE("BLE_ERROR_ALREADY_INITIALIZED");
        break;
    case BLE_ERROR_UNSPECIFIED:
        LOGE("BLE_ERROR_UNSPECIFIED: Unknown error");
        break;
    case BLE_ERROR_INTERNAL_STACK_FAILURE:
        LOGE("BLE_ERROR_INTERNAL_STACK_FAILURE: internal stack faillure");
        break;
    case BLE_ERROR_NOT_FOUND:
        LOGE("BLE_ERROR_NOT_FOUND: not found BLE device");
        break;
    default:
        LOGE("\r\n");
    }
    LOGE("\r\n");
}

#endif