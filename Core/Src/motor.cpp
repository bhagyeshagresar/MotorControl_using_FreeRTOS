/**
 * @file motor.cpp
 * @brief Implementation of the Motor class.
 */

#include "motor.hpp"

Motor::Motor(HBridge* hbridge, Encoder* encoder)
    : hbridge_(hbridge),
      encoder_(encoder)
{
}

void Motor::start(void)
{
    hbridge_->startPwmOutput();
}

void Motor::stop(void)
{
    hbridge_->stopPwmOutput();
}

void Motor::setDutyCycle(int8_t dutyCycle)
{
    hbridge_->setPwmVal(dutyCycle);
}

int32_t Motor::getPosition(void) const
{
    return encoder_->readPosition();
}

void Motor::resetPosition(void)
{
    encoder_->resetPosition();
}
