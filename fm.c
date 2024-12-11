#include "util.h"
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

