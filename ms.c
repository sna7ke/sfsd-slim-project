#include "ms.h"

//TODO : create function to allocate blocks on the ms and test it
// TODO : test the Empty_MS function once you allocate blocks to see if the ms gets empty or not

void InitializeDisk(FILE *ms ,Disk D){

    // Initialiser le tableau d'allocation dynamiquement � partir du nombre de blocs utilis�
    bool * t = malloc(sizeof(bool)*D.blocks);

    rewind(ms);

    for (int i=0 ; i<D.blocks ; i++){
        t[i]=false; // Initialiser les valeurs du tableau

    }

    fwrite(t, sizeof(bool), D.blocks, ms); // Ecrire le tableau d'allocation de fichier au debut de la MS

    Block buffer ;
    InitializeBlock(D,&buffer);

    // Ecrire les blocks selon le nombre que l'utilisateur d�finit
    for(int i=0;i<D.blocks;i++) {
        //since the array of students varies on the blocking factor, we need to write the student's data directly
        fwrite(buffer.student,sizeof(Student),D.bf,ms);
        fwrite(&buffer.num,sizeof(int),1,ms);
        fwrite(&buffer.next,sizeof(int),1,ms);

    }

    free(buffer.student);

}


// Mettre � jour tableau d'allocation

void Update_FAT(FILE *ms, int idbloc, bool v){
  // idbloc = le numero du bloc qui a �t� modifi�
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
    //if the mode is contiguose we start searching for adja�ant free blocks
    if (Mode==CONTIG_FILE) {

        while(i<D.blocks){
                //we found one element to now we start checking the adja�ant ones
            if(t[i]==false){
                int j=i;
                while (j<blocsFile && t[j]==false && j<D.blocks){
                    j++; //we increment j and we count as we find other free blocks
                }
                if(j>=blocsFile){
                    free(t); //if the counter reaches the number of block files we want that means there is enough space for one to be had
                    Position = malloc(sizeof(int));
                    *Position =i ;
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

int *Allocate_Block(FILE *ms, Disk D, int nbr_blocks, int mode) {
    // Verification du nombre de blocs demand�s
    if (nbr_blocks <= 0) {
        printf("You should allocate at least 1 block.\n");
        return NULL;
    }

    // Tableau pour les num�ros des blocs allou�s
    int *t = malloc(sizeof(int) * nbr_blocks);
    if (!t) {
        printf("ereure dans l'alocation memoire .\n");
        return NULL;
    }

    if (mode == CONTIG_FILE) {
        // Recherche des blocs contigus
        int *i = checkFAT(ms, D, nbr_blocks, CONTIG_FILE);
        if (!i ) {
            printf("Espace contigu insuffisant.\n");
            free(t);
            return NULL;
        }

        // Mise � jour de la FAT et enregistrement des blocs allou�s
        for (int j = 0; j < nbr_blocks; j++) {
            Update_FAT(ms, *i + j, true);
            t[j] = *i + j;
        }

        printf("Allocation effectu�e avec succ�s � partir du bloc n� %d.\n", *i);
        free(i);
        return t;

    } else if (mode == CHAINED_FILE) {
        // Recherche des blocs non contigus
        int *positions = checkFAT(ms, D, nbr_blocks, CHAINED_FILE);
        if (!positions) {
            printf("Espace insuffisant pour une allocation cha�n�e.\n");
            free(t);
            return NULL;
        }

        // Mise � jour de la FAT et enregistrement des blocs allou�s
        for (int j = 0; j < nbr_blocks; j++) {
            Update_FAT(ms, positions[j], true);
            t[j] = positions[j];

            // Mettre � jour le pointeur 'next' du bloc pr�c�dent
            if (j > 0) {
                offset(ms, D, t[j - 1]);
                fwrite(&t[j], sizeof(int), 1, ms);
            }
        }

        // Mettre � jour le dernier bloc pour indiquer la fin de la cha�ne
        offset(ms, D, t[nbr_blocks - 1]);
        int fin = -1;
        fwrite(&fin, sizeof(int), 1, ms);

        printf("Allocation cha�n�e effectu�e avec succ�s.\n");
        free(positions);
        return t;
    }

    printf("Mode d'allocation inconnu.\n");
    free(t);
    return NULL;
}


void LoadFile(FILE *ms, Disk D, const char *fNom) { // I'm not sure if i should read buffer.next in contiguous organisation but i put it in case , please remove it if i shouldn't (line 253 for now)
    // Lire les m�tadonn�es
    Meta fMeta ;
    fMeta = readMeta(ms);
    // Chercher le fichier dans la MS
    if (strncmp(fMeta.nomF, fNom, sizeof(fMeta.nomF)) != 0) {
        printf("Le fichier '%s' n'existe pas dans la MS.\n", fNom);
        return;
    }

    // Initialiser une structure pour stocker les blocs charg�s
    Block buffer;
    InitializeBlock(D, &buffer);

    if (fMeta.orgGlobal == CONTIG_FILE) { // Si organisation contigue
        int start = fMeta.adress1stBlock->num;
        for (int i = 0; i < fMeta.tailleEnBlock; i++) {
            offset(ms, D, start + i); // Aller au bloc correspondant
            fread(buffer.student, sizeof(Student), D.bf, ms);
            fread(&buffer.num, sizeof(int), 1, ms);
            fread(&buffer.next, sizeof(int), 1, ms);
        }
    } else if (fMeta.orgGlobal == CHAINED_FILE) { // Si organisation chain�e
        int pos = fMeta.adress1stBlock->num ; // Position pendant le parcous
        while (pos != -1) {
            offset(ms, D, pos);
            fread(buffer.student, sizeof(Student), D.bf, ms); // Lire les �tudiants
            fread(&buffer.num, sizeof(int), 1, ms);
            fread(&buffer.next, sizeof(int), 1, ms);
            pos = buffer.next;
        }
    }
    free(buffer.student);
    printf("Loading Successful.\n");
}




