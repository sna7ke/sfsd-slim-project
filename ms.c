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
    fseek(ms,(sizeof(Student)*D.bf+(sizeof(int)*2))*Block_Number,SEEK_CUR);
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

int checkFAT(FILE *ms, Disk D, int blocsFile){ //blocsFile nombre de blocs dans le fichier

 bool *t = ReadFAT(ms, D.blocks);
    int i=0;
    while(i<D.blocks){
        if(t[i]==false){
            int j=0;
            while (j<blocsFile && t[j]==false){
                j++;
            }
            if(j>=blocsFile){
                free(t);
                return i;
            }
        }
        i++;
    }
    free(t);
    return -1;
}


void offset (FILE *ms,Disk D, int Block_Number){
     fseek(ms,(sizeof(Student)*D.bf+(sizeof(int)*2))*Block_Number,SEEK_SET);
}


int * Allocate_Block(FILE *ms,Disk D,int nbr_blocks) { // MANQUE DU CAS CHAINEE , POUR LE MOMENT ALLOCATION CONTIGUE SEULEMENT
    // Verification du nombre de blocs demandés
    if (nbr_blocks <= 0) {
        printf("You should allocate at least 1 bloc.\n") ;
        return NULL ;
    }
    // Recherche des blocks libres grâce au tableau FAT pour allocation contigue
    int i = checkFAT(ms, D, nbr_blocks) ; // Indice du début des blocs contigus
    if (i == -1) {
        printf("Espace contigu insuffisant") ;
        return NULL ;
    }
    // Tableau pour garder les numéros des blocks alloués
        int *t = malloc(sizeof(int) * nbr_blocks) ;
    //  Mise a jour de la FAT
        for (int j = 0; j < nbr_blocks; j++) {
            Update_FAT(ms, i + j, true) ;  // Marqué comme occupé
            t[i] = i + j ;  // Ecriture de l'indice du bloc dans chaque case de t
        }
    printf("Allocation effectuée avec succés à partir du bloc n° %d.\n",i) ;
    return t ;
}
