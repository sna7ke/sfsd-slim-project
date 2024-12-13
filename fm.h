#include "Util.h"
#pragma once
Block fillBuffer(Disk d);
void creatFile (FILE *ms, Disk D, int nombreDeRecord, int mode ) ;
bool fileExists(FILE *ms, Disk D, const char* fName) ;
