#include "stm32f4xx_hal.h"
#include "app.hpp"
#include "encoder.hpp"

volatile int32_t debug_position = 0;

// ---------------------------------------------------------------------------
// Global encoder instance — pins match CubeMX config (PB6 = Ch A, PB7 = Ch B)
// ---------------------------------------------------------------------------
static Encoder g_encoder(GPIOB, GPIO_PIN_6,   // Channel A (EXTI)
                         GPIOB, GPIO_PIN_7);   // Channel B (input)


// ---------------------------------------------------------------------------
// C bridge: called from HAL_GPIO_EXTI_Callback() in main.c
// ---------------------------------------------------------------------------
extern "C" void encoder_exti_callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_7)
    {
        g_encoder.read_encoder();
    }
}


extern "C" void cpp_main(void)
{
    while (1) {
    	debug_position = g_encoder.read_position();
    }
}

