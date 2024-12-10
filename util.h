#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#pragma once

typedef struct Student {
	char name[20];
	int  ID;
	int group;
	bool deleted;
}Student;

typedef struct Block {
	Student *student;
	int num;
	int next; //pointer to the next block it is going to be : -1 for contiguous files , other values for chained files
}Block;

typedef struct Disk {
	int bf; //blocking factor
	int blocks; //number of blocks used
}Disk;


