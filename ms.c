#include <stdbool.h>
#include "util.h"


void initialize(FILE *ms , int fb, int blocutil, disk Disk){

bool t[blocutil];
// initialiser facteur de blockage et nombre du block utilisés
Disk.bf=fb;
Disk.blocks=blocutil;

rewind(ms);
for (int i=0 ; i<blocutil ; i++){
     t[i]=true; //initialiser les valeurs du tableau
}

fwrite(t, sizeof(bool), blocutil, ms); //ecrire les valeurs dans le fichier ms

 printf("Facteur de blocage: %d\n", Disk.bf);
    printf("Nombre de blocs utilisés: %d\n", Disk.blocks);


}


















int main()
{
    printf("Hello world!\n");
    return 0;
}
