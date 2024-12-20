#include "util.h"
#include "md.h"
#pragma once
void InitializeDisk(FILE *ms ,Disk D);
bool * ReadFAT (FILE *ms,int n);
void Update_FAT(FILE *ms, int idbloc,bool v);
void InitializeBlock (Disk D,Block* B);
void Display_Block(int Block_Number,FILE*ms,Disk D,Block * buffer);
void Empty_MS (FILE *ms,Disk D);
int* checkFAT(FILE *ms, Disk D, int blocsFile,int Mode);
void offset (FILE *ms,Disk D, int Block_Number);
void Allocate_Block(FILE *ms, Disk D, int nbr_blocks, int mode,Meta * met);
void LoadFile(FILE *ms, Disk D, int pos);
void WriteBlock(FILE * ms,Disk D,Block buffer,int pos);
