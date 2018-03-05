/**************************************************************************************************
* Copyright (C) 2017 by Akshit Shah,
*
* Redistribution, modification or use of this software in source or binary
* forms is permitted as long as the files maintain this copyright. Users are
* permitted to modify this and use it to learn about the field of embedded
* software. Akshit Shah,Prof. Alex Fosdick and the University of Colorado are 
* not liable for any misuse of this material.
***************************************************************************************************/
/***************************************************************************************************
* @author : Akshit Shah
* @date : 03/2/2018
*
* @file : process2.c
* @brief : Demonstration of different I2c mechanism
*
* @tool : Compiler - GCC, Linker - GDB, Cross Compiler - arm-linux-gnueabihf-gcc
* @hardware : Beagle Bone Green AM335x Arm Corex - A8, TMP106, APDS-9301
* @reference : https://www.geeksforgeeks.org/socket-programming-cc/
***************************************************************************************************/


#include "process.h"

int main()
{
	/* Create a msg structure to send to other process */
	Message_t msg ={0};
	
	/* USER I/P STRING AND LED ON/OFF */
	const char* my_msg = "MSG from Process 2 - Hi Process 1!!!";
	memmove(msg.data,my_msg,strlen(my_msg));
	
	msg.length = strlen(msg.data);
	msg.ledOn = 1;
	
#ifdef SHARED_MEM
	const int SIZE = 4096;
	const char *name = "HW4";

	/* Shared Mem Vars */ 
	int shm_fd;
	void *ptr;
	
	/* Access Shared Memory */
	shm_fd = shm_open(name, O_RDWR, 0666);
	
	/* Map to Memory address space */
	ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	printf("%s", (char*)ptr);
	
	ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	
	/* Send ACK back to process 1*/
	sprintf(ptr, "A");

	/* Unlink the memory after use*/
	shm_unlink(name);
#endif

#ifdef SOCKET
	struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *ack = "ACK MSG from client";
    char buffer[1024] = {0};
    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    /* Convert IPv4 and IPv6 addresses from text to binary form */
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
  	/* Connect socket to the server addr */
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    
    /* Read the data and send the ACK*/
    send(sock, ack, strlen(ack), 0 );
    printf("ACK message sent\n");
    valread = read(sock, buffer, 1024);
    printf("%s\n",buffer );

#endif

#ifdef QUEUE
	mqd_t my_q;
	struct mq_attr attr;
	attr.mq_msgsize = sizeof(Message_t);
	attr.mq_maxmsg = 256;
	const char *qname = "/HW4-Queue";
	
	my_q = mq_open(qname, O_CREAT | O_RDWR, 0666, &attr);
	if (my_q == -1) 
	{
		perror("QUEUE ERROR");
        exit(EXIT_FAILURE); 
	}
	
	Message_t rmsg = {0};
	char *recMsg = (char*)&rmsg;
	if(mq_receive(my_q, recMsg, sizeof(Message_t),0) == -1)
	{
		perror("QUEUE Receive ERROR");
        exit(EXIT_FAILURE); 
	}
	
	printf("MSG: %s\tSTRLEN:%u\tLED: %s\n",
		rmsg.data, rmsg.length, rmsg.ledOn?"ON":"OFF");
	
	if(mq_send(my_q, (char*)&msg, sizeof(msg), NULL) == -1)
	{
		perror("QUEUE SEND ERROR");
        exit(EXIT_FAILURE); 
	}
	
	mq_close(my_q);
    
#endif

	return 0;
}

