/**
 * @file uart_printf.cpp
 * @brief Lightweight printf over UART. No malloc/heap needed.
 */

#include "uart_printf.hpp"
#include <cstdarg>

static UART_HandleTypeDef* uartHandle = nullptr;

static void uartPutchar(char ch)
{
    HAL_UART_Transmit(uartHandle, (uint8_t*)&ch, 1, HAL_MAX_DELAY);
}

void uartPrintfInit(UART_HandleTypeDef* huart)
{
    uartHandle = huart;
}

void uartPrintf(const char* fmt, ...)
{
    va_list vargs;
    va_start(vargs, fmt);

    while (*fmt)
    {
        if (*fmt == '%')
        {
            fmt++;
            switch (*fmt)
            {
                case '\0':
                    uartPutchar('%');
                    goto end;
                case '%':
                    uartPutchar('%');
                    break;
                case 's':
                {
                    const char* s = va_arg(vargs, const char*);
                    while (*s)
                    {
                        uartPutchar(*s);
                        s++;
                    }
                    break;
                }
                case 'd':
                {
                    int value = va_arg(vargs, int);
                    unsigned magnitude = value;
                    if (value < 0)
                    {
                        uartPutchar('-');
                        magnitude = -magnitude;
                    }

                    unsigned divisor = 1;
                    while (magnitude / divisor > 9)
                        divisor *= 10;

                    while (divisor > 0)
                    {
                        uartPutchar('0' + magnitude / divisor);
                        magnitude %= divisor;
                        divisor /= 10;
                    }
                    break;
                }
                case 'x':
                {
                    unsigned value = va_arg(vargs, unsigned);
                    for (int i = 7; i >= 0; i--)
                    {
                        unsigned nibble = (value >> (i * 4)) & 0xf;
                        uartPutchar("0123456789abcdef"[nibble]);
                    }
                    break;
                }
            }
        }
        else
        {
            uartPutchar(*fmt);
        }

        fmt++;
    }

end:
    va_end(vargs);
}
