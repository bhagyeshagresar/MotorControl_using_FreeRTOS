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
    : chA_port(chA_port),
	  chA_pin(chA_pin),
	  chB_port(chB_port),
	  chB_pin(chB_pin),
      position(0)
{
}



int32_t Encoder::readPosition(void) const
{
    // On Cortex-M, a 32-bit aligned read is atomic, so this is safe
    // even when called from a FreeRTOS task while the ISR updates it.
    return position;
}

void Encoder::resetPosition(void)
{
    position = 0;
}


void Encoder::readEncoder(void)
{
    GPIO_PinState chA = HAL_GPIO_ReadPin(chA_port, chA_pin);
    GPIO_PinState chB = HAL_GPIO_ReadPin(chB_port, chB_pin);

    if (chA != chB)
    {
        position++;
    }
    else
    {
        position--;
    }
}


