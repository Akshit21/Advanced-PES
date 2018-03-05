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
* @file : processTree_module.c
* @brief : Source file for custom kernel module for proc tree info
*****************************************************************************/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/pid.h>

static int pid = -1;

module_param(pid, int, S_IRUGO);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Akshit");

/* Function to give back the status */
static inline char* getState(int state)
{
	if(state>0)	return "Stopped";
	if(state == 0)	return "Running";
	if(state == -1)	return "Can't Run";
	return "Unknown state";	
}

/* Macro to get child Count */
#define getCount(ch_taskStruct)					\
({												\
	static unsigned int count = 0;				\
	struct list_head *list;						\
	list_for_each(list,ch_taskStruct)			\
	{											\
		count++;								\
	}											\
	count;										\
})


/*Initialization function */
int processTree_module_init(void)
{
	printk(KERN_ALERT "%s added\n",__FUNCTION__); 
	struct task_struct *my_task;
	
	if(pid == -1)
	{
		my_task = current;
	}
	else
	{
		struct pid *pid_struct = find_get_pid(pid);
		my_task = pid_task(pid_struct, PIDTYPE_PID);
	}
	
	/* Get current Process info*/
	printk(KERN_INFO "Current process: %s, PID: %d, State: %s, Children: %u, Nice: %d",
			my_task->comm, my_task->pid, getState(my_task->state),
			getCount(&my_task->children), task_nice(my_task));
	do
    {
    	/* Get Parent Process Info */
        my_task = my_task->parent;
        printk(KERN_INFO "Parent process: %s, PID: %d, State: %s, Children: %u, Nice: %d",
        		my_task->comm, my_task->pid, getState(my_task->state),
        		getCount(&my_task->children), task_nice(my_task));

    }while(my_task->pid != 0);
	return 0;
}

/*Exit function */
void processTree_module_exit(void)
{
  /* remove custom module */
  printk(KERN_ALERT "%s removed\n",__FUNCTION__);
}

/*Indicate the Initializing function*/
module_init(processTree_module_init);

/*Indicate the Exit function*/
module_exit(processTree_module_exit);
