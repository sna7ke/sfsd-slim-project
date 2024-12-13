#include "Fm.h"
Block fillBuffer(Disk d) {
    Block buffer;

    buffer.student = malloc(d.bf * sizeof(Student));
    if (!buffer.student) {
        printf("Erreur: Impossible d'allouer de la m�moire pour le buffer.\n");
         exit(EXIT_FAILURE);
    }

    buffer.num = 0;
    buffer.next = -1;
    char temp[100];

   printf("Remplissage du buffer (maximum %d �tudiants) :\n", d.bf);

for (int i = 0; i < d.bf; i++) {
    printf("\n�tudiant %d :\n", i + 1);

    printf("Nom : ");
    scanf("%s", buffer.student[i].name);

    printf("ID : ");
    scanf("%d", &buffer.student[i].ID);

    printf("Groupe : ");
    scanf("%d", &buffer.student[i].group);

    buffer.student[i].deleted = false;

    buffer.num++;

    if (i < d.bf - 1) {
        char choice;
        printf("Voulez-vous ajouter un autre �tudiant ? (y/n) : ");
        scanf(" %c", &choice);
        if (choice == 'n' || choice == 'N') {
            break;
        }
    }
}

printf("\nLe buffer a �t� rempli avec %d �tudiants.\n", buffer.num);
return  buffer;
}


void creatFile (FILE *ms, Disk D, int nombreDeRecord, int mode ){
Meta mymeta;

int nbBlock = (nombreDeRecord/D.bf)+1; //calculer le nombre de block
int * space = checkFAT(ms, D, nbBlock,mode);

if(space == NULL){
    printf("ERREUR !!!");
}else{

    Allocate_Block(ms, D, nbBlock, mode);
    initMeta(mymeta, D);
    createMeta(ms, mymeta); //creer un fichier de metadonnee pour ce fichier
    // initialize meta
    free(space);
}
}


bool fileExists(FILE *ms, Disk D, const char* fName) {

    // D�placer le curseur � la fin du fichier pour acc�der aux m�tadonn�es
    fseek(ms, 0, SEEK_END);

    // Parcourir toutes les m�tadonn�es pour v�rifier si le nom existe d�j�
    for (int i = D.nbrFiles - 1; i >= 0; i--) {
        long pos = ftell(ms) - sizeof(Meta) * (D.nbrFiles - i); // Position des m�tadonn�es du fichier actuel
        fseek(ms, pos, SEEK_SET);
        Meta fMeta;
        fread(&fMeta, sizeof(Meta), 1, ms);  // Lire les m�tadonn�es du fichier actuel

        // Comparer le nom du fichier avec celui recherch�
        if (strcmp(fMeta.nomF, fName) == 0) {
            printf("The file already exists.\n", fName);
            return true;  // Le fichier existe
            break ;
        }
    }

    // Si aucun fichier n'a le m�me nom
    printf("The file does not exist.\n", fName);
    return false;
}
