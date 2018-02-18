#ifndef LINKLIST_H_
#define LINKLIST_H_

#include <stdint.h>

/* Structure of Link List Nodes */
typedef struct node{
	int data;
	struct node* next;
}Node_t;


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
Node_t* get_new_node(uint32_t data);

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
Node_t* insert_at_end(Node_t *head, uint32_t data);

/**
* @brief Function to update the data field at a given position
*
* Given a head node, this function will increment the
* data value by 1 at a given index of the node
*
* @param head Pointer to the Head node of the link list
* @param index Position where data is to be incremented
*
* @return void
*/
void updateValue(Node_t *head, uint32_t index);

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

#endif /*LINKLIST_H_*/
