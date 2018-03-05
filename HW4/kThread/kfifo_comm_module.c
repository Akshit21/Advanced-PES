/*****************************************************************************
* Copyright (C) 2017 by Akshit Shah
*
* Redistribution, modification or use of this software in source or binary
* forms is permitted as long as the files maintain this copyright. Users are
* permitted to modify this and use it to learn about the field of embedded
* software. Akshit Shah, Prof Alex Fosdick and the University of Colorado are   
* not liable for any misuse of this material.
*****************************************************************************/
/****************************************************************************
* @author : Akshit Shah
* @date : 02/03/2018
*
* @file : kfifo_comm_module.c
* @brief : Source file for custom kernel module for kernel communication
*****************************************************************************/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kfifo.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/kthread.h>	
#include <linux/delay.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Akshit");

#define MYKFIFO		mykfifo
#define MYKFIFO_SIZE	(16)

static DECLARE_KFIFO(MYKFIFO,struct task_struct*,MYKFIFO_SIZE);	
static DEFINE_MUTEX(fifo_lock);

static int task1_count = 0;
static int task2_count = 0;

static unsigned long sleep_time= 5;
module_param(sleep_time, ulong, S_IRUGO | S_IWUSR);

struct task_struct *task1;
struct task_struct *task2;
	
int task1_callback(void *params)
{
	printk(KERN_ALERT "%s added\n",__FUNCTION__);

	while(!kthread_should_stop())
	{		
		/* Lock the mutex*/
		if (mutex_lock_interruptible(&fifo_lock))
		{
			printk(KERN_ERR "Lock not Acquired\n");
			return -ERESTARTSYS;
		}
		
		/* Put the data into KFIFO */
		if(kfifo_put(&MYKFIFO, current) == 0)
			printk(KERN_INFO "BUFFER FULL\n");
		
		else
		{
			//printk(KERN_INFO "PRINT SOMETHING\n");
		}

		/* Unlock the mutex*/
		mutex_unlock(&fifo_lock);

		/* Increment the count */
		task1_count++;
		ssleep(sleep_time);
	}

	printk(KERN_INFO "%s is removed\n",__FUNCTION__);	
	return task1_count;
}

int task2_callback(void *params)
{
	struct task_struct *data;
	printk(KERN_ALERT "%s added\n",__FUNCTION__);

	while(!kthread_should_stop())
	{
		/* Lock the mutex*/
		if (mutex_lock_interruptible(&fifo_lock))
		{
			printk(KERN_ERR "Lock not Acquired\n");
			return -ERESTARTSYS;
		}
		
		/* Recv the data */
		if(kfifo_get(&MYKFIFO, &data) == 0)
		{
			//printk(KERN_INFO "BUFFER EMPTY\n");
		}
		else	
		{
			/* Print Process Id and Vruntime of the module*/
			printk(KERN_INFO "Prev Process ID: %d, Vruntime: %llu\n",list_prev_entry(data, tasks)->pid, list_prev_entry(data, tasks)->se.vruntime);
			
			printk(KERN_INFO "Curr Process ID: %d, Vruntime: %llu\n",data->pid, data->se.vruntime);
			
			printk(KERN_INFO "Next Process ID: %d, Vruntime: %llu\n",list_next_entry(data, tasks)->pid, list_next_entry(data, tasks)->se.vruntime);
			
			task2_count++;
		}

		/* Unlock the mutex*/	
		mutex_unlock(&fifo_lock);	
	}

	printk(KERN_INFO "%s is removed\n",__FUNCTION__);
	return task2_count;	
}

/*Initialization function */
int kfifo_comm_module_init(void)
{
	printk(KERN_ALERT "%s added\n",__FUNCTION__);

	/* Init a kfifo */
	INIT_KFIFO(MYKFIFO);

	/* Create two threads */
	task1 = kthread_run(task1_callback,NULL,"Task 1");
	if(IS_ERR(task1))	
	{
		printk(KERN_ERR "Couldn't create Task1 Thread.\n");
		return -1;
	}

	task2 = kthread_run(task2_callback,NULL,"Task 2");
	if(IS_ERR(task2))	
	{
		printk(KERN_ERR "Couldn't create Task2 Thread.\n");

		if(kthread_stop(task1) != -1)
		{
			printk(KERN_INFO "STOPPED TASK 1\n");
		}
		return -1;
	}

	return 0;
}

/* Exit Function */
void kfifo_comm_module_exit(void)
{
	/* Clean up the threads */
	if(kthread_stop(task1) != -1)
	{
		printk(KERN_INFO "Task1 thread has stopped. Task1 Count:%d\n", kthread_stop(task1));
	}

	if(kthread_stop(task2) != -1)
	{
		printk(KERN_INFO "task2 thread has stopped. Data Consumed Count:%d\n",kthread_stop(task2));
	}

	printk(KERN_INFO "%s is removed\n",__FUNCTION__);
}

/* Indicate init and exit functions */
module_init(kfifo_comm_module_init);
module_exit(kfifo_comm_module_exit);
