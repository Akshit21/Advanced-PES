/*
 * myTask.h
 *
 *  Created on: Apr 7, 2018
 *      Author: akshit
 */

#ifndef MYTASK_H_
#define MYTASK_H_

#define TOGGLE_LED  (0x01)
#define LOG_STRING  (0x02)

#define TASK3_QUEUE_LENGTH (0x0F)

typedef struct
{
    uint32_t my_ticks;
    char message[50];
} NOTIFICATION_MSG_T;

/* Task Handlers */
void led_blink_2hz(void *params);
void log_string_4hz(void *params);
void task3_handler(void *params);

#endif /* MYTASK_H_ */
