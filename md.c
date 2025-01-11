#include "md.h"
//TODO : the current functions only work with one metadata in mind , once we add more files it stops working , one of you should update it to be flexible

void createMeta(FILE *ms, Meta mymeta){
    fseek(ms, 0, SEEK_END);
    fwrite(&mymeta, sizeof(Meta), 1, ms);
}


Meta readMeta(FILE *ms, Disk D ,int pos){
    Meta meta;
    // we move the cursor to the position of metadata in the MS , then we read it
    //the indexes of the meta data start at 1
    fseek(ms, -(D.nbrFiles-pos+1)*sizeof(Meta), SEEK_END);


    fread(&meta, sizeof(Meta), 1, ms);
    return meta;
}



void majmeta(FILE *ms, Disk D, Meta newMeta, int pos){
   Meta meta;
    fseek(ms, -(D.nbrFiles-pos+1)*sizeof(Meta), SEEK_END);
    fread(&meta,sizeof(Meta), 1, ms);
    //changer les informations necessaires
    meta.tailleEnBlock = newMeta.tailleEnBlock;
    meta.tailleEnRecord = newMeta.tailleEnRecord;
    strcpy(meta.nomF,newMeta.nomF);

    fseek(ms,-sizeof(Meta),SEEK_CUR);
    fwrite(&meta, sizeof(Meta), 1, ms);
    printf("Métadonnées mises à jour avec succès.\n");
}

