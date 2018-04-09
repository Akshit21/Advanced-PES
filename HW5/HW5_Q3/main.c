/*
 * main.c
 *
 * Akshit Shah
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#include "drivers/pinout.h"
#include "utils/uartstdio.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "priorities.h"
#include "timers.h"

#include "driverlib/sysctl.h"
#include "driverlib/debug.h"
#include "driverlib/rom.h"
#include "driverlib/gpio.h"
#include "driverlib/rom_map.h"

TimerHandle_t tHandle[2];

void vTimerCallback(TimerHandle_t timerHandle)
{
    /* 2 Hz Timer handle */
    if(timerHandle == tHandle[0])
    {
        static uint32_t led1 = GPIO_PIN_0;
        led1 ^= (GPIO_PIN_0);
        LEDWrite(0x0F, led1);

        //UARTprintf("TASK 2Hz\n");
    }

    /* 4 Hz Timer handle */
    if(timerHandle == tHandle[1])
    {
        static uint32_t led = GPIO_PIN_1;
        led ^= (GPIO_PIN_1);
        LEDWrite(0x0F, led);

        //UARTprintf("TASK 4Hz\n");
    }
}

static void led_blink_2hz(void *params)
{
    tHandle[0] = xTimerCreate("Timer2HZ", pdMS_TO_TICKS(500) , pdTRUE,  (void*)0, vTimerCallback);

    if(tHandle[0] == NULL)
    {
        while(1);
    }

    if((xTimerStart(tHandle[0], 0)) != pdTRUE)
    {
        while(1);
    }

    vTaskSuspend( NULL );
}

static void led_blink_4hz(void *params)
{
    tHandle[1] = xTimerCreate("Timer4HZ", pdMS_TO_TICKS(250) , pdTRUE,  (void*)0, vTimerCallback);

    if(tHandle[1] == NULL)
    {
        while(1);
    }

    if((xTimerStart(tHandle[1], 0)) != pdTRUE)
    {
        while(1);
    }

    vTaskSuspend( NULL );
}

int main(void)
{
    /* Initialize system clock to 120 MHz */
    uint32_t MY_SYSTEM_CLOCK;
    MY_SYSTEM_CLOCK = ROM_SysCtlClockFreqSet(
                               (SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN |
                                SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480),
                               SYSTEM_CLOCK);
    ASSERT(MY_SYSTEM_CLOCK == SYSTEM_CLOCK);

    /* GPIO Configuration */
    PinoutSet(false, false);

    /* UART Configuration */
    UARTStdioConfig(0, BAUD_RATE, SYSTEM_CLOCK);

    /* Print Header */
    UARTprintf("PROJECT FOR AKSHIT SHAH - 04/07/2018\n");

    /* Create the task 1 */
    if(xTaskCreate(led_blink_2hz, (const portCHAR *)"led_blink_2hz", MYTASKSTACKSIZE, NULL,
                   tskIDLE_PRIORITY + PRIO_MY_TASK1, NULL) != pdTRUE)
    {
        UARTprintf("Task 1 Creation Failed\n");
        return (1);
    }

    /* Create the task 2 */
    if(xTaskCreate(led_blink_4hz, (const portCHAR *)"led_blink_4hz", MYTASKSTACKSIZE, NULL,
                   tskIDLE_PRIORITY + PRIO_MY_TASK2, NULL) != pdTRUE)
    {
        UARTprintf("Task 2 Creation Failed\n");
        return 1;
    }

    UARTprintf("TASK CREATION SUCCESS\n");

    /* Start the Scedular */
    vTaskStartScheduler();

    return 0;
}

/*  ASSERT() Error function
 *
 *  failed ASSERTS() from driverlib/debug.h are executed in this function
 */
void __error__(char *pcFilename, uint32_t ui32Line)
{
    // Place a breakpoint here to capture errors until logging routine is finished
    while (1)
    {
    }
}
