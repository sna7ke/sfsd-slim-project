#include "util.h"
void InitializeDisk(FILE *ms ,Disk D);
bool * ReadFAT (FILE *ms,int n);
void Metajour(FILE *ms, int idbloc);