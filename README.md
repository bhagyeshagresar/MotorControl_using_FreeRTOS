# Motor Control Using FreeRTOS
This project is an attempt to build a complete motion control system to support control of a brushed DC motor to start. It will include an interactive UI and plotting features, communication over UART to the STM32F401RE and firmware for motor control.

## Components used in this project:
1) Raspberry Pi SBC (Raspberry Pi 4 Model B)
2) STM32 MCU
3) INA219 Current Sensor
4) Brushed DC motor with encoder
5) H-bridge


## Software Architecture

![System Architecture](motor_control_architecture.png)

### Using Cpp source file
The application code is written in cpp. The cpp_main() is executed for all the application logic. In order to avoid C++ mangling, the keyword ``` extern "C" ``` is used.
```#ifdef __cplusplus``` is used so that the C compiler ignores C++-only syntax (like class, extern "C", constructors, and member functions) that would cause compilation errors, since C doesn't understand any of these constructs.

## UI Development
The Raspberry Pi runs a backend using FastAPI Python framework.



## Low-level software configuration

### 1: Motor Encoder Configuration
The channel A and B of the quadrature are configured to the GPIO Pins PB6 and PB7 respectively. In CubeMX, PB6 GPIO mode is set to generate an interrupt on rising or falling edge(x2 decoding). Make sure to enable the EXTI line[9:5] in System Core NVIC settings. External Interrupt/Event Controller is a hardware block inside the STM32 that watches GPIO pins and generates an interrupt when it detects an edge (rising, falling, or both).

### 2: PWM Configuration
Basic PWM Generation using Timer
Configured Timer1 CH4 to generate a user defined duty cycle. The prescaler of the timer is set to 1MHz and the ARR register(max rollover counter) is set to 100.

Tpwm = ARR/Prescaler = 100/1MHZ = 100 microseconds. The frequency of the PWM is 10 kHZ.


## FreeRTOS Configuration
This project includes the FreeRTOS kernel in the ThirdParty folder and does not use CMSIS RTOS API. This project does not use sysmem.c because FreeRTOS has its own heap management(make sure to check ThirdParty folder to not exclude from build and exclude sysmem.c from build). Also only heap_4.c is used.

```
    //Modified the preprocessor directive for gcc compiler in FreeRTOSConfig.h

    #if defined(__ICCARM__) || defined(__GNUC__) || defined(__CC_ARM)
        #include <stdint.h>
        extern uint32_t SystemCoreClock;
    #endif

```

FreeRTOS uses ARM Cortex-M processor's internal systick timer as its time base(RTOS ticking). HAL also uses the systick timer by default for HAL delay functions.
In order to prevent overriding each other's settings, the HAL is configured to use the TIM5 general purpose timer using CubeMX Sys configuration settings.


