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

    b=ReadFAT(ms,D.blocks);
    Update_FAT(ms,0,true);
    Update_FAT(ms,2,true);
    Update_FAT(ms,3,true);
    //Update_FAT(ms,1,true);





    /*for(int i=0;i<D.blocks;i++) {
        printf("block number %d is : %d",i,b[i]);
    }
    printf(" \n");
    Metajour(ms,1);
    */
    b=ReadFAT(ms,D.blocks);
     for(int i=0;i<D.blocks;i++) {
        printf("block number %d is : %d\n",i,b[i]);
    }

    int * Position=NULL;
    Position=checkFAT(ms,D,2,CHAINED_FILE);
    printf("%p \n",Position);
    if(Position==NULL){
        printf("it works \n");
    }

    //this prints the values after a contiguous allocation

    else {
        /*for(int i=0;i<2;i++) {
        printf("block number %d is : %d\n",*Position+i,b[i+*Position]);
    }*/
    Allocate_Block(ms,D,2,CHAINED_FILE);

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

    }

    free(b);
    fclose(ms);
    free(buffer.student);
    return 0;

}
