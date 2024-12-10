#include "ms.h"
#include "md.h"

int main(void) {
    FILE * ms;
    Meta met;
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
    InitializeDisk(ms,D);
   /* printf("give me the name of the file , how many blocks it is going to take , how many elements it is going to have , the global org, and the internal org \n");
    scanf("%s",met.nomF);
    scanf("%d %d %d %d",&met.tailleEnBlock,&met.tailleEnRecord,&met.orgGlobal,&met.orgInterne);
    createMeta(ms,met);
    readMeta(ms);
    printf("Nom du fichier : %s\n", met.nomF);
    printf("Taille en blocs : %d\n", met.tailleEnBlock);
    printf("Taille en enregistrements : %d\n", met.tailleEnRecord);

    printf("Mode d'organisation globale : %d\n", met.orgGlobal);
    printf("Mode d'organisation interne : %d\n", met.orgInterne);
*/
    InitializeBlock(D,&buffer);

    for(int i=0;i<D.blocks;i++) {
        Display_Block(i,ms,D,&buffer);
        printf("the block number %d : \n",i);
        for(int j=0;j<D.bf;j++){
            printf("   name : %s  ID : %d group : %d deleted ? : %d \n",buffer.student[j].name,buffer.student[j].ID,buffer.student[j].group,buffer.student[j].deleted);
        }
        printf("the next block is %d \n",buffer.next);

    }


   /* buffer =Display_Block(0,ms,D);
    printf("these are the info of block 0 \n");
    printf("it has %d elements \n",buffer.num);

    for (int i=0; i<2;i++) {
        printf("student : %d \n",i);
        printf("name : %s ID :%d groupe : %d",buffer.student[i].name,buffer.student[i].ID,buffer.student[i].group);
    }*/
    b=ReadFAT(ms,D.blocks);


    /*for(int i=0;i<D.blocks;i++) {
        printf("block number %d is : %d",i,b[i]);
    }
    printf(" \n");
    Metajour(ms,1);
    */
    b=ReadFAT(ms,D.blocks);
     for(int i=0;i<D.blocks;i++) {
        printf("block number %d is : %d",i,b[i]);
    }
    fclose(ms);
    free(buffer.student);
    return 0;

}
