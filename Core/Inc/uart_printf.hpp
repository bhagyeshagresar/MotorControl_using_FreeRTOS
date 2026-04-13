/**
 * @file uart_printf.hpp
 * @brief Lightweight printf over UART. No malloc/heap needed.
 */

#ifndef UART_PRINTF_HPP
#define UART_PRINTF_HPP

#include "stm32f4xx_hal.h"

/**
 * @brief Initialise with the UART handle to use for output.
 * @param huart  Pointer to HAL UART handle (e.g. &huart2)
 */
void uartPrintfInit(UART_HandleTypeDef* huart);

/**
 * @brief Lightweight printf that sends formatted text over UART.
 *        Supports: %d (int), %s (string), %x (hex), %% (literal %).
 */
void uartPrintf(const char* fmt, ...);

#endif // UART_PRINTF_HPP
