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

#define NUM_THREADS (3)

pthread_t threads[NUM_THREADS];

typedef struct node{
	int data;
	struct node* next;
}Node_t;

typedef struct _threadInfo_t{
	FILE *fp;
	pid_t processId;
	pthread_t threadId;
}ThreadInfo_t;

ThreadInfo_t infoStruct;
pthread_mutex_t threadInfoLock;

bool runChildThread1 = true;
bool runChildThread2 = true;

/*Helper Function to get new node*/
Node_t* get_new_node(uint32_t data)
{
	/*Create a new node in heap*/
	Node_t *newNode = (Node_t*)malloc(sizeof(Node_t));
	/* Assign data and initialise pointers to NULL*/
	newNode->data = data;
	newNode->next = NULL;
	return newNode;
}

/*Function to insert a node in the end of link list*/
Node_t* insert_at_end(Node_t *head, uint32_t data)
{
	/*create a new node*/
	Node_t *newNode = get_new_node(data);

	/*Temp var to return head in the end*/
	Node_t *temp = head;

	/*if link is empty, new node as head*/
	if(head == NULL)	
	{
		head = newNode;
		return head;
	}

	/*Loop till you reach the last node*/
	while(head->next)
	{
		head = head->next;
	}
	
	/*adjust the new links*/
	head->next = newNode;

	return temp;
}

void updateValue(Node_t *head, uint32_t index)
{
	while(index--)
	{
		head = head->next;
	}
	head->data++;
}

void printLinkList(Node_t *head)
{
	/*Traverse the entire list*/
	while(head)
	{
		printf("%d ",head->data);
		head = head->next;
	}
	printf("\n");
}

/*Function to Delete every node in the link list*/
void destroy(Node_t* head)
{
	/*if link list empty do nothing*/
	if(head == NULL)	return;

	/*Info structure var to be freed*/
	Node_t *temp;

	/*Loop till you reach the last node*/
	while(head != NULL)
	{
		temp = head;
		head = head->next;
		free(temp);
	}
}

void exitThread_sigHandler(int sigNum)
{
	if(sigNum == SIGUSR1)
		runChildThread1 = false;
	if(sigNum == SIGUSR2)
		runChildThread2 = false;
}

void *childThread1(void *threadp)
{
	Node_t *head=NULL;
	for(int i=0;i<26;i++)
		head = insert_at_end(head,0);
	printf("\n");
	printLinkList(head);
	
	clock_t start = clock();
	pthread_mutex_lock(&threadInfoLock);
	infoStruct.fp = fopen("infolog.txt","a+");
	fprintf(infoStruct.fp, "%s %s %lu %s %d",
	"\nIn Child 1 Thread","\nThreadid: ",pthread_self(),"\nProcessid: ",getpid());
	fprintf(infoStruct.fp, "%s %lu", "\nChild 1 Start Time: ", start);
	
	printf("\nIn child 1");
	printf("\nthread id 1:%lu \t process id:%d",pthread_self(),getpid());
	
	FILE *file = fopen("Valentinesday.txt", "r");
	if (file == NULL)
        	return NULL; //could not open file
	int c;
	while ((c = fgetc(file)) != EOF)
    	{
        	if ((char)c >= 'A' && (char)c <= 'Z')
			updateValue(head,c-'A');
		if ((char)c >= 'a' && (char)c <= 'z')
			updateValue(head,c-'a');
        }
	fclose(file);
	printf("\n");
	printLinkList(head);

	fprintf(infoStruct.fp, "%s", "\ncharacters occurring thrice: "); 
	
	Node_t *temp = head;
	int count = 0;
	while(temp!=NULL)
	{
		if(temp->data == 3)
			fprintf(infoStruct.fp, "%c ", (char)((char)count + 'A'));
		temp = temp->next;
		count++;
	}
	fclose(infoStruct.fp);
	pthread_mutex_unlock(&threadInfoLock);
	
	signal(SIGUSR1, exitThread_sigHandler);
	while(runChildThread1)  sleep(1);

	pthread_mutex_lock(&threadInfoLock);
	infoStruct.fp = fopen("infolog.txt","a+");
	clock_t end = clock();
	fprintf(infoStruct.fp, "%s %lu", "\nChild 1 End Time: ", end);
	fclose(infoStruct.fp);
	pthread_mutex_unlock(&threadInfoLock);

	printf("\nExiting child thread1");
	destroy(head);
	pthread_exit(NULL);
}

