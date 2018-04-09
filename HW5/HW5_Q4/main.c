/*
 * main.c
 *
 * Akshit Shah
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

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

#include "main.h"
#include "myTask.h"

/* Handler declarations */
QueueHandle_t task3Queue;
TaskHandle_t task3Handle;

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

    /* Create a Message Queue */
    task3Queue = xQueueCreate(TASK3_QUEUE_LENGTH, sizeof(NOTIFICATION_MSG_T));

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
    if(xTaskCreate(log_string_4hz, (const portCHAR *)"log_string_4hz", MYTASKSTACKSIZE, NULL,
                   tskIDLE_PRIORITY + PRIO_MY_TASK2, NULL) != pdTRUE)
    {
        UARTprintf("Task 2 Creation Failed\n");
        return 1;
    }

    if(xTaskCreate(task3_handler, (const portCHAR *)"task3_handler", MYTASKSTACKSIZE, NULL,
                   tskIDLE_PRIORITY + PRIO_MY_TASK3, &task3Handle) != pdTRUE)
    {
        UARTprintf("Task 3 Creation Failed\n");
        return 1;
    }

    UARTprintf("TASK CREATION SUCCESS\n");

    /* Start the Scheduler */
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
