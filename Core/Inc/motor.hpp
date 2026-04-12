/**
 * @file motor.hpp
 * @brief Motor class composing PWM driver, encoder, and current sensor.
 *
 * The Motor class provides a unified interface for controlling a DC motor
 * and reading its feedback signals. Subsystems are injected via pointers
 * so they can be configured and tested independently.
 */

#ifndef MOTOR_HPP
#define MOTOR_HPP

#include "hbridge.hpp"
#include "encoder.hpp"

class Motor {

private:
    HBridge* hbridge_;
    Encoder* encoder_;
    // CurrentSensor* current_sensor_;  // TODO: add when hardware is ready

public:
    /**
     * @brief Construct a Motor instance.
     * @param hbridge  Pointer to an initialised HBridge driver.
     * @param encoder  Pointer to an initialised Encoder reader.
     */
    Motor(HBridge* hbridge, Encoder* encoder);

    /**
     * @brief Start the motor (enables PWM output).
     */
    void start(void);

    /**
     * @brief Stop the motor (disables PWM output).
     */
    void stop(void);

    /**
     * @brief Set motor duty cycle.
     * @param duty  -100 to +100. Sign sets direction.
     */
    void setDutyCycle(int8_t dutyCycle);

    /**
     * @brief Get current encoder position.
     * @return Tick count (signed).
     */
    int32_t getPosition(void) const;

    /**
     * @brief Reset encoder position to zero.
     */
    void resetPosition(void);
};

#endif // MOTOR_HPP
