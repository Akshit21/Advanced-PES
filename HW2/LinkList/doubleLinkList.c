/*****************************************************************************
* Copyright (C) 2017 by Akshit Shah
*
* Redistribution, modification or use of this software in source or binary
* forms is permitted as long as the files maintain this copyright. Users are
* permitted to modify this and use it to learn about the field of embedded
* software. Akshit Shah, Prof Alex Fosdick and the University of Colorado are   not liable for any misuse of this material.
*****************************************************************************/
/***************************************************************************************************
* @author : Akshit Shah
* @date : 02/03/2018
*
* @file : doubleLinkList.c
* @brief : Source file for doubly link list generic methods
	   This file aims to provide source code for general methods of 
	   doubly link list like insertion, deletion and traversal methods
		
* @tool : Compiler - GCC, Linker - GDB
***************************************************************************************************/

#include "doubleLinkList.h"
#include <stdio.h>
#include <stdlib.h>
#include<stddef.h>

#define GET_LIST_CONTAINER(ptr, type, member) ({                      \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})

/*Helper Function to get new node*/
Info_t* get_new_node(uint32_t data)
{
	/*Create a new node in heap*/
	Info_t *newNode = (Info_t*)malloc(sizeof(Info_t));
	/* Assign data and initialise pointers to NULL*/
	newNode->data = data;
	newNode->node.next = NULL;
	newNode->node.prev = NULL;
	return newNode;
}

/*Helper Function to print the link list*/
void printLinkList(Node_t *head)
{
	/*Traverse the entire list*/
	while(head)
	{
		Info_t *temp = GET_LIST_CONTAINER(head,Info_t,node);	
		printf("%d ",temp->data);
		head = head->next;
	}
	printf("\n");
}

/*Helper Function to verify given node value*/
Status_t verifyValue(Node_t *head, uint32_t data, uint32_t index)
{
	Info_t *temp;
	/*Traverse the entire list or till the index */
	while(head && index--)
	{
		temp = GET_LIST_CONTAINER(head,Info_t,node);	
		head = head->next;
	}
	/* If data matches*/
	if(data == temp->data)
		return SUCCESS;
	else
		return FAILURE;
}

/*Function to Delete every node in the link list*/
void destroy(Node_t* head)
{
	/*if link list empty do nothing*/
	if(head == NULL)	return;

	/*Info structure var to be freed*/
	Info_t *temp;

	/*Loop till you reach the last node*/
	while(head != NULL)
	{
		temp = GET_LIST_CONTAINER(head,Info_t,node);
		free(temp);
		head = head->next;
	}
}

/*Function to insert a node in the beginning of link list*/
Node_t* insert_at_beginning(Node_t *head, uint32_t data)
{
	/*create a new node*/
	Info_t *newNode = get_new_node(data);

	/*adjust the new links*/
	newNode->node.next = head;
	if(head !=  NULL)
		head->prev = &(newNode->node);
	head = &(newNode->node);
	
	return head;
}

/*Function to insert a node in the end of link list*/
Node_t* insert_at_end(Node_t *head, uint32_t data)
{
	/*create a new node*/
	Info_t *newNode = get_new_node(data);

	/*Temp var to return head in the end*/
	Node_t *temp = head;

	/*if link is empty, new node as head*/
	if(head == NULL)	
	{
		printf("NULL\n");
		return &(newNode->node);
	}

	/*Loop till you reach the last node*/
	while(head->next)
	{
		head = head->next;
	}
	
	/*adjust the new links*/
	head->next = &(newNode->node);
	newNode->node.prev = head;

	return temp;
}

/*Function to insert a node at a given position of link list*/
Node_t* insert_at_position(Node_t *head, uint32_t data, uint32_t index)
{
	/*create a new node*/
	Info_t *newNode = get_new_node(data);

	/*if head is null the new node is the only node*/
	if(head == NULL)	return &(newNode->node);

	/*Temp var to return head in the end*/
	Node_t *temp = head;
	int i=0;
	/*if first node*/
	if(index==1)
	{
		head = insert_at_beginning(head,data);
		return head;
	}
	/*Loop till you reach the last node or at the reqd position*/
	while(head->next != NULL && i<(index-2))
	{
		head = head->next;
		i++;	
	}

	/*Adjust the links at that positions*/
	newNode->node.next = head->next;
	newNode->node.prev = head;

	head->next = &(newNode->node);

	return temp;
}

/*Function to delete a node from the beginning of link list*/
Node_t* delete_from_beginning(Node_t* head)
{
	/*No nodes*/
	if(head==NULL)	return NULL;

	// only one Node
	if(head->next == NULL)
	{
		Info_t *temp = GET_LIST_CONTAINER(head,Info_t,node);	
		free(temp);
		return NULL;
	}

	/*Var temp for node to be freed*/
	Info_t *temp = GET_LIST_CONTAINER(head,Info_t,node);

	/*Move the head*/
	head = head->next;
	head->prev = NULL;
	/*delete the node*/
	free(temp);

	return head;
}

/*Function to delete a node from the end of link list*/
Node_t* delete_from_end(Node_t* head)
{
	/*No nodes*/
	if(head==NULL)	return NULL;

	// only one Node
	if(head->next == NULL)
	{
		Info_t *temp = GET_LIST_CONTAINER(head,Info_t,node);	
		free(temp);
		return NULL;
	}

	/*Var temp for node to be freed*/
	Node_t *temp1=head,*prev=NULL;

	/*Traverse till the end node*/
	while(temp1->next != NULL)
	{
		prev = temp1;
		temp1 = temp1->next;
	}

	Info_t *temp = GET_LIST_CONTAINER(temp1,Info_t,node);	

	/*adjust link of the prev node*/
	Info_t *x = GET_LIST_CONTAINER(prev,Info_t,node);
	x->node.next = NULL;	
	/*delete the node*/
	free(temp);

	return head;
}

/*Function to delete a node from a given position in a link list*/
Node_t* delete_from_position(Node_t* head, uint32_t index)
{
	/*No nodes*/
	if(head == NULL)	return NULL;

	/*Var temp for node to be freed*/
	Node_t *temp1 = head,*prev=NULL;
	uint32_t i=0;
	/*Traverse till the end node*/
	while(temp1->next != NULL && i<index-1)
	{
		prev = temp1;
		temp1 = temp1->next;
		i++;
	}

	/*If position is invalid do nothing and return*/
	if(temp1->next == NULL)	return head;
	if(temp1->next == NULL && i == index) 
	{
		head  = delete_from_end(head);
		return head;
	}
	/*adjust link of the prev and next node*/
	Info_t *temp = GET_LIST_CONTAINER(temp1,Info_t,node);
	Info_t *pre = GET_LIST_CONTAINER(prev,Info_t,node);
	Info_t *next = GET_LIST_CONTAINER(temp->node.next,Info_t,node);

	pre->node.next = &(next->node);
	next->node.prev = &(pre->node);


	/*delete the node*/
	free(temp);

	return head;
}

/*Function to give number of nodes in the link list*/
uint32_t size(Node_t *head)
{
	/*if no nodes return 0*/
	if(head == NULL)	return 0;

	/*Counter to determine number of links*/
	uint32_t count = 1;

	/*Traverse till the end node*/
	while(head->next != NULL)
	{
		head = head->next;
		count++;
	}
	return count;
}

