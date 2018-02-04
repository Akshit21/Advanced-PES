/**********************************************************************
* @author : Akshit Shah
* @date : 02/03/2018
* @file : unitTest_linkList.c
* @brief : Source file for testing link list functions
*               -insert_athead_test()
*               -insert_atmiddle_test()
*               -insert_attail_test()
*               -insert_atnull_test()
*               -delete_athead_test()
*               -delete_atend_test()
*               -delete_atmiddle_test()
*               -delete_atnull_test()
*               -destroy_test()
*               -get_size_test()
*
* Tester : Akshit Shah, MS in Embedded Systems, CU Boulder
* Testing environment : To test the functions we need to install the
* Cmocka framework libraries in our folder path and make changes to
* the test Makefile to include our source files and test files.
*********************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "../doubleLinkList.h"
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

uint32_t data[5] = {20,1,43,2,13};

/****************************************************************************
* insert_athead_test() - Function to test if linked list function
* can handle adding data at the first position
* Input parameter - For this we call the insert_at_beginning function 
* with index 1
* Expected Result - Receive a SUCCESS enum
****************************************************************************/
void insert_athead_test(void **state)
{
  Node_t *head = NULL;
  head = insert_at_beginning(head, data[0]);
  head = insert_at_beginning(head, data[1]);
  head = insert_at_beginning(head, data[2]);
  Status_t status = verifyValue(head, data[2], 1);
  assert_int_equal(status,SUCCESS);
  destroy(head);
}

/****************************************************************************
* insert_atmiddle_test() - Function to test if linked list function
* can handle adding data in the middle of the list
* Input parameter - For this we add nodes at the first 2 positions and 
* last 2 positions then add data between them.
* Expected Result - Receive a SUCCESS enum
****************************************************************************/
void insert_atmiddle_test(void **state)
{
  Node_t *head = NULL;
  head = insert_at_beginning(head, data[0]);
  head = insert_at_beginning(head, data[1]);
  head = insert_at_end(head, data[2]);
  head = insert_at_end(head, data[3]);
  head = insert_at_position(head, data[4], 3);
  Status_t status = verifyValue(head, data[4], 3);
  assert_int_equal(status,SUCCESS);
  destroy(head);
}

/****************************************************************************
* insert_attail_test() - Function to test if linked list function
* can handle adding data at the last position
* Input parameter - For this we add data at the beginning twice and then add 
* data at the last position
* Expected Result - Receive a SUCCESS enum
****************************************************************************/
void insert_attail_test(void **state)
{
  Node_t *head = NULL;
  head = insert_at_beginning(head, data[0]);
  head = insert_at_beginning(head, data[1]);
  head = insert_at_end(head, data[2]);
  Status_t status = verifyValue(head, data[2], 3);
  assert_int_equal(status,SUCCESS);
  destroy(head);
}

/****************************************************************************
* insert_atnull_test() - Function to test if linked list function
* can handle adding node when head is NULL
* Input parameter - For this we add node when head is NULL, new node will be
* head
* Expected Result - Receive a SUCCESS enum
****************************************************************************/
void insert_atnull_test(void **state)
{
  Node_t *head = NULL;
  head = insert_at_beginning(head, data[0]);
  Status_t status = verifyValue(head, data[0], 1);
  assert_int_equal(status,SUCCESS);
  destroy(head);
}

/****************************************************************************
* delete_athead_test() - Function to test if linked list function
* can handle removind node from the first position
* Input parameter - For this we add some nodes to the list and then remove
* data from the first position
* Expected Result - Receive a SUCCESS enum
****************************************************************************/
void delete_athead_test(void **state)
{
  Node_t *head = NULL;
  head = insert_at_beginning(head, data[0]);
  head = insert_at_beginning(head, data[1]);
  head = insert_at_end(head, data[2]);
  head = insert_at_end(head, data[3]);
  head = insert_at_position(head, data[4], 3);
  head = delete_from_beginning(head);
  Status_t status = verifyValue(head, data[0], 1);
  assert_int_equal(status,SUCCESS);
  destroy(head);
}

