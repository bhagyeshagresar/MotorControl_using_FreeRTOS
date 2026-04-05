/**
 * @file encoder.cpp
 * @brief Implementation of the Encoder class and C bridge functions.
 *
 * Place this file in Core/Src/
 */

#include "stm32f4xx_hal.h"   // HAL GPIO types — adjust if using a different family
#include "encoder.hpp"

// ---------------------------------------------------------------------------
// Encoder class implementation
// ---------------------------------------------------------------------------

Encoder::Encoder(GPIO_TypeDef* chA_port, uint16_t chA_pin,
                 GPIO_TypeDef* chB_port, uint16_t chB_pin)
    : chA_port_(chA_port),
      chA_pin_(chA_pin),
      chB_port_(chB_port),
      chB_pin_(chB_pin),
      position_(0)
{
}



int32_t Encoder::read_position(void) const
{
    // On Cortex-M, a 32-bit aligned read is atomic, so this is safe
    // even when called from a FreeRTOS task while the ISR updates it.
    return position_;
}

void Encoder::reset_position(void)
{
    position_ = 0;
}


void Encoder::read_encoder(void)
{
    GPIO_PinState chA = HAL_GPIO_ReadPin(chA_port_, chA_pin_);
    GPIO_PinState chB = HAL_GPIO_ReadPin(chB_port_, chB_pin_);

    if (chA != chB)
    {
        position_++;
    }
    else
    {
        position_--;
    }
}


