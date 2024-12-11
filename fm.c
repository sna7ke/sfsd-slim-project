#include "util.h"
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
int main() {
    int bf = 3;
    Block buffer;
    Disk disk = { bf, 100 };
    FILE *ms = fopen("datafile.bin", "wb+");

    if (!ms) {
        printf("Erreur: Impossible d'ouvrir le fichier.\n");
        return 1;
    }


   buffer= fillBuffer( disk);
    printf("\nDonn�es dans le buffer :\n");
    for (int i = 0; i < buffer.num; i++) {
        printf("�tudiant %d : Nom = %s, ID = %d, Groupe = %d, Supprim� = %s\n",
               i + 1,
               buffer.student[i].name,
               buffer.student[i].ID,
               buffer.student[i].group,
               buffer.student[i].deleted ? "Oui" : "Non");
    }
}
