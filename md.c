#include "md.h"
//TODO : the current functions only work with one metadata in mind , once we add more files it stops working , one of you should update it to be flexible
void createMeta(FILE *ms, Meta mymeta){
    fseek(ms, 0, SEEK_END);
    fwrite(&mymeta, sizeof(Meta), 1, ms);
}


Meta readMeta(FILE *ms){
    Meta meta;
    // we move the cursor to the metadata in the MS , then we read it
    fseek(ms, -sizeof(Meta), SEEK_END);
    fread(&meta, sizeof(Meta), 1, ms);
    return meta;
}



void majmeta(FILE *ms, int newBlocks, int newRecords){
    Meta meta;
    fseek(ms,-sizeof(Meta), SEEK_END);
    fread(&meta,sizeof(Meta), 1, ms);
    meta.tailleEnBlock = newBlocks;
    meta.tailleEnRecord = newRecords;
    fseek(ms,-sizeof(Meta),SEEK_CUR);
    fwrite(&meta, sizeof(Meta), 1, ms);
    printf("Métadonnées mises à jour avec succès.\n");
}

