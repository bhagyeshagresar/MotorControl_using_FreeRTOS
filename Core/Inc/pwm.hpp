/**
 * @file pwm.hpp
 *
 * Hardware setup (STM32F401RE Nucleo):
 *   - PWM  -> PA11 (TIM1_CH4, 10 kHz, ARR = 8399)
 *   - DIR  -> PB5  (GPIO Output)
 *
 * CubeMX configuration required:
 *   1. PA11: TIM1_CH4, PWM Generation CH4, ARR = 100, PSC = 0
 *   2. PB5:  GPIO_Output, no pull, push-pull
 *
 *  Created on: Apr 11, 2026
 *      Author: bhagy
 */



