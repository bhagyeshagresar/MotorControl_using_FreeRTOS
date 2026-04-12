/**
 * @file hbridge.cpp
 * @brief Implementation of the HBridge driver class.
 */

#include "hbridge.hpp"

HBridge::HBridge(TIM_HandleTypeDef* htim, uint32_t channel,
                 GPIO_TypeDef* dir_port, uint16_t dir_pin)
    : htim(htim),
      channel(channel),
      dir_port(dir_port),
      dir_pin(dir_pin)
{
}

void HBridge::startPwmOutput(void)
{
    HAL_TIM_PWM_Start(htim, channel);
}

void HBridge::stopPwmOutput(void)
{
    HAL_TIM_PWM_Stop(htim, channel);
}

void HBridge::setPwmVal(int8_t pwmVal)
{
    if (pwmVal > 100)  pwmVal = 100;
    if (pwmVal < -100) pwmVal = -100;

    if (pwmVal >= 0)
    {
        HAL_GPIO_WritePin(dir_port, dir_pin, GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(dir_port, dir_pin, GPIO_PIN_RESET);
        pwmVal = -pwmVal;
    }

    uint32_t arr = __HAL_TIM_GET_AUTORELOAD(htim); //get the ARR value(100 in this case)
    uint32_t compare = (arr * pwmVal) / 100; //calculate the compare value
    __HAL_TIM_SET_COMPARE(htim, channel, compare); //set the tim->ccr register for pwm output
}
