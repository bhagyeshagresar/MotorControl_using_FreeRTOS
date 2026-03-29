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

## UI Development
The Raspberry Pi runs a backend using FastAPI Python framework.

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


