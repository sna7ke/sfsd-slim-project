#include <stdio.h>
#include <stdlib.h>

typedef struct Student {
	char name[20];
	int  ID;
	int group;
}Student;

typedef struct Block {
	Student *student;
	int num;
}block;

typedef struct disk {
	int bf; //blocking factor
	int blocks; //number of blocks used
}