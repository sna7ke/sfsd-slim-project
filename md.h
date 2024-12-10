#include "util.h"
#pragma once

void createMeta(FILE *ms, Meta mymeta);
Meta readMeta(FILE *ms);
void majmeta(FILE *ms, int newBlocks, int newRecords);
