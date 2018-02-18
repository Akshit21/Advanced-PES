/*****************************************************************************
* Copyright (C) 2017 by Akshit Shah
*
* Redistribution, modification or use of this software in source or binary
* forms is permitted as long as the files maintain this copyright. Users are
* permitted to modify this and use it to learn about the field of embedded
* software. Akshit Shah, Prof Alex Fosdick and the University of Colorado are 
* not liable for any misuse of this material.
*****************************************************************************/
/***************************************************************************************************
* @author : Akshit Shah
* @date : 02/18/2018
*
* @file : pthreadTest.h
* @brief : This header file provides an abstraction of threads handlers and 
* 	   defination of thread info structs
***************************************************************************************************/
#ifndef PTHREADTEST_H_
#define PTHREADTEST_H_

#define NUM_THREADS (3)

/* Array to store thread ids */
pthread_t threads[NUM_THREADS];

/* Information struct common to all threads*/
typedef struct _threadInfo_t{
	char* logFileName;
	pid_t processId;
	pthread_t threadId;
}ThreadInfo_t;

/* Thread handlers */
void *childThread1(void *threadp);
void *childThread2(void *threadp);
void *parentThread(void *threadp);

#endif /* PTHREADTEST_H_ */
