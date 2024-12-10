#include "util.h"
#pragma once
void InitializeDisk(FILE *ms ,Disk D);
bool * ReadFAT (FILE *ms,int n);
void Update_FAT(FILE *ms, int idbloc,bool v);
void InitializeBlock (Disk D,Block* B);
void Display_Block(int Block_Number,FILE*ms,Disk D,Block * buffer);
void Empty_MS (FILE *ms,Disk D);
int checkFAT(FILE *ms, Disk D, int blocsFile) ;
void offset (FILE *ms,Disk D, int Block_Number) ;
int * Allocate_Block(FILE *ms,Disk D,int nbr_blocks) ;
