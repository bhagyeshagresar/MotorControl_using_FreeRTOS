

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
