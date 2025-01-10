#include "ms.h"
#include "md.h"
#include "fm.h"

int main(void) {
    FILE * ms;
    Meta meta, nvMeta;
    Student st;
    char name[20];
    int fonction, insere, i, pos;
    Disk D;
    bool arret = false;
    bool * b;
    Block buffer;
    ms=fopen("SecondaryMemory.bin","wb+");
    if(ms==NULL){
            printf(" Erreur d'initialisation de la mémoire secondaire ! ");
        return 0;
    }

    printf(" Veuillez fournir les informations suivantes pour initialiser le disque : \n");
    printf(" Facteur de blockage : ");
    scanf(" %d",&D.bf);
    printf(" Nombre de blocs à utiliser : ");
    scanf(" %d",&D.blocks);
    printf("\n");
    D.nbrFiles=0;

    InitializeDisk(ms,D);
    printf("Disque initialisé avec succes !\n");
    printf("\n");  printf("\n");





    do{

        printf("voici les fonctionalitees disponibles: \n");
        printf(" 1.Creer un fichier           2.Insere                    3.supprimer un fichier \n");
        printf(" 4.Renommer un fichier        5.Afficher un fichier \n");
        printf(" 6.Compactage                 7.Deffragmentation          8.Vider la ms \n");
        printf("Pour choisir une fonctionnalite, veuillez entrer le numero correspondant: \n");
        scanf("%d", &fonction);
        printf("\n");


        switch (fonction)
        {
        case 1:
            printf("Entrer le nom du fichier : \n");
            scanf(" %s",meta.nomF);
            printf("Entrer le nombre de block a utiliser : \n");
            scanf("%d",&meta.tailleEnBlock);

            printf("Entrer le mode d'organization global : 1.CHAINED , 2.CONTIGUOUS \n");
            scanf("%d",&meta.orgGlobal);

            printf("Entrer le mode d'organization interne : 1.ORDONNE , 2.NONRDONEE \n");
            scanf("%d",&meta.orgInterne);
            meta.tailleEnRecord=0;

            creatFile ( ms, &D, &meta);
            break;

        case 2:
            printf("Voici la liste de tous les fichiers existants : \n");

           for(int i=1 ; i<=D.nbrFiles ; i++){
               nvMeta = readMeta(ms, D ,i);
               printf("%d. %s \n", i , nvMeta.nomF);

           }
           printf("Choisissez un fichier pour l'insertion (veuillez entrer le numéro correspondant) : \n");
           scanf("%d", &pos);
            meta = readMeta(ms, D, pos);

           printf("Entrez le nombre d'éléments que vous souhaitez insérer : \n");
           scanf("%d", &insere);
            printf("\n");

        for(int i =0;i<insere ;i++) {
        printf("enter the students name : ");
        scanf("%s",&st.name);
        printf("enter the students group : ");
        scanf("%d",&st.group);
        printf("enter the students ID : ");
        scanf("%d",&st.ID);
        st.deleted=false;
        insertStudent(ms,D,st,&meta);
        }

            break;

        case 3:
            printf(" Entrer le nom du fichier : ");
            scanf("%s", &name);
            deleteFile(ms, &D, name);
            printf("\n"); ////////////////////////////
            break;

        case 4:


        printf(" Entrer le nom du fichier : ");
        scanf("%s", &name);
        i = fileExists(ms, D, name );
        nvMeta = readMeta(ms, D ,i);
        printf(" Entrer le nouveau nom : ");
        scanf("%s", &name);
        strcpy(nvMeta.nomF, name);
        majmeta(ms, D, nvMeta, i);

        break;

        case 5:

            printf(" Entrer le nom du fichier : ");
            scanf("%s", &name);
            i = fileExists(ms, D, name );
            LoadFile(ms, D, i);
        break;

        case 6:
            compactage(ms, D);
        break;

        case 7:
            //defraa
        break;

        case 8:
            Empty_MS(ms, D);
        break;

        default:
            printf("Erreur");
            break;

    }


    char c;


    do {
        printf("Voulez-vous fermer le programme ? (Y/N) : ");
        scanf(" %c", &c);

        if (c != 'Y' && c != 'y' && c != 'N' && c != 'n') {
            printf("Valeur inconnue, veuillez réessayer.\n");
        }
    } while (c != 'Y' && c != 'y' && c != 'N' && c != 'n');



    if (c == 'Y' || c == 'y') {
        arret = true;
        printf("Fermeture du programme...\n");
    } else {
        printf("Le programme continue...\n");
    }




    }while( arret == false);




    free(b);
    fclose(ms);
    free(buffer.student);
    return 0;

}

