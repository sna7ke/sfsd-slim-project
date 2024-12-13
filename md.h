#include "util.h"
#pragma once

void initMeta(Meta meta, Disk D);
void createMeta(FILE *ms, Meta mymeta);
Meta readMeta(FILE *ms, Disk D ,int pos);
void majmeta(FILE *ms, Disk D, Meta newMeta, int pos);
