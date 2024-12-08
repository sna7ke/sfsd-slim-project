#include "util.h"
#pragma once
void InitializeDisk(FILE *ms ,Disk D);
bool * ReadFAT (FILE *ms,int n);
void Metajour(FILE *ms, int idbloc);
void InitializeBlock (Disk D,Block* B);
void Display_Block(int Block_Number,FILE*ms,Disk D,Block * buffer);
