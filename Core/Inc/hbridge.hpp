/**
 * @file hbridge.hpp
 * @brief H-Bridge driver for DC motor using TIM + direction GPIO.
 *
 * Hardware setup (STM32F401RE Nucleo):
 *   - PWM  -> PA11 (TIM1_CH4, 10 kHz, ARR = 8399)
 *   - DIR  -> PB5  (GPIO Output)
 *
 * CubeMX configuration required:
 *   1. PA11: TIM1_CH4, PWM Generation CH4
 *   2. PB5:  GPIO_Output, no pull, push-pull
 */

#ifndef HBRIDGE_HPP
#define HBRIDGE_HPP

#include "stm32f4xx_hal.h"

class HBridge {

private:
    TIM_HandleTypeDef* htim;
    uint32_t           channel;
    GPIO_TypeDef*      dir_port;
    uint16_t           dir_pin;

public:
    /**
     * @brief Construct an HBridge driver.
     * @param htim      Pointer to the HAL timer handle (e.g. &htim1)
     * @param channel   Timer channel (e.g. TIM_CHANNEL_4)
     * @param dir_port  GPIO port for direction pin (e.g. GPIOB)
     * @param dir_pin   GPIO pin mask for direction (e.g. GPIO_PIN_5)
     */
    HBridge(TIM_HandleTypeDef* htim, uint32_t channel,
            GPIO_TypeDef* dir_port, uint16_t dir_pin);

    /**
     * @brief Start PWM output. Call once before set_duty().
     */
    void startPwmOutput(void);

    /**
     * @brief Stop PWM output.
     */
    void stopPwmOutput(void);

    /**
     * @brief Set motor duty cycle.
     * @param pwmVal  -100 to +100. Sign controls direction pin.
     *              Positive = forward (DIR low), Negative = reverse (DIR high).
     *     Hardware setup (STM32F401RE Nucleo):
	 *   - PWM  -> PA11 (TIM1_CH4, 10 kHz, ARR = 8399)
	 *   - DIR  -> PB5  (GPIO Output)
     */
    void setPwmVal(int8_t pwmVal);
};

#endif // HBRIDGE_HPP
