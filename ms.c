#include "ms.h"
//TODO : Create function to fill the disk with empty blocks and test it

void InitializeDisk(FILE *ms ,Disk D){

    //initialiser le tableu d'allocation dynamiquement a partire des nombe de blocs utilisé
    bool * t = malloc(sizeof(bool)*D.blocks);

    rewind(ms);

    for (int i=0 ; i<D.blocks ; i++){
        t[i]=false; //initialiser les valeurs du tableau

    }

    fwrite(t, sizeof(bool), D.blocks, ms); //ecrire le tableu d'allocation de fichier dans le debut de la ms

    Block buffer ;
    InitializeBlock(D,&buffer);

    // ecrire les block selon le nombre que l'utilisateur définit
    for(int i=0;i<D.blocks;i++) {
        //since the array of students varies on the blocking factor, we need to write the student's data directly
        fwrite(buffer.student,sizeof(Student),D.bf,ms);
        fwrite(&buffer.num,sizeof(int),1,ms);

    }

    free(buffer.student);

}


//metre a jour tableau dallocation

void Metajour(FILE *ms, int idbloc){
  // idbloc = le numero de bloc qui a été modifier
bool v=true;
fseek(ms, idbloc*sizeof(bool), SEEK_SET);

fwrite(&v, sizeof(bool), 1, ms);
}


void InitializeBlock (Disk D,Block *B) { //initialiser les blocks
    B->student=malloc(sizeof(Student)*D.bf);
    B->num=0;
    //a loop to initialize the block so that all the records in it will have the same elements
    for (int i=0;i<D.bf;i++){
        B->student[i].group=0;
        B->student[i].ID=0;
        strcpy(B->student[i].name,"INITIALIZED !");
    }
}

bool * ReadFAT (FILE *ms,int n) { // this function will read the file allocation table and return it for use
    rewind(ms);
    bool * b=malloc(sizeof(bool)*n);
    fread(b,sizeof(bool),n,ms);
    return b;

}

/*Block Display_Block(int Block_Number,FILE*ms,Disk D) {
    Block buffer;
    rewind(ms);
    fseek(ms,sizeof(bool)*D.blocks,SEEK_SET);
    //this moves the cursor to the end of the file allocation table then we would start reading the blocks bellow it
    fseek(ms,sizeof(Block)*Block_Number,SEEK_CUR);
    //this moves the cursor to the desired block
    fread(&buffer,sizeof(Block),1,ms);
    return buffer;

}*/

void Display_Block(int Block_Number,FILE*ms,Disk D,Block * buffer) {
    rewind(ms);
    fseek(ms,sizeof(bool)*D.blocks,SEEK_SET);
    //this moves the cursor to the end of the file allocation table then we would start reading the blocks bellow it
    fseek(ms,(sizeof(Student)*D.bf+sizeof(int))*Block_Number,SEEK_CUR);
    //this moves the cursor to the desired block
    fread(buffer->student,sizeof(Student),D.bf,ms);
    fread(buffer->num,sizeof(int),1,ms);
}


