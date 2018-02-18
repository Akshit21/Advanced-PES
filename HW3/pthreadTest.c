/*https://stackoverflow.com/questions/3769405/determining-cpu-utilization*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#include "linkList.h"

#define NUM_THREADS (3)

/* Array to store thread ids */
pthread_t threads[NUM_THREADS];

/* Information struct common to all threads*/
typedef struct _threadInfo_t{
	FILE *fp;
	pid_t processId;
	pthread_t threadId;
}ThreadInfo_t;

ThreadInfo_t infoStruct;

/* Declare Mutex variable to protect shared file resource */
pthread_mutex_t threadInfoLock;

/* Variables to track running threads */
bool runChildThread1 = true;
bool runChildThread2 = true;

/* Signal handler for SIGUSR1 & SIGUSR2 */
void exitThread_sigHandler(int sigNum)
{
	if(sigNum == SIGUSR1)
		runChildThread1 = false;
	if(sigNum == SIGUSR2)
		runChildThread2 = false;
}

/* Child Thread 1 Handler */
void *childThread1(void *threadp)
{
	/* Start the clock timer */
	clock_t start = clock();

	/* Debug Prints */
	printf("\nIn child 1");
	printf("\nthread id 1:%lu \t process id:%d",pthread_self(),getpid());

	/* Create a link List with 26 nodes */
	Node_t *head=NULL;
	for(int i=0;i<26;i++)
		head = insert_at_end(head,0);
	
	/* Lock the mutex before accessing the shared file resource */
	pthread_mutex_lock(&threadInfoLock);
	
	/* Open the file */
	infoStruct.fp = fopen("infolog.txt","a+");
	if(infoStruct.fp == NULL)
		return NULL;	/*could not open file*/

	/* Log the data in the file */
	fprintf(infoStruct.fp, "%s %s %lu %s %d",
	"\nIn Child 1 Thread","\nThreadid: ",pthread_self(),"\nProcessid: ",getpid());
	fprintf(infoStruct.fp, "%s %lu", "\nChild 1 Start Time: ", start);
	
	/* Read the given text file */
	FILE *file = fopen("Valentinesday.txt", "r");
	if (file == NULL)
        	return NULL; /*could not open file*/
	int c;
	/* Read untill end of file */
	while ((c = fgetc(file)) != EOF)
    	{
		/* If Alphabet found increment the data count */
        	if ((char)c >= 'A' && (char)c <= 'Z')
			updateValue(head,c-'A');
		if ((char)c >= 'a' && (char)c <= 'z')
			updateValue(head,c-'a');
        }
	fclose(file);
	
	fprintf(infoStruct.fp, "%s", "\ncharacters occurring thrice: "); 
	
	Node_t *temp = head;
	int count = 0;
	/* Traverse the link list and print the character which has occured thrice */ 
	while(temp!=NULL)
	{
		if(temp->data == 3)
			fprintf(infoStruct.fp, "%c ", (char)((char)count + 'A'));
		temp = temp->next;
		count++;
	}
	
	/* Close the file */
	fclose(infoStruct.fp);
	/* Release the lock */
	pthread_mutex_unlock(&threadInfoLock);
	
	/* Wait for SIGUSR1 from user to exit the thread*/ 
	signal(SIGUSR1, exitThread_sigHandler);
	while(runChildThread1)  sleep(1);
	
	/* After exit signal received log the exit time*/
	pthread_mutex_lock(&threadInfoLock);
	infoStruct.fp = fopen("infolog.txt","a+");
	
	/* Get the end time */
	clock_t end = clock();
	fprintf(infoStruct.fp, "%s %lu", "\nChild 1 End Time: ", end);
	/* Close the file */
	fclose(infoStruct.fp);
	/* Release the lock */
	pthread_mutex_unlock(&threadInfoLock);

	printf("\nExiting child thread1");
	/* Free the dynamic link list memory */
	destroy(head);
	pthread_exit(NULL);
}

