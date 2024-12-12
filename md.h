#include "Util.h"
#pragma once

void createMeta(FILE *ms, Meta mymeta);
Meta* readMeta(FILE *ms, Disk D);
void majmeta(FILE *ms, Disk D, const char* fName, int newBlocks, int newRecords) ;
