#include "Fm.h"
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

void CreateFile(FILE *ms, Disk D) { // PAS ENCORE FINIE
    // Demander nom du fichier
    char fNom[20];
    printf("File Name : "); // Temporaire avant l'implémentation de l'interface graphique
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
    int nbr_blocks = (nbr_records + D.bf - 1) / D.bf; // Il y a toujours un bloc supplémentaire d'alloué dans le cas ou le dernier n'est pas entiérement rempli

    // Initialisation des métadonnées
    Meta fMeta ;
    strncpy(fMeta.nomF, fNom, sizeof(fMeta.nomF) - 1);
    fMeta.tailleEnBlock = nbr_blocks;
    fMeta.tailleEnRecord = nbr_records;
    fMeta.adress1stBlock = NULL ; // Pour le moment , nous ne pouvons pas avoir l'adresse du premier bloc , mais nous l'initialisons ici pour une bonne structure
    fMeta.orgGlobal = orgaGlobale;
    fMeta.orgInterne = orgaInterne;
    // Ecriture des métadonnées à la fin de la MS
    createMeta(ms, fMeta);
}


bool fileExists(FILE *ms, Disk D, const char* fName) {

    // Déplacer le curseur à la fin du fichier pour accéder aux métadonnées
    fseek(ms, 0, SEEK_END);

    // Parcourir toutes les métadonnées pour vérifier si le nom existe déjà
    for (int i = D.nbrFiles - 1; i >= 0; i--) {
        long pos = ftell(ms) - sizeof(Meta) * (D.nbrFiles - i); // Position des métadonnées du fichier actuel
        fseek(ms, pos, SEEK_SET);
        Meta fMeta;
        fread(&fMeta, sizeof(Meta), 1, ms);  // Lire les métadonnées du fichier actuel

        // Comparer le nom du fichier avec celui recherché
        if (strcmp(fMeta.nomF, fName) == 0) {
            printf("The file already exists.\n", fName);
            return true;  // Le fichier existe
            break ;
        }
    }

    // Si aucun fichier n'a le même nom
    printf("The file does not exist.\n", fName);
    return false;
}
