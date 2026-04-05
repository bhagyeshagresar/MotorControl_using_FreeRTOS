/**
 * @file encoder.hpp
 * @brief Quadrature encoder reader using GPIO EXTI interrupts on Channel A.
 *
 *
 * This class configures a GPIO EXTI interrupt on both rising and falling
 * edges of encoder Channel A. Channel B is read as a level to determine
 * direction. Position is updated atomically in the ISR context.
 *
 * Hardware setup (example for STM32F401RE Nucleo):
 *   - Channel A -> PB6  (configured as GPIO_EXTI6, rising + falling edge)
 *   - Channel B -> PB7  (configured as GPIO_Input, no interrupt needed)
 *
 * CubeMX configuration required:
 *   1. PB6: GPIO_EXTI6, GPIO mode = "External Interrupt Mode with Rising/Falling edge trigger detection"
 *   2. PB7: GPIO_Input (pull-up or pull-down per your encoder type)
 *   3. NVIC: Enable EXTI line6 interrupt
 */

#ifndef ENCODER_HPP
#define ENCODER_HPP

/**
 * @class Encoder
 * @brief Reads a quadrature encoder using EXTI on Channel A.
 *
 * Only Channel A triggers interrupts (both edges). Channel B is
 * sampled as a GPIO level at interrupt time to resolve direction.
 *
 * For a standard quadrature encoder:
 *   - Rising edge of A  + B is LOW  -> forward
 *   - Rising edge of A  + B is HIGH -> reverse
 *   - Falling edge of A + B is HIGH -> forward
 *   - Falling edge of A + B is LOW  -> reverse
 */
class Encoder {

	private:
		GPIO_TypeDef* chA_port_;
		uint16_t      chA_pin_;
		GPIO_TypeDef* chB_port_;
		uint16_t      chB_pin_;

		volatile int32_t position_;   // volatile: modified in ISR, read from tasks

	public:
		/**
		 * @brief Construct an Encoder instance.
		 * @param chA_port  GPIO port for Channel A (e.g. GPIOA)
		 * @param chA_pin   GPIO pin mask for Channel A (e.g. GPIO_PIN_0)
		 * @param chB_port  GPIO port for Channel B (e.g. GPIOA)
		 * @param chB_pin   GPIO pin mask for Channel B (e.g. GPIO_PIN_1)
		 */
		Encoder(GPIO_TypeDef* chA_port, uint16_t chA_pin,
				GPIO_TypeDef* chB_port, uint16_t chB_pin);


		/**
		 * @brief Read the current encoder position (thread/ISR-safe on Cortex-M).
		 * @return Current tick count (signed, forward = positive).
		 */
		int32_t read_position(void) const;

		/**
		 * @brief Call this from HAL_GPIO_EXTI_Callback() in main.c or stm32f4xx_it.c.
		 *        It delegates to the Encoder class internally.
		 * @param GPIO_Pin  The pin mask that triggered the interrupt.
		 */
		void read_encoder(void);

		/**
		 * @brief Reset position to zero.
		 */
		void reset_position(void);

};

#endif // ENCODER_HPP
