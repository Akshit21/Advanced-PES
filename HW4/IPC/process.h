/***************************************************************************************************
* Copyright (C) 2017 by Akshit Shah
*
* Redistribution, modification or use of this software in source or binary
* forms is permitted as long as the files maintain this copyright. Users are
* permitted to modify this and use it to learn about the field of embedded
* software. Akshit Shah, Prof Alex Fosdick and the University of Colorado are 
* not liable for any misuse of this material.
***************************************************************************************************/
/***************************************************************************************************
* @author : Akshit Shah
* @date : 03/2/2018
*
* @file : process.h
* @brief : This header file provides an abstraction of Mesagge structure.
***************************************************************************************************/
#ifndef PROCESS_H_
#define PROCESS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include <mqueue.h>

#define PORT 8080

typedef struct _Message_t
{
	int8_t data[100];
	size_t length;
	uint8_t ledOn;
}Message_t;

//#define SHARED_MEM
#define PIPE
//#define SOCKET
//#define QUEUE

#endif /* PROCESS_H_ */
