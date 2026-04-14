#include "stm32f4xx_hal.h"
#include "app.hpp"
#include "motor.hpp"
#include "uart_printf.hpp"

extern TIM_HandleTypeDef htim1;
extern UART_HandleTypeDef huart2;

volatile int32_t debugPosition = 0;
volatile int32_t debugTarget = 0;
volatile int32_t debugError = 0;
volatile float   debugU = 0;
static int32_t pos = 0;
static uint32_t prevT = 0;
static float eprev = 0;
static uint32_t printCounter = 0;
static float eint = 0;

// ---------------------------------------------------------------------------
// Subsystem instances
// ---------------------------------------------------------------------------
static Encoder g_encoder(GPIOB, GPIO_PIN_6,   // Channel A (EXTI)
                         GPIOB, GPIO_PIN_7);   // Channel B (input)

static HBridge g_hbridge(&htim1, TIM_CHANNEL_4,   // TIM1 CH4 on PA11
                         GPIOB, GPIO_PIN_5);       // DIR on PB5

static Motor g_motor(&g_hbridge, &g_encoder);

// Microseconds from SysTick (no extra timer needed)
uint32_t getMicros(void)
{
    uint32_t ms = HAL_GetTick();
    uint32_t ticks = SysTick->VAL;
    uint32_t load = SysTick->LOAD + 1;
    return (ms * 1000) + ((load - ticks) * 1000) / load;
}

// ---------------------------------------------------------------------------
// C bridge: called from HAL_GPIO_EXTI_Callback()
// ---------------------------------------------------------------------------
extern "C" void encoder_exti_callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_7)
    {
        g_encoder.readEncoder();
    }
}

extern "C" void cpp_main(void)
{
    uartPrintfInit(&huart2);
    g_motor.start();
    prevT = getMicros(); //fill prevT so that the deltaT for first iteration is not a huge bogus value
    while (1) {

    	//set target position
    	int32_t target = 5000;

    	//PID constants
    	float kp = 5;
    	float ki = 0.2;
    	float kd = 0.5;

    	//time difference
    	uint32_t currT = getMicros();
    	float deltaT = ((float)(currT - prevT) / 1.0e6);
    	prevT = currT;

    	//get the current position
    	pos = g_motor.getPosition();

    	//compute the error
    	int32_t e = target - pos;

    	//skip if deltaT is zero (loop ran faster than timer resolution)
    	if (deltaT == 0) continue;

    	//compute the derivative
    	float edert = (e - eprev) / (deltaT);

    	//compute the integral
    	eint = eint + (e * deltaT);

    	//anti-windup: clamp integral term
    	if (eint > 100) eint = 100;
    	if (eint < -100) eint = -100;

    	//compute the control signal
    	float u = (kp * e) + (kd * edert) + (ki * eint);

    	//clamp output
    	if (u > 100) u = 100;
    	if (u < -100) u = -100;

    	g_motor.setDutyCycle((int8_t)u);

    	//store the previous error
    	eprev = e;

    	//update debug variables for live expressions
    	debugPosition = pos;
    	debugTarget = target;
    	debugError = e;
    	debugU = u;

    	//print every 10th iteration to avoid slowing the loop
    	if (++printCounter >= 10)
    	{
    		printCounter = 0;
    		uartPrintf("%d,%d,%d,%d\n", (int)target, (int)pos, (int)e, (int)u);
    	}

    }
}
