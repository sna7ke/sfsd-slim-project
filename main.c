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
            printf("ERROR \n");
        return 0;
    }
    printf("give me the info to initialize the disk \n");
    printf("the blocking factor :");
    scanf("%d",&D.bf);
    printf("the number of blocks :");
    scanf("%d",&D.blocks);
    D.nbrFiles=0;
    InitializeDisk(ms,D);
    Meta met;

    printf("enter the name of the file\n");
    scanf("%s",&met.nomF);
    printf("enter the global organization : 1. CHAINED , 2.CONTIGUOUS \n");
    scanf("%d",&met.orgGlobal);
    printf("enter the internal organization : 1.ORDONNEE ,2.NON ORDONEE \n");
    scanf("%d",&met.orgInterne);
    printf("how many blocks do you wish to initialize your file with? \n");
    scanf("%d",&met.tailleEnRecord);
    met.tailleEnRecord=0;



    creatFile(ms,&D,&met);
    b=ReadFAT(ms,D.blocks);
       //b=ReadFAT(ms,D.blocks);
       printf("\n");
       printf("state of the secondary memory : \n");

     for(int i=0;i<D.blocks;i++) {
        if( b[i] == true){
            printf("block number %d is : not empty \n",i);
        }else if(b[i] == false){
            printf("block number %d is : empty \n",i);
        } else{
        printf("ERROR \n");
     }}

    meta=readMeta(ms,D,1);



    printf("\n Nom du fichier : %s\n", meta.nomF);
    printf("Taille en blocs : %d\n", meta.tailleEnBlock);
    printf("Taille en enregistrements : %d\n", meta.tailleEnRecord);

    printf("Mode d'organisation globale : %d\n", meta.orgGlobal);
    printf("Mode d'organisation interne : %d\n", meta.orgInterne);

    printf("position of the file is : %d \n",meta.position);
    printf("first block is : %d \n",meta.adress1stBlock);

    InitializeBlock(D,&buffer);

    b=ReadFAT(ms,D.blocks);
    printf("\n");
       printf("state of the secondary memory : \n");

     for(int i=0;i<D.blocks;i++) {
        if( b[i] == true){
            printf("block number %d is : not empty \n",i);
        }else if(b[i] == false){
            printf("block number %d is : empty \n",i);
        } else{
        printf("ERROR \n");
     }}

    for(int i=0;i<D.blocks;i++) {
        Display_Block(i,ms,D,&buffer);
        printf("the block number %d : \n",i);
        for(int j=0;j<D.bf;j++){
            printf("   name : %s  ID : %d group : %d deleted ? : %d \n",buffer.student[j].name,buffer.student[j].ID,buffer.student[j].group,buffer.student[j].deleted);
        }
        printf("the next block is %d \n",buffer.next);

    }
    Student st;

    printf("how many students do you wish to insert ? \n");
    int num;
    scanf("%d",&num);
    for(int i =0;i<num;i++) {
        printf("enter the students name : ");
        scanf("%s",&st.name);
        printf("enter the students group :");
        scanf("%d",&st.group);
        printf("enter the students ID :");
        scanf("%d",&st.ID);
        st.deleted=false;

        insertStudent(ms,D,st,&meta);
    }

    b=ReadFAT(ms,D.blocks);
       //b=ReadFAT(ms,D.blocks);
       printf("\n");
       printf("state of the secondary memory : \n");

     for(int i=0;i<D.blocks;i++) {
        if( b[i] == true){
            printf("block number %d is : not empty \n",i);
        }else if(b[i] == false){
            printf("block number %d is : empty \n",i);
        } else{
        printf("ERROR \n");
     }}

    for(int i=0;i<D.blocks;i++) {
        Display_Block(i,ms,D,&buffer);
        printf("the block number %d : \n",i);
        for(int j=0;j<D.bf;j++){
            printf("   name : %s  ID : %d group : %d deleted ? : %d \n",buffer.student[j].name,buffer.student[j].ID,buffer.student[j].group,buffer.student[j].deleted);
        }
        printf("the next block is %d \n",buffer.next);

    }
    char name[20];
    printf("enter the name of the file you wish to delete : \n");
    scanf(" %s",name);

    fileExists(ms,D,name);
    printf("exist \n");
    deleteFile(ms,&D,name);
    printf("deletes");
    //fileExists(ms,D,name);

    Update_FAT(ms, 1, true);
    Update_FAT(ms, 3, true);

    b=ReadFAT(ms,D.blocks);
       //b=ReadFAT(ms,D.blocks);
       printf("\n");
       printf("state of the secondary memory BEFORE COMPACTAGE : \n");

     for(int i=0;i<D.blocks;i++) {
        if( b[i] == true){
            printf("block number %d is : not empty \n",i);
        }else if(b[i] == false){
            printf("block number %d is : empty \n",i);
        } else{
        printf("ERROR \n");
     }}

     printf("TESTER COMPACTAGE \n");
     compactage(ms, D);

     b=ReadFAT(ms,D.blocks);
       //b=ReadFAT(ms,D.blocks);
       printf("\n");
       printf("state of the secondary memory AFTER COMPACTAGE : \n");

     for(int i=0;i<D.blocks;i++) {
        if( b[i] == true){
            printf("block number %d is : not empty \n",i);
        }else if(b[i] == false){
            printf("block number %d is : empty \n",i);
        } else{
        printf("ERROR \n");
     }}

    free(b);
    fclose(ms);
    free(buffer.student);
    return 0;

}

