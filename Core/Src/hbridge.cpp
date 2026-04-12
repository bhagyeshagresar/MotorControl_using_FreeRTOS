/**
 * @file hbridge.cpp
 * @brief Implementation of the HBridge driver class.
 */

#include "hbridge.hpp"

HBridge::HBridge(TIM_HandleTypeDef* htim, uint32_t channel,
                 GPIO_TypeDef* dir_port, uint16_t dir_pin)
    : htim_(htim),
      channel_(channel),
      dir_port_(dir_port),
      dir_pin_(dir_pin)
{
}

void HBridge::start(void)
{
    HAL_TIM_PWM_Start(htim_, channel_);
}

void HBridge::stop(void)
{
    HAL_TIM_PWM_Stop(htim_, channel_);
}

void HBridge::set_duty(int8_t duty)
{
    if (duty > 100)  duty = 100;
    if (duty < -100) duty = -100;

    if (duty >= 0)
    {
        HAL_GPIO_WritePin(dir_port_, dir_pin_, GPIO_PIN_RESET);
    }
    else
    {
        HAL_GPIO_WritePin(dir_port_, dir_pin_, GPIO_PIN_SET);
        duty = -duty;
    }

    uint32_t arr = __HAL_TIM_GET_AUTORELOAD(htim_);
    uint32_t compare = (arr * duty) / 100;
    __HAL_TIM_SET_COMPARE(htim_, channel_, compare);
}
