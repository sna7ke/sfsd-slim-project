#include "ms.h"
//TODO : Create function to fill the disk with empty blocks and test it
void InitializeDisk(FILE *ms ,Disk D){

//initialiser le tableu d'allocation dynamiquement a partire des nombe de blocs utilisé
bool * t = malloc(sizeof(bool)*D.blocks);

// initialiser facteur de blockage et nombre du block utilisés

rewind(ms);

for (int i=0 ; i<D.blocks ; i++){
     t[i]=true; //initialiser les valeurs du tableau

}

fwrite(t, sizeof(bool), D.blocks, ms); //ecrire les valeurs dans le fichier ms

    /*printf("Facteur de blocage: %d\n", D.bf);
    printf("Nombre de blocs utilisés: %d\n", D.blocks);*/

}


//metre a jour tableau dallocation

void Metajour(FILE *ms, int idbloc){
  // idbloc = le numero de bloc qui a été modifier
bool v=true;
fseek(ms, idbloc*sizeof(bool), SEEK_SET);

fwrite(&v, sizeof(bool), 1, ms);
}


void InitializeBlock (Disk D,Block B) { //initialiser les blocks
    B.student=malloc(sizeof(Student)*D.bf);
    B.num=0;


}

bool * ReadFAT (FILE *ms,int n) { // this function will read the file allocation table and return it for use
    rewind(ms);
    bool * b=malloc(sizeof(bool)*n);
    fread(b,sizeof(bool),n,ms);
    return b;

}

