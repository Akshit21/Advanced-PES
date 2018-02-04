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
* @file : fileIO.c
* @brief : Source file for demo file operations in c
***************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>

void main()
{
	/*Print to standard out an interesting string using printf*/
	printf("Hello @profoz this is HW2 problem 2\n");
	/*
	* Create a file
	* Modify the permissions of the file to be read/write
	* Open the file (for writing)
	*/
	FILE *fptr = fopen("test.txt","a+");
	/*Write a character to the file*/
	fputc(65, fptr);
        /*close the file*/
	fclose(fptr);
	/*Open the file (in append mode)*/
	fptr = fopen("test.txt","a");
	/*Dynamically allocate an array of memory*/
	char *buff = (char *)malloc(sizeof(char)*255);
	/*Read and input string from the command line and write to the string*/
	scanf("%s", buff);
	/*Write the string to the file*/
	fputs(buff,fptr);
	fgets(buff, 255, fptr);
	/*Flush file output*/
	fflush(fptr);
	/*close the file*/
	fclose(fptr);
	/*Open the file (in read mode)*/
	fptr = fopen("test.txt","r");
	/*Read a single character (getc)*/
	printf("%c",fgetc(fptr));
	/*Read a string of characters (gets)*/
	fgets(buff,255,fptr);
	printf("%s",buff);
	/*close the file*/
	fclose(fptr);
	/*Free the memory*/
	free(buff);
}