/****************************************************************************
* delete_atend_test() - Function to test if linked list function
* can handle removing node from the last position
* Input parameter - For this we add data some data to the list and then
* remove data from the last position
* Expected Result - Receive a SUCCESS enum
****************************************************************************/
void delete_atend_test(void **state)
{
  Node_t *head = NULL;
  head = insert_at_beginning(head, data[0]);
  head = insert_at_beginning(head, data[1]);
  head = insert_at_end(head, data[2]);
  head = insert_at_end(head, data[3]);
  head = insert_at_position(head, data[4], 3);
  head = delete_from_end(head);
  Status_t status = verifyValue(head, data[2], 4);
  assert_int_equal(status,SUCCESS);
  destroy(head);
}

/****************************************************************************
* delete_atmiddle_test() - Function to test if linked list function
* can handle removing node from the middle of the list
* Input parameter - For this we add some data to the list and then remove
* data from the middle
* Expected Result - Receive a SUCCESS enum
****************************************************************************/
void delete_atmiddle_test(void **state)
{
  Node_t *head = NULL;
  head = insert_at_beginning(head, data[0]);
  head = insert_at_beginning(head, data[1]);
  head = insert_at_end(head, data[2]);
  head = insert_at_end(head, data[3]);
  head = insert_at_position(head, data[4], 3);
  head = delete_from_position(head,2);
  Status_t status = verifyValue(head, data[4], 2);
  assert_int_equal(status,SUCCESS);
  destroy(head);
}

/****************************************************************************
* delete_atnull_test() - Function to test if linked list
* function can handle removing node from the wrong index
* Input parameter - For this we pass NULL value and see if it returns NULL
* Expected Result - Receive a SUCCESS enum if it returns NULL
****************************************************************************/
void delete_atnull_test(void **state)
{
  Node_t *head = NULL;
  head = delete_from_beginning(head);
  assert_int_equal(head, NULL);
}


/****************************************************************************
* destroy_test() - Function to test if linked list
* function can handle removing all the nodes
* Input parameter - For this we add some nodes to the list and then try to
* remove all the nodes
* Expected Result - Receive a FAILURE enum
****************************************************************************/
void destroy_test(void **state)
{
  Node_t *head = NULL;
  head = insert_at_beginning(head, data[0]);
  head = insert_at_beginning(head, data[1]);
  head = insert_at_end(head, data[2]);
  head = insert_at_end(head, data[3]);
  head = insert_at_position(head, data[4], 3);
  destroy(head);
  Status_t status = verifyValue(head, data[4], 1);
  assert_int_equal(status,FAILURE);
}

/****************************************************************************
* get_size_test() - Function to test if linked list
* function can get correct size
* Input parameter - For this we add some nodes to the list and then try to
* find the size of the link list
* Expected Result - Receive a SUCCESS enum
****************************************************************************/
void get_size_test(void **state)
{
  Node_t *head = NULL;
  head = insert_at_beginning(head, data[0]);
  head = insert_at_beginning(head, data[1]);
  head = insert_at_end(head, data[2]);
  head = insert_at_end(head, data[3]);
  head = insert_at_position(head, data[4], 3);
  uint32_t sizedll = size(head);
  assert_int_equal(sizedll, 5);
  destroy(head);
}


/*Function testing the unit tests by passing
* function pointers to cmoka_unit_test*/
int main(int argc, char **argv)
{
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(insert_athead_test),
    cmocka_unit_test(insert_atmiddle_test),
    cmocka_unit_test(insert_attail_test),
    cmocka_unit_test(insert_atnull_test),
    cmocka_unit_test(delete_athead_test),
    cmocka_unit_test(delete_atend_test),
    cmocka_unit_test(delete_atmiddle_test),
    cmocka_unit_test(delete_atnull_test),
    cmocka_unit_test(destroy_test),
    cmocka_unit_test(get_size_test),
  };
  return cmocka_run_group_tests(tests, NULL, NULL);
}
