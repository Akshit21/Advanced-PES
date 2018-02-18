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
* @file : linkList.c
* @brief : Source file for link list generic methods to store character count
	   This file aims to provide source code for general methods of 
	   link list like insertion,traversal methods inorder to store and retreive 
	   data of character count required by child thread 1
		
* @tool : Compiler - GCC, Linker - GDB 
***************************************************************************************************/


#include "linkList.h"
#include <stdio.h>
#include <stdlib.h>

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
