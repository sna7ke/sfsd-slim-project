#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#pragma once

typedef struct Student {
	char name[20];
	int  ID;
	int group;
}Student;

typedef struct Block {
	Student *student;
	int num;
}Block;

typedef struct Disk {
	int bf; //blocking factor
	int blocks; //number of blocks used
}Disk;


