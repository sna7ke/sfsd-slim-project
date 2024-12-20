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
    if ( meta.tailleEnRecord % D->bf == 0 ){
        meta.tailleEnBlock =  meta.tailleEnRecord/D->bf ;
    }else{
        meta.tailleEnBlock = ( (meta.tailleEnRecord/D->bf)+1 );
    }
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
      meta.adress1stBlock = *space;
if(space == NULL){
    printf("ERREUR f creat !!! \n");
}else{


    Allocate_Block(ms, *D, meta.tailleEnBlock,meta.orgGlobal,&meta);

    createMeta(ms, meta); //creer un fichier de metadonnee pour ce fichier
    D->nbrFiles++;
    free(space);
}
}







int fileExists(FILE *ms, Disk D, char fName[20]) {
    Meta met;
    // Parcourir toutes les métadonnées pour vérifier si le nom existe déjà
    for (int i =0 ; i <D.nbrFiles ; i++) {
        met=readMeta(ms,D,i+1);

        if( strcmp( met.nomF, fName) == 0) {
            printf("FILE FOUND \n");
            return met.position;
        }
    }
    // Si aucun fichier n'a le même nom
    printf("The file does not exist.\n");
    return -1;
}











/*
void insertStudent(FILE *ms, Disk D, Student newStudent, Meta *meta) {
     Block buffer;
     InitializeBlock (D,&buffer);
      if (meta->orgGlobal == 1) {
        if (meta->orgInterne == 2) {
            int i = meta->adress1stBlock;
            while (i != -1) {
                offset(ms, D, i);
                Display_Block(i,ms,D,&buffer);
                if (buffer.num < D.bf){    // si il ya de place
                    buffer.student[buffer.num] = newStudent;
                    buffer.num++;
                    offset(ms, D, i); // fseek(ms, -sizeof(Block), SEEK_CUR);

                    fwrite(buffer.student,sizeof(Student),D.bf,ms);
                    fwrite(&buffer.num,sizeof(int),1,ms);
                    fwrite(&buffer.next,sizeof(int),1,ms);

                    meta->tailleEnRecord++;
                   // meta->tailleEnBlock++;

                    return; //break
                }
                i = buffer.next;

            }
            //si ya pas de place

               //chainage

            int * i2 = checkFAT(ms, D, 1, CHAINED_FILE);

             /* if (!i2) {
            fprintf(stderr, "Erreur : Impossible de trouver un bloc libre dans la FAT\n");
            return;
            }  */

            offset(ms, D, *i2);
            Display_Block(*i2,ms,D,&buffer);
            buffer.next = *i2;

            fseek(ms, -sizeof(Block), SEEK_CUR);
            fwrite(buffer.student,sizeof(Student),D.bf,ms);
            fwrite(&buffer.num,sizeof(int),1,ms);
            fwrite(&buffer.next,sizeof(int),1,ms);


            Update_FAT(ms, *i2, true);

            //meta->tailleEnBlock++;

            // fill new student


            InitializeBlock(D, &buffer);
            buffer.student[0] = newStudent;

            /*buffer.student[0].deleted = false;*/

            buffer.num = 1;
            buffer.next = -1;


            offset(ms, D, *i2);

            fwrite(buffer.student,sizeof(Student),D.bf,ms);
            fwrite(&buffer.num,sizeof(int),1,ms);
            fwrite(&buffer.next,sizeof(int),1,ms); //le nv  bloc est ajoutee just apres le dernier blc

            D.blocks++;


            meta->tailleEnRecord++;
            //meta->tailleEnBlock++;
            free(i2);
            return;


      }
      else if (meta->orgInterne == 1) {
        int i = meta->adress1stBlock;
        while (i != -1) {
            offset(ms, D, i);
            fread(&buffer, sizeof(Block), 1, ms);
            for (int j = 0; j < buffer.num; j++) {
                if (newStudent.ID < buffer.student[j].ID) {
                        for (int k = buffer.num; k > j; k--) {
                            buffer.student[k] = buffer.student[k-1];
                        }                        buffer.student[j] = newStudent;
                buffer.student[j].deleted = false;
                buffer.num++;
                fseek(ms, -sizeof(Block), SEEK_CUR);
                fwrite(&buffer, sizeof(Block), 1, ms);
                meta->tailleEnRecord++;
                return;
                }
              }

                if (buffer.next == -1) {
                    if (buffer.num < D.bf) {
                        buffer.student[buffer.num] = newStudent;                        buffer.student[buffer.num].deleted = false;
                        buffer.num++;
                        fseek(ms, -sizeof(Block), SEEK_CUR);
                        fwrite(&buffer, sizeof(Block), 1, ms);
                        meta->tailleEnRecord++;
                        return;
                   }
                }// le prochain blc
                     i = buffer.next;
            }
            InitializeBlock ( D, &buffer);
            buffer.student[0] = newStudent;
            buffer.student[0].deleted = false;// ajt un nv blc
            buffer.num = 1;
            buffer.next = -1;
            fseek(ms, 0, SEEK_END);
            fwrite(&buffer, sizeof(Block), 1, ms);// misaj  le pointeur du dernier bloc
            if (D.blocks > 0) {
                fseek(ms, sizeof(bool)*D.blocks + (D.blocks - 1) * sizeof(Block)+sizeof(int), SEEK_SET);
                int nvblcidx = D.blocks;
                fwrite(&nvblcidx, sizeof(int), 1, ms);
            }// misaj metadata
            meta->tailleEnRecord++;
            D.blocks++;
            meta->tailleEnBlock++;
            return;        }
    }else if (meta->orgGlobal == 2) {
          if (meta->orgInterne == 2) {
            for (int i = 0; i< D.blocks; i++) {// Seek to the specific block
            offset(ms, D,i);// lire le bloc
            fread(&buffer, sizeof(Block), 1, ms);// verifier si le bloc a de lespace
               if (buffer.num < D.bf) {
                    buffer.student[buffer.num] = newStudent;
                    buffer.student[buffer.num].deleted = false;
                    buffer.num++;
                    buffer.next = -1;
                    fseek(ms, -sizeof(Block), SEEK_CUR);
                    fwrite(&buffer, sizeof(Block), 1, ms);// misaj metadata                    meta->tailleEnRecord++;
                    return;
                    }
             }     // si toutes les blocs ne conitent pas despace en ajoute un nv blc            InitializeBlock ( D,&buffer);
             buffer.student[0] = newStudent;
             buffer.student[0].deleted = false;
             buffer.num = 1;
             buffer.next = -1;
             fseek(ms, 0, SEEK_END);
             fwrite(&buffer, sizeof(Block), 1, ms);// misaj metadata
             meta->tailleEnRecord++;
             D.blocks++;
             meta->tailleEnBlock++;
             return;
        } else if (meta->orgInterne == 1) {
            for (int i = 0; i< D.blocks; i++) {
                offset(ms, D, i);
                fread(&buffer, sizeof(Block), 1, ms);//trouverle point dinsertion dans les blocs
                for (int j = 0; j < buffer.num; j++) {
                        if (newStudent.ID < buffer.student[j].ID) {// Shift records to make space
                            for (int k = buffer.num; k > j; k--) {
                            buffer.student[k] = buffer.student[k-1];
                            }
                            // Inserer un nv etudiant
                            buffer.student[j] = newStudent;                        buffer.student[j].deleted = false;
                            buffer.num++;// Overwrite block                        fseek(ms, -sizeof(Block), SEEK_CUR);
                            fwrite(&buffer, sizeof(Block), 1, ms);// misaj metadata                        meta->tailleEnRecord++;
                            return;                    }
                }
            }// Isiya pas despace en ajoute un nv
            InitializeBlock (D,&buffer);
            buffer.student[0] = newStudent;
            buffer.student[0].deleted = false;
            buffer.num = 1;
            buffer.next = -1;//ajouter un nv bloc
            fseek(ms, 0, SEEK_END);
            fwrite(&buffer, sizeof(Block), 1, ms);// misajour metadata
            meta->tailleEnRecord++;
            D.blocks++;
            meta->tailleEnBlock++;
            return;        }

}
}

