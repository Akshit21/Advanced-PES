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

#include "driverlib/sysctl.h"
#include "driverlib/debug.h"
#include "driverlib/rom.h"
#include "driverlib/gpio.h"
#include "driverlib/rom_map.h"

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

    uint8_t led = GPIO_PIN_0 ;

    for(;;)
    {
       /* Led Count */
       static uint32_t count = 0;

       /* Toggle LED */
       led ^= GPIO_PIN_0;

       /* Write Led value */
       LEDWrite(0x0F, led);

       /* Print Count on Serial Terminal */
       UARTprintf("LED BLINK COUNT: %u\n",++count);
       SysCtlDelay((my_SYSTEM_CLOCK/ (1000 * 3))*500);
    }

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
