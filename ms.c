#include <stdbool.h>
#include "util.h"


void initialize(FILE *ms , int fb, int blocutil, disk Disk){

bool t[blocutil];
// initialiser facteur de blockage et nombre du block utilisés
Disk.bf=fb;
Disk.blocks=blocutil;

rewind(ms);
for (int i=0 ; i<blocutil ; i++){
     t[i]=false; //initialiser les valeurs du tableau
}

fwrite(t, sizeof(bool), blocutil, ms); //ecrire les valeurs dans le fichier ms

 printf("Facteur de blocage: %d\n", Disk.bf);
    printf("Nombre de blocs utilisés: %d\n", Disk.blocks);


}

//metre a jour tableau dallocation

void Metajour(FILE *ms, int idbloc){
  // idbloc = le numero de bloc qui a été modifier
bool v=true;
fseek(ms, idbloc*sizeof(bool), SEEK_SET);

fwrite(&v, sizeof(bool), 1, ms);
}


//afficher tableau de allocation

void affichertab(FILE *ms, int blocutil){
bool t[blocutil];

fread(t, sizeof(bool), blocutil, ms);
for(int i =0; i<blocutil; i++){
    printf("bloc : %d %s ", i, t[i] );
}
}








int main()
{
    printf("Hello world!\n");
    return 0;
}
