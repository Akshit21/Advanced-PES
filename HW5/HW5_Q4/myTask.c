/*
 * myTask.c
 *
 *  Created on: Apr 7, 2018
 *      Author: akshit
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

#include "myTask.h"

/* Timer Handlers for 2 tasks */
TimerHandle_t tHandle[2];
extern QueueHandle_t task3Queue;
extern TaskHandle_t task3Handle;

void vTimerCallback(TimerHandle_t timerHandle)
{
    const uint32_t waitTime = pdMS_TO_TICKS(500);

    /* 2 Hz Timer handle */
    if(timerHandle == tHandle[0])
    {
        xTaskNotify(task3Handle,TOGGLE_LED,eSetBits);
    }

    /* 4 Hz Timer handle */
    if(timerHandle == tHandle[1])
    {
        NOTIFICATION_MSG_T notification;
        notification.my_ticks = xTaskGetTickCount();
        memcpy(notification.message, "Hello from Task 2!", sizeof(notification.message));

        if(xQueueSend(task3Queue, &notification, waitTime) == pdPASS)
        {
            /* Notify Task 3 with LOG_STRING */
            xTaskNotify(task3Handle,LOG_STRING,eSetBits);
        }
        else
        {
            UARTprintf("Task 2 QUEUE Error\n");
        }
    }
}

void led_blink_2hz(void *params)
{
    tHandle[0] = xTimerCreate("Timer2Hz", pdMS_TO_TICKS(500) , pdTRUE,  (void*)0, vTimerCallback);

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

void log_string_4hz(void *params)
{
    tHandle[1] = xTimerCreate("Timer4Hz", pdMS_TO_TICKS(250) , pdTRUE,  (void*)0, vTimerCallback);

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

void task3_handler(void *params)
{
    uint32_t led = GPIO_PIN_0;
    NOTIFICATION_MSG_T notification;
    uint32_t notify = 0;

    while(1)
    {
        if( xTaskNotifyWait( pdFALSE,      /* Don't clear bits on entry. */
                             ULONG_MAX,    /* Clear all bits on exit. */
                             &notify,      /* Stores the notified value. */
                             pdMS_TO_TICKS(500) == pdPASS )
        {
             if(notify & TOGGLE_LED)
             {
                 /* Toggle the LED */
                 led ^= (GPIO_PIN_0);
                 LEDWrite(0x0F, led);
             }

             if(notify & LOG_STRING)
             {
                 /* Get the Message from LOG_QUEUE*/
                 if(xQueueReceive(task3Queue, &notification, pdMS_TO_TICKS(500))
                 {
                     UARTprintf("NOTIFICATION MSG - %s NUM OF TICKS:%u\n", notification.message, notification.my_ticks);
                 }
                 else
                 {
                     UARTprintf("TASK 3 QUEUE RECV ERROR\n");
                 }
             }
        }
    }
}
