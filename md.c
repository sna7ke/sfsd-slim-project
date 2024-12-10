#include "util.h"

void createMeta(FILE *ms, Meta mymeta){


    fseek(ms, 0, SEEK_END);
    fwrite(&mymeta, sizeof(Meta), 1, ms);
    }


    void readMeta(FILE *ms){
    Meta meta;
    fseek(ms, -sizeof(Meta), SEEK_END);
    fread(&meta, sizeof(Meta), 1, ms);

    printf("Nom du fichier : %s\n", meta.nomF);
    printf("Taille en blocs : %d\n", meta.tailleEnBlock);
    printf("Taille en enregistrements : %d\n", meta.tailleEnRecord);

    printf("Mode d'organisation globale : %s\n", meta.orgGlobal);
    printf("Mode d'organisation interne : %s\n", meta.orgInterne);

    }



    void majmeta(FILE *ms, int newBlocks, int newRecords){
    Meta meta;
    fseek(ms, -sizeof(Meta), SEEK_END);
    fread(&meta, sizeof(Meta), 1, ms);
    meta.tailleEnBlock = newBlocks;
    meta.tailleEnRecord = newRecords;
    fseek(ms, -sizeof(Meta), SEEK_CUR);
    fwrite(&meta, sizeof(Meta), 1, ms);
    printf("Métadonnées mises à jour avec succès.\n");
    }

