#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define CHAINED_FILE 1
#define CONTIG_FILE 2

#define ORDONE_FILE 1
#define NONORDONE_FILE 2

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
	int nbrFiles ; // total number of files
}Disk;


typedef struct Meta {
    char nomF[20];
    int tailleEnBlock; // Taille du fichier en blocs
    int tailleEnRecord;// Taille du fichier en enregistrements
    int adress1stBlock;
    int orgGlobal;
    int orgInterne;
    int position;
}Meta;

typedef struct posStudent {
            int numBlock ;
            int deplacement ;
}posStudent;