void *childThread2(void *threadp)
{
	clock_t start = clock();
	pthread_mutex_lock(&threadInfoLock);
	infoStruct.fp = fopen("infolog.txt","a+");
	fprintf(infoStruct.fp, "%s %s %lu %s %d",
	"\nIn Child 2 Thread","\nThreadid: ",pthread_self(),"\nProcessid: ",getpid());
	fprintf(infoStruct.fp, "%s %lu", "\nChild 2 Start Time: ", start);
	printf("\nIn child 2");
	printf("\nthread id 2:%lu \t process id:%d",pthread_self(),getpid());
	fclose(infoStruct.fp);
	pthread_mutex_unlock(&threadInfoLock);
	
	long double a[4], b[4], loadavg;
	FILE *fp;
	char dump[50];
	int milisec = 100; // length of time to sleep, in miliseconds
	struct timespec req = {0};
	req.tv_sec = 0;
	req.tv_nsec = milisec * 1000000L;

	signal(SIGUSR2, exitThread_sigHandler);

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
		if(runChildThread2 == false) break;
	}
	
	//while(runChildThread2)  sleep(1);
	
	clock_t end = clock();
	pthread_mutex_lock(&threadInfoLock);
	infoStruct.fp = fopen("infolog.txt","a+");
	fprintf(infoStruct.fp, "%s %lu", "\nChild 2 End Time: ", end);
	fclose(infoStruct.fp);
	pthread_mutex_unlock(&threadInfoLock);
	pthread_exit(NULL);
}

void *parentThread(void *threadp)
{
	clock_t start = clock();
	
	pthread_mutex_lock(&threadInfoLock);
	infoStruct.fp = fopen("infolog.txt","a+");
	fprintf(infoStruct.fp, "%s %s %lu %s %d",
	"\nIn Parent Thread","\nThreadid: ",pthread_self(),"\nProcessid: ",getpid());
	fprintf(infoStruct.fp, "%s %lu", "\nParent Start Time: ", start);
	printf("\nIn Parent thread");
	printf("\nthread id parent:%lu \t process id:%d",pthread_self(),getpid());
	fclose(infoStruct.fp);
	pthread_mutex_unlock(&threadInfoLock);

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

   	pthread_join(threads[1], NULL);	/* join the pthread wiith the existing processes */
	pthread_join(threads[2], NULL);	/* join the pthread wiith the existing processes */
	
	pthread_mutex_lock(&threadInfoLock);
	infoStruct.fp = fopen("infolog.txt","a+");
	clock_t end = clock();
	fprintf(infoStruct.fp, "%s %lu", "\nParent End Time: ", end);
	fclose(infoStruct.fp);
	pthread_mutex_unlock(&threadInfoLock);

	pthread_exit(NULL);
}

int main()
{
	infoStruct.fp = fopen("infolog.txt","a+");
	fprintf(infoStruct.fp, "%s", "\n**************************************");
	fprintf(infoStruct.fp, "%s", "\nThread Statistics\n");
	fprintf(infoStruct.fp, "%s", "**************************************\n");
	fclose(infoStruct.fp);
	
	if(pthread_mutex_init(&threadInfoLock, NULL)!= 0)
	{
		printf("ERROR:Mutex Init\n");
		return 1;
	}
	
	pthread_create(&threads[0],   		/* pointer to thread descriptor */
		      (void *)0,		/* use default attributes */
		       parentThread, 		/* thread function entry point */
		       (void *)&infoStruct 	/* parameters to pass in */
		      );

   	pthread_join(threads[0], NULL);	/* join the pthread wiith the existing processes */
	pthread_mutex_destroy(&threadInfoLock);
	exit(0);
}


