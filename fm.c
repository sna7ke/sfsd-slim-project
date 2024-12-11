#include "fm.h"
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

void CreateFile(FILE *ms, Disk D) { // PAS ENCORE FINIE
    // Demander nom du fichier
    char fNom[20];
    printf("File Name : "); // Temporaire avant l'impl�mentation de l'interface graphique
    scanf("%s", fNom);
    // Demander nombre de records
    int nbr_records;
    printf("Number of records : ");
    scanf("%d", &nbr_records);
    // Demande du mode d'organisation global
    int orgaGlobale ;
    printf("Choose global organisation mode ( 1:CONTIG_FILE / 2:CHAINED_FILE ) : ");
    scanf("%d", &orgaGlobale);
    // Demande du mode d'organisation interne
    int orgaInterne ;
    printf("Choose intern organisation mode ( 1:ORDONE_FILE / 2:NONORDONE_FILE ) : ");
    scanf("%d", &orgaInterne);
    // Calcul du nombre de blocs
    int nbr_blocks = (nbr_records + D.bf - 1) / D.bf; // Il y a toujours un bloc suppl�mentaire d'allou� dans le cas ou le dernier n'est pas enti�rement rempli

    // Initialisation des m�tadonn�es
    Meta fMeta ;
    strncpy(fMeta.nomF, fNom, sizeof(fMeta.nomF) - 1);
    fMeta.tailleEnBlock = nbr_blocks;
    fMeta.tailleEnRecord = nbr_records;
    fMeta.adress1stBlock = NULL ; // Pour le moment , nous ne pouvons pas avoir l'adresse du premier bloc , mais nous l'initialisons ici pour une bonne structure
    fMeta.orgGlobal = orgaGlobale;
    fMeta.orgInterne = orgaInterne;
    // Ecriture des m�tadonn�es � la fin de la MS
    createMeta(ms, fMeta);
}

