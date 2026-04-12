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
    hbridge_->start();
}

void Motor::stop(void)
{
    hbridge_->stop();
}

void Motor::set_duty(int8_t duty)
{
    hbridge_->set_duty(duty);
}

int32_t Motor::get_position(void) const
{
    return encoder_->read_position();
}

void Motor::reset_position(void)
{
    encoder_->reset_position();
}
