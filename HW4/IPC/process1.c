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
* @file : process1.c
* @brief : Demonstration of different I2c mechanism
*
* @tool : Compiler - GCC, Linker - GDB, Cross Compiler - arm-linux-gnueabihf-gcc
* @hardware : Beagle Bone Green AM335x Arm Corex - A8, TMP106, APDS-9301
* @reference : Socket: https://www.geeksforgeeks.org/socket-programming-cc/
***************************************************************************************************/


#include "process.h"

int main()
{
	/* Create a msg structure to send to other process */
	Message_t msg ={0};
	
	/* USER I/P STRING AND LED ON/OFF */
	const char* my_msg = "MSG from Process 1 - Hi Process 2!!!";
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
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	
	/* Truncate the size */
	ftruncate(shm_fd, SIZE);
	
	/* Access Memory address space */
	ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

	/* SEND DATA INTO THE SHARED MEM*/
	memcpy((char*)ptr,&msg,sizeof(Message_t));
	
	sem_post(shm_fd);
		
	printf("\nDATA SENT: WAITING FOR ACK\n");
	
	sem_wait(shm_fd);
	
	/* YOU WILL REACH HERE IF YOU RECEIVE ACK*/
	printf("Recieved ACK\n");
#endif

#ifdef PIPE
	/*pipes for parent and child*/
	int pipe1[2];
	int pipe2[2];
     
    pid_t pChild;
 
    if (pipe(pipe1)==-1)
    {
        printf("Pipe call error\n");
        return 1;
    }
    if (pipe(pipe2)==-1)
    {
        printf("Pipe call error\n");
        return 1;
    }
 
    pChild = fork();
 
    if (pChild < 0)
    {
        printf("Fork error\n");
        return 1;
    }
    
    /*Parent Process*/
	else if (pChild >0)
	{
		/* Close receiving end*/
		close(pipe1[0]);
		
		write(pipe1[1], (char*)&msg, sizeof(msg));
		close(pipe1[1]);
		
		/* Wait for child to send a string */
        wait(NULL);
		
		close(pipe2[1]);
		
		char buf[sizeof(Message_t)] = {0};
		read(pipe2[0], buf, sizeof(Message_t));
		
		Message_t *rmsg = (Message_t*)buf;
		printf("MSG: %s\tSTRLEN:%u\tLED: %s\n",
		rmsg->data, rmsg->length, rmsg->ledOn?"ON":"OFF");
		
		close(pipe2[0]);
	}
	
	/*Child Process*/
	else
	{
		/* Close receiving end*/
		close(pipe1[1]);
		
		char buf[sizeof(Message_t)] = {0};
		read(pipe1[0], buf, sizeof(Message_t));
		
		Message_t *rmsg = (Message_t*)buf;
		printf("MSG: %s\tSTRLEN:%u\tLED: %s\n",
		rmsg->data, rmsg->length, rmsg->ledOn?"ON":"OFF");
		
		close(pipe1[0]);
		
		close(pipe2[0]);
		
		/* Create a msg structure to send to other process */
		Message_t smsg ={0};
	
		/* USER I/P STRING AND LED ON/OFF */
		const char* my_msg = "MSG from Child Process - Hi Process 1!!!";
		memmove(smsg.data,my_msg,strlen(my_msg));
	
		smsg.length = strlen(msg.data);
		smsg.ledOn = 1;
		
		write(pipe2[1], (char*)&smsg, sizeof(smsg));
		
		close(pipe2[1]);
	}
	
#endif

#ifdef SOCKET
	
	int server_fd, new_socket, valread;
    struct sockaddr_in address;
    
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *msg_data;
      
    /* Creating socket connection */
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    /* Forcefully attaching socket to the port 8080 */
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    
    /* Setup Address */
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    /* Binding socket to the port 8080 */
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    /* Listen for connections */
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    
    /* Accept the connections if any */
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    
    /* SEND DATA INTO THE SOCKET*/
	sprintf(msg_data, "MSG: %s\tSTRLEN:%u\tLED: %s\n",
		msg.data, msg.length, msg.ledOn?"ON":"OFF");
		
    valread = read(new_socket, buffer, 1024);
    printf("%s\n",buffer );
    send(new_socket , msg_data , strlen(msg_data) , 0 );
    printf("Process1 :Message sent\n");

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

	if(mq_send(my_q, (char*)&msg, sizeof(msg), NULL) == -1)
	{
		perror("QUEUE SEND ERROR");
        exit(EXIT_FAILURE); 
	}
	
	Message_t rmsg = {0};

	if(mq_receive(my_q, (char*)&rmsg, sizeof(rmsg),0) == -1)
	{
		perror("QUEUE Receive ERROR");
        exit(EXIT_FAILURE); 
	}
	
	printf("MSG: %s\tSTRLEN:%u\tLED: %s\n",
		rmsg.data, rmsg.length, rmsg.ledOn?"ON":"OFF");

	mq_unlink(qname);
#endif
	return 0;
}

