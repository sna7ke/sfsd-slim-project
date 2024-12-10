#include "ms.h"

//TODO : create function to allocate blocks on the ms and test it
// TODO : test the Empty_MS function once you allocate blocks to see if the ms gets empty or not

void InitializeDisk(FILE *ms ,Disk D){

    // Initialiser le tableau d'allocation dynamiquement à partir du nombre de blocs utilisé
    bool * t = malloc(sizeof(bool)*D.blocks);

    rewind(ms);

    for (int i=0 ; i<D.blocks ; i++){
        t[i]=false; // Initialiser les valeurs du tableau

    }

    fwrite(t, sizeof(bool), D.blocks, ms); // Ecrire le tableau d'allocation de fichier au debut de la MS

    Block buffer ;
    InitializeBlock(D,&buffer);

    // Ecrire les blocks selon le nombre que l'utilisateur définit
    for(int i=0;i<D.blocks;i++) {
        //since the array of students varies on the blocking factor, we need to write the student's data directly
        fwrite(buffer.student,sizeof(Student),D.bf,ms);
        fwrite(&buffer.num,sizeof(int),1,ms);
        fwrite(&buffer.next,sizeof(int),1,ms);

    }

    free(buffer.student);

}


// Mettre à jour tableau d'allocation

void Update_FAT(FILE *ms, int idbloc, bool v){
  // idbloc = le numero du bloc qui a été modifié
fseek(ms, idbloc*sizeof(bool), SEEK_SET);

fwrite(&v, sizeof(bool), 1, ms);
}


void InitializeBlock (Disk D,Block *B) { //initialiser les blocks
    B->student=malloc(sizeof(Student)*D.bf);
    B->num=0;
    B->next=-1;
    //a loop to initialize the block so that all the records in it will have the same elements
    for (int i=0;i<D.bf;i++){
        B->student[i].group=0;
        B->student[i].ID=0;
        strcpy(B->student[i].name,"INITIALIZED !");
        B->student[i].deleted=false;
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
    //fseek(ms,(sizeof(Student)*D.bf+(sizeof(int)*2))*Block_Number,SEEK_CUR);
    offset(ms,D,Block_Number);
    //this moves the cursor to the desired block
    fread(buffer->student,sizeof(Student),D.bf,ms);
    printf("read students");
    fread(&buffer->num,sizeof(int),1,ms);
    printf("read the number");
    fread(&buffer->next,sizeof(int),1,ms);
    printf("read the next");
}

void Empty_MS (FILE *ms,Disk D){
    //set all the blocks to be free
    for(int i=0;i<D.blocks;i++){
        Update_FAT(ms,i,false);
    }
    Block buffer;
    InitializeBlock(D,&buffer);
    //we initialize the buffer here to hold dummy empty values to write them on the ms
     for(int i=0;i<D.blocks;i++) {
        //since the array of students varies on the blocking factor, we need to write the student's data directly
        fwrite(buffer.student,sizeof(Student),D.bf,ms);
        fwrite(&buffer.num,sizeof(int),1,ms);
        fwrite(&buffer.next,sizeof(int),1,ms);

    }


}


int * checkFAT(FILE *ms, Disk D, int blocsFile,int Mode){ //blocsFile nombre de blocs dans le fichier

    int *Position =NULL; //pointer that i will return later and it will give out the position of the free block/blocks
    bool *t = ReadFAT(ms, D.blocks);

    int i=0;
    //if the mode is contiguose we start searching for adjaçant free blocks
    if (Mode==CONTIG_FILE) {

        while(i<D.blocks){
                //we found one element to now we start checking the adjaçant ones
            if(t[i]==false){
                int j=i;
                while (j<blocsFile && t[j]==false && j<D.blocks){
                    j++; //we increment j and we count as we find other free blocks
                }
                if(j>=blocsFile){
                    free(t); //if the counter reaches the number of block files we want that means there is enough space for one to be had
                    Position = malloc(sizeof(int));
                    *Position =i
                    return Position;
                }
                i=j;
                    //else we'll continue looping through the array
                }
                i++;
            }
        }
    else if(Mode==CHAINED_FILE) {
            int k=0;
            Position = malloc(sizeof(int)*blocsFile); //we allocate an array enough to hold all blocks
        for(int i=0;i<D.blocks;i++){
            if(t[i]==false){
                Position[k]=i;
                k++;
            }
            if(k>=blocsFile){
                return Position;
            }
        }
    }

    free(t);
    return NULL;
}


void offset (FILE *ms,Disk D, int Block_Number){
    fseek(ms,sizeof(bool)*D.blocks,SEEK_SET);
    fseek(ms,(sizeof(Student)*D.bf+(sizeof(int)*2))*Block_Number,SEEK_CUR);
}

