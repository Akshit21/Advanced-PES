/*****************************************************************************
* Copyright (C) 2017 by Akshit Shah
*
* Redistribution, modification or use of this software in source or binary
* forms is permitted as long as the files maintain this copyright. Users are
* permitted to modify this and use it to learn about the field of embedded
* software. Akshit Shah, Prof Alex Fosdick and the University of Colorado are   not liable for any misuse of this material.
*****************************************************************************/
/**
* @file doubleLinkList.h
* @brief An abstraction for general link list operations
*
* This header file provides an abstraction of general link list operations
* link insertion, deletion and traversal via function calls.
*
* @author Akshit Shah
* @date 02/03/2018
*
*/

#ifndef DOUBLELINKEDLIST_H_
#define DOUBLELINKEDLIST_H_

#include <stdint.h>

/* Node structure */
typedef struct Node {
	struct Node * prev;
	struct Node * next;
}Node_t;

typedef struct Info {
	uint32_t data;
	Node_t node;
}Info_t;

typedef enum{
	SUCCESS,
	FAILURE
}Status_t;

/**
* @brief Creates a new Node in the link list
*
* Given a data value for the new Node, this will create 
* a new node from dynamic memory, assign the data value
* and return the address of the new node
*
* @param data Value of the new node
*
* @return Pointer to the new node.
*/
Info_t* get_new_node(uint32_t data);

/**
* @brief Destroys all the link list nodes
*
* Given the head node of the link list, this function
* will delete all the nodes in the link list and free
* up the memory
*
* @param head Pointer to the Head node of the link list
*
* @return void
*/
void destroy(Node_t* head);

/**
* @brief Insert a new node in the beginning
*
* Given a head node and data to be assigned to a new Node,
* this function will create a new node at the beginning of
* the link list. If the link list will be empty, it will be
* a head node
*
* @param head Pointer to the Head node of the link list
* @param data Data to be assigned to the new Node
*
* @return Pointer to the head node
*/
Node_t* insert_at_beginning(Node_t* head, uint32_t data);

/**
* @brief Insert a new node in the end
*
* Given a head node and data to be assigned to a new Node,
* this function will create a new node at the end of
* the link list. If the link list will be empty, it will be
* a head node
*
* @param head Pointer to the Head node of the link list
* @param data Data to be assigned to the new Node
*
* @return Pointer to the head node
*/
Node_t* insert_at_end(Node_t* head, uint32_t data);

/**
* @brief Insert a new node at the given position
*
* Given a head node,data to be assigned and position of the new Node,
* this function will create a new node at the given position of
* the link list. If the link list will be empty, it will be
* a head node
*
* @param head Pointer to the Head node of the link list
* @param data Data to be assigned to the new Node
* @param index Position to be inserted at
*
* @return Pointer to the head node
*/
Node_t* insert_at_position(Node_t* head, uint32_t data, uint32_t index);

/**
* @brief Delete a node from the beginning
*
* Given a head node, this function will delete a node 
* from the beginning of the link list.
*
* @param head Pointer to the Head node of the link list
*
* @return Pointer to the head node
*/
Node_t* delete_from_beginning(Node_t* head);

/**
* @brief Delete a node from the end
*
* Given a head node, this function will delete a node 
* from the end of the link list.
*
* @param head Pointer to the Head node of the link list
*
* @return Pointer to the head node
*/
Node_t* delete_from_end(Node_t* head);

/**
* @brief Delete a node from the specified position
*
* Given a head node, this function will delete a node 
* from a given position of the link list.
*
* @param head Pointer to the Head node of the link list
* @param index Position from which node will be deleted
*
* @return Pointer to the head node
*/
Node_t* delete_from_position(Node_t* head, uint32_t index);

/**
* @brief Size of the link list
*
* Given a head node, this function will give you 
* number of nodes in the link list.
*
* @param head Pointer to the Head node of the link list
*
* @return size of the link list
*/
uint32_t size(Node_t* head);

/**
* @brief Helper function to print the link list nodes
*
* Given a head node, this function will print all the
* data values of link list nodes
*
* @param head Pointer to the Head node of the link list
*
* @return void
*/
void printLinkList(Node_t *head);

/**
* @brief Helper function to verify data of the given node
*
* Given a head node, data and position this function will verify
* data values of required linked list node
*
* FOR UNIT TESTING
*
* @param head Pointer to the Head node of the link list
* @param data Expected data which is to be verified
* @param index position of the given node
*
* @return status SUCCESS or FAILURE
*/
Status_t verifyValue(Node_t *head, uint32_t data, uint32_t index);

#endif /* DOUBLELINKLIST_H_ */

