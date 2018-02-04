# ECEN 5013-001: Advanced Practical Embedded Software Development: Homework 2

### Author: Akshit Shah
 
### Date: 02/04/2018

## Contents
```
* FileIO - Problem 2 file IO operations
* Syscall - Promlem 3 & 4 Implementation of syscall and cron task
* KernelModule - Problem 5 Implementation of kernel module
* LinkList - Problem 6 & 7 Implementation of Link List functions and Unit Test
```

## File IO operations
```
* SOURCES: Consisting of source files required for our project.
           1. fileIO.c - Source file for basic file IO operations
		   2. test.txt - File for IO operations
```

## Syscall and Cron task
```
** File structure: **
* syssort : Directory for syscall.c file and makefile
* SOURCES: Consisting of source files required for our project.
           1. syscall.c - Source file custom implemented sorting syscall
		   2. userspace.c - Source file for verifying syscall working and CRON task
		   
* Makefile: Makefile required to run syscall on our source files.
```

## Kernel Module
```
* SOURCES: Consisting of source files required for our project.
           1. mytimer_kernel_module.c - Source file custom implemented timer kernel module
		   
* Makefile: Makefile required to generate kernel module
```

## Double Link List and Unit test
```
** File structure: **
* unit_test: Consisting of Makefile and folders required to install and run unit tests in cmocka framework.

* Headers: Consisting of header files required for our project.
           1. dubleLinkList.h - Header file for double linked list functions

* SOURCES: Consisting of source files required for our project.
           1. dubleLinkList.c - Source file for double linked list functions
           2. unitTest_linkList.c - Source file for unit testing double linked list functions using cmocka
									(under unit_test directory)


* Makefile: Makefile required to run unit test on our source files.

** Execution: **
To run test for Double Linked List - $make test
```
