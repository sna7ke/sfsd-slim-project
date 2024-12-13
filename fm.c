#include "fm.h"
#include "md.h"

//TODO : there is a small memory issue which we could optimize , instead of doing checkfat inside allocate block we could do it once outside and pass taht array as argument for allocate block

Block fillBuffer(Disk d) {
    Block buffer;

    buffer.student = malloc(d.bf * sizeof(Student));
    if (!buffer.student) {
        printf("Erreur: Impossible d'allouer de la mémoire pour le buffer.\n");
         exit(EXIT_FAILURE);
    }

    buffer.num = 0;
    buffer.next = -1;
    char temp[100];

   printf("Remplissage du buffer (maximum %d étudiants) :\n", d.bf);

for (int i = 0; i < d.bf; i++) {
    printf("\nÉtudiant %d :\n", i + 1);

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
        printf("Voulez-vous ajouter un autre étudiant ? (y/n) : ");
        scanf(" %c", &choice);
        if (choice == 'n' || choice == 'N') {
            break;
        }
    }
}

printf("\nLe buffer a été rempli avec %d étudiants.\n", buffer.num);
return  buffer;
}


void creatFile (FILE *ms, Disk *D){
    //NOTE : THIS FUNCTION'S STRUCTURE IS TEMPORARY UNTIL WE WORK ON THE GUI
Meta meta;
 // Demander nom du fichier
    printf("File Name : "); // Temporaire avant l'implémentation de l'interface graphique
    scanf("%s", meta.nomF);
    // Demander nombre de records
    printf("Number of records : ");
    scanf("%d", &meta.tailleEnRecord);
    // Calcul du nombre de blocs
    meta.tailleEnBlock = ( (meta.tailleEnRecord/D->bf)+1 );
    // Demande du mode d'organisation global
    printf("Choose global organisation mode ( 1:CHAINED_FILE / 2:CONTIGUOUS_FILE ) : ");
    scanf("%d", &meta.orgGlobal);
    // Demande du mode d'organisation interne
    printf("Choose intern organisation mode ( 1:ORDONE_FILE / 2:NONORDONE_FILE ) : ");
    scanf("%d", &meta.orgInterne);
    // position du fichier
    meta.position=D->nbrFiles+1;

        //int nbBlock = (nombreDeRecord/D.bf)+1; //calculer le nombre de block
        printf("org global : %d \n",meta.orgGlobal);
int * space = checkFAT(ms, *D, meta.tailleEnBlock,meta.orgGlobal);

if(space == NULL){
    printf("ERREUR !!!");
}else{


    Allocate_Block(ms, *D, meta.tailleEnBlock,meta.orgGlobal,&meta);

    createMeta(ms, meta); //creer un fichier de metadonnee pour ce fichier
    D->nbrFiles++;
    free(space);
}
}


bool fileExists(FILE *ms, Disk D, const char* fName) {
    Meta met;
    // Parcourir toutes les métadonnées pour vérifier si le nom existe déjà
    for (int i =0 ; i <D.nbrFiles ; i++) {
        met=readMeta(ms,D,i+1);

        if(strcmp(met.nomF,fName)==0) {
            printf("FILE FOUND \n");
            return true;
        }

    }


    // Si aucun fichier n'a le même nom
    printf("The file does not exist.\n");
    return false;
}
