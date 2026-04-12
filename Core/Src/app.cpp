#include "stm32f4xx_hal.h"
#include "app.hpp"
#include "motor.hpp"

extern TIM_HandleTypeDef htim1;

volatile int32_t debug_position = 0;

// ---------------------------------------------------------------------------
// Subsystem instances
// ---------------------------------------------------------------------------
static Encoder g_encoder(GPIOB, GPIO_PIN_6,   // Channel A (EXTI)
                         GPIOB, GPIO_PIN_7);   // Channel B (input)

static HBridge g_hbridge(&htim1, TIM_CHANNEL_4,   // TIM1 CH4 on PA11
                         GPIOB, GPIO_PIN_5);       // DIR on PB5

static Motor g_motor(&g_hbridge, &g_encoder);

// ---------------------------------------------------------------------------
// C bridge: called from HAL_GPIO_EXTI_Callback()
// ---------------------------------------------------------------------------
extern "C" void encoder_exti_callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_7)
    {
        g_encoder.readEncoder();
    }
}

extern "C" void cpp_main(void)
{
    g_motor.start();
    g_motor.set_duty(50);  // 50% forward for testing

    while (1) {
        debug_position = g_motor.get_position();
    }
}