/* Child Thread 2 Handler */
void *childThread2(void *threadp)
{
	/* Start the clock timer */
	clock_t start = clock();
	
	/* Debug Prints */
	printf("\nIn child 2");
	printf("\nthread id 2:%lu \t process id:%d",pthread_self(),getpid());

	/* Lock the mutex before accessing the shared file resource */
	pthread_mutex_lock(&threadInfoLock);

	/* Open the file */
	infoStruct.fp = fopen("infolog.txt","a+");

	/* Log the data in the file */
	fprintf(infoStruct.fp, "%s %s %lu %s %d",
	"\nIn Child 2 Thread","\nThreadid: ",pthread_self(),"\nProcessid: ",getpid());
	fprintf(infoStruct.fp, "%s %lu", "\nChild 2 Start Time: ", start);
	
	/* Close the file */
	fclose(infoStruct.fp);
	/* Release the lock */
	pthread_mutex_unlock(&threadInfoLock);
	
	long double a[4], b[4], loadavg;
	FILE *fp;
	char dump[50];
	int milisec = 100; /* length of time to sleep, in miliseconds */
	struct timespec req = {0};
	req.tv_sec = 0;
	req.tv_nsec = milisec * 1000000L;
	/* Wait for SIGUSR2 from user to exit the thread*/
	signal(SIGUSR2, exitThread_sigHandler);
	
	/* Log CPU Utilization every 100 ms */
	while(1)
	{
		fp = fopen("/proc/stat","r");
		fscanf(fp,"%*s %Lf %Lf %Lf %Lf",&a[0],&a[1],&a[2],&a[3]);
		fclose(fp);
		nanosleep(&req, (struct timespec *)NULL);
		fp = fopen("/proc/stat","r");
		fscanf(fp,"%*s %Lf %Lf %Lf %Lf",&b[0],&b[1],&b[2],&b[3]);
		fclose(fp);
		loadavg = ((b[0]+b[1]+b[2]) - (a[0]+a[1]+a[2])) / 
			  ((b[0]+b[1]+b[2]+b[3]) - (a[0]+a[1]+a[2]+a[3]));
		
		pthread_mutex_lock(&threadInfoLock);
		infoStruct.fp = fopen("infolog.txt","a+");
		fprintf(infoStruct.fp, "%s %LF","\nThe current CPU utilization is : ",loadavg);
		fclose(infoStruct.fp);
		pthread_mutex_unlock(&threadInfoLock);

		nanosleep(&req, (struct timespec *)NULL);
		if(runChildThread2 == false) break; /* exit if signal received */
	}
	
	/* After exit signal received log the exit time*/
	clock_t end = clock();
	pthread_mutex_lock(&threadInfoLock);
	/* Open the file */
	infoStruct.fp = fopen("infolog.txt","a+");
	/* Log the data in the file */
	fprintf(infoStruct.fp, "%s %lu", "\nChild 2 End Time: ", end);
	/* Close the file */	
	fclose(infoStruct.fp);
	/* Release the lock */
	pthread_mutex_unlock(&threadInfoLock);

	printf("\nExiting child thread2");
	pthread_exit(NULL);
}

/* Parent Thread Handler */
void *parentThread(void *threadp)
{
	/* Start the clock timer */
	clock_t start = clock();

	/* Debug Prints */
	printf("\nIn Parent thread");
	printf("\nthread id parent:%lu \t process id:%d",pthread_self(),getpid());
	
	/* Lock the mutex before accessing the shared file resource */
	pthread_mutex_lock(&threadInfoLock);

	/* Open the file */
	infoStruct.fp = fopen("infolog.txt","a+");

	/* Log the data in the file */
	fprintf(infoStruct.fp, "%s %s %lu %s %d",
	"\nIn Parent Thread","\nThreadid: ",pthread_self(),"\nProcessid: ",getpid());
	fprintf(infoStruct.fp, "%s %lu", "\nParent Start Time: ", start);
	
	/* Close the file */
	fclose(infoStruct.fp);

	/* Release the lock */
	pthread_mutex_unlock(&threadInfoLock);

	/* Create 2 child threads */
	pthread_create(&threads[1],   		/* pointer to thread descriptor */
		      (void *)0,		/* use default attributes */
		       childThread1, 		/* thread function entry point */
		       (void *)&infoStruct 	/* parameters to pass in */
		      );
	
	pthread_create(&threads[2],   		/* pointer to thread descriptor */
		      (void *)0,		/* use default attributes */
		       childThread2, 		/* thread function entry point */
		       (void *)&infoStruct 	/* parameters to pass in */
		      );

   	pthread_join(threads[1], NULL);	/* join the pthread with the existing processes */
	pthread_join(threads[2], NULL);	/* join the pthread with the existing processes */
	
	/* After 2 child threads exit log the exit time*/
	pthread_mutex_lock(&threadInfoLock);
	/* Open the file */
	infoStruct.fp = fopen("infolog.txt","a+");
	clock_t end = clock();
	fprintf(infoStruct.fp, "%s %lu", "\nParent End Time: ", end);
	/* Close the file */
	fclose(infoStruct.fp);
	/* Release the lock */
	pthread_mutex_unlock(&threadInfoLock);	
	printf("\nExiting Parent thread");
	pthread_exit(NULL);
}

int main()
{
	/* Create the file to log the data */
	infoStruct.fp = fopen("infolog.txt","a+");
	fprintf(infoStruct.fp, "%s", "\n**************************************");
	fprintf(infoStruct.fp, "%s", "\nThread Statistics\n");
	fprintf(infoStruct.fp, "%s", "**************************************\n");
	fclose(infoStruct.fp);
	
	/* Init the mutex */
	if(pthread_mutex_init(&threadInfoLock, NULL)!= 0)
	{
		printf("ERROR:Mutex Init\n");
		return 1;
	}
	
	/* Create the master thread */
	pthread_create(&threads[0],   		/* pointer to thread descriptor */
		      (void *)0,		/* use default attributes */
		       parentThread, 		/* thread function entry point */
		       (void *)&infoStruct 	/* parameters to pass in */
		      );

   	pthread_join(threads[0], NULL);	/* join the pthread with the existing processes */
	pthread_mutex_destroy(&threadInfoLock);
	exit(0);
}


