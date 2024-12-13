#include "ms.h"
#include "md.h"
#include "fm.h"

int main(void) {
    FILE * ms;
    Meta meta;
    Disk D;
    bool * b;
    Block buffer;
    ms=fopen("SecondaryMemory.bin","wb+");
    if(ms==NULL){
            printf("wow");
        return 0;
    }
    printf("give me the info to initialize the disk \n");
    printf("the blocking factor :");
    scanf("%d",&D.bf);
    printf("the number of blocks :");
    scanf("%d",&D.blocks);
    D.nbrFiles=0;
    InitializeDisk(ms,D);


    creatFile(ms,&D);
    b=ReadFAT(ms,D.blocks);
       b=ReadFAT(ms,D.blocks);
     for(int i=0;i<D.blocks;i++) {
        printf("block number %d is : %d\n",i,b[i]);
    }

    meta=readMeta(ms,D,1);



    printf("Nom du fichier : %s\n", meta.nomF);
    printf("Taille en blocs : %d\n", meta.tailleEnBlock);
    printf("Taille en enregistrements : %d\n", meta.tailleEnRecord);

    printf("Mode d'organisation globale : %d\n", meta.orgGlobal);
    printf("Mode d'organisation interne : %d\n", meta.orgInterne);

    printf("position of the file is : %d \n",meta.position);

    InitializeBlock(D,&buffer);

    for(int i=0;i<D.blocks;i++) {
        Display_Block(i,ms,D,&buffer);
        printf("the block number %d : \n",i);
        for(int j=0;j<D.bf;j++){
            printf("   name : %s  ID : %d group : %d deleted ? : %d \n",buffer.student[j].name,buffer.student[j].ID,buffer.student[j].group,buffer.student[j].deleted);
        }
        printf("the next block is %d \n",buffer.next);

    }

    fileExists(ms,D,"chahi");



    //Update_FAT(ms,1,true);





    /*for(int i=0;i<D.blocks;i++) {
        printf("block number %d is : %d",i,b[i]);
    }
    printf(" \n");
    Metajour(ms,1);
    */


    /*int * Position=NULL;
    Position=checkFAT(ms,D,2,CHAINED_FILE);
    printf("%p \n",Position);
    if(Position==NULL){
        printf("it works \n");
    }*/

    //this prints the values after a contiguous allocation

    //else {
        /*for(int i=0;i<2;i++) {
        printf("block number %d is : %d\n",*Position+i,b[i+*Position]);
    }*/
   /* Allocate_Block(ms,D,2,CHAINED_FILE);

    b=ReadFAT(ms,D.blocks);

    for(int i=0;i<D.blocks;i++) {
        printf("block number %d is : %d\n",i,b[i]);
    }
        for (int i=0;i<2;i++) {
        printf("the block : %d is : %d \n",Position[i],b[Position[i]]);
        }

         for(int i=0;i<D.blocks;i++) {
        Display_Block(i,ms,D,&buffer);
        printf("the block number %d : \n",i);
        for(int j=0;j<D.bf;j++){
            printf("   name : %s  ID : %d group : %d deleted ? : %d \n",buffer.student[j].name,buffer.student[j].ID,buffer.student[j].group,buffer.student[j].deleted);
        }
        printf("the next block is %d \n",buffer.next);

    }

    }*/

    free(b);
    fclose(ms);
    free(buffer.student);
    return 0;

}
