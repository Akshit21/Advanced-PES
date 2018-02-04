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
* @date : 02/03/2018
*
* @file : mytimer_kernel_module.c
* @brief : Source file for custom timer kernel module
***************************************************************************************************/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/kernel.h>

static struct timer_list kTimer;

static char * name = "akshit";
static int count = 10;

module_param(name, charp, S_IRUGO);
module_param(count, int, S_IRUGO);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Akshit");

/*Callback function for our timer*/
void kTimer_callback(unsigned long data)
{
  printk(KERN_INFO "Name:%s\t Number of times callback called - %d\\n",name,count);
  count++;
  /*Reset the timer*/
  mod_timer(&kTimer, jiffies + msecs_to_jiffies(500));
}

/*Initialization function sets up the timer*/
int mytimer_kernel_module_init(void)
{ 
  printk(KERN_ALERT "%s loaded\n",__FUNCTION__);
  /* setup timer to call my_timer_callback */
  setup_timer(&kTimer, kTimer_callback, 0);
  /* setup timer interval to 500 msecs */
  mod_timer(&kTimer, jiffies + msecs_to_jiffies(500));

  return 0;
}

/*Exit function deletes the timer*/
void mytimer_kernel_module_exit(void)
{
  int status;
  /* remove kernel timer when unloading module */
  if((status = del_timer(&kTimer))== 0)
  {
    printk(KERN_ALERT "Timer inactive\n");
  }
  printk(KERN_ALERT "%s removed\n",__FUNCTION__);
}

/*Indicate the Initializing function*/
module_init(mytimer_kernel_module_init);

/*Indicate the Exit function*/
module_exit(mytimer_kernel_module_exit);
