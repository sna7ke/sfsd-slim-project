
#include "util.h"
#pragma once

typedef struct Meta {
    char nomF[20];
    int tailleEnBlock; // Taille du fichier en blocs
    int tailleEnRecord;// Taille du fichier en enregistrements
    char orgGlobal[20];
    char orgInterne[20];
}Meta;


void createMeta(FILE *ms, Meta mymeta);
void readMeta(FILE *ms);
void majmeta(FILE *ms, int newBlocks, int newRecords);
