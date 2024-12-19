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
void insertStudent(FILE *ms, Disk *D, Student newStudent, Meta *meta) {
    Block buffer;
    buffer.num = 0;
    buffer.next = -1;
    for (int i=0;i<D->bf;i++) {
        buffer.student[i].ID = 0;
        buffer.student[i].group = 0;
        buffer.student[i].deleted = false;
        strcpy(buffer.student[i].name, "");
    }

    if (meta->orgGlobal == 1) {
        if (meta->orgInterne == 2) {
            int i = meta->adress1stBlock;

            while (i != -1) {
                offset(ms, *D, i);
                fread(&buffer, sizeof(Block), 1, ms);
                if (buffer.num < D->bf) {
                    buffer.student[buffer.num] = newStudent;
                    buffer.student[buffer.num].deleted = false;
                    buffer.num++;
                    fseek(ms, -sizeof(Block), SEEK_CUR);
                    fwrite(&buffer, sizeof(Block), 1, ms);

                    meta->tailleEnRecord++;
                    return;
                }

                i = buffer.next;
            }//si ya pas de place
            buffer.student[0] = newStudent;
            buffer.student[0].deleted = false;
            buffer.num = 1;
            buffer.next = -1;

            fseek(ms, 0, SEEK_END);
            fwrite(&buffer, sizeof(Block), 1, ms);//le nv  bloc est ajoutee just apres le dernier blc
            if (D->blocks > 0) {
               fseek(ms, sizeof(bool)*D->blocks + (D->blocks - 1) * sizeof(Block)+sizeof(int), SEEK_SET);
                int nvblcidc = D->blocks;
                fwrite(&nvblcidc, sizeof(int), 1, ms);
            }
            meta->tailleEnRecord++;
            D->blocks++;
            meta->tailleEnBlock++;
            return;
        }
        else if (meta->orgInterne == 1) {
            int i = meta->adress1stBlock;

            while (i != -1) {
                offset(ms, *D, i);

                fread(&buffer, sizeof(Block), 1, ms);
                for (int j = 0; j < buffer.num; j++) {
                    if (newStudent.ID < buffer.student[j].ID) {
                        for (int k = buffer.num; k > j; k--) {
                            buffer.student[k] = buffer.student[k-1];
                        }
                        buffer.student[j] = newStudent;
                        buffer.student[j].deleted = false;
                        buffer.num++;
                        fseek(ms, -sizeof(Block), SEEK_CUR);
                        fwrite(&buffer, sizeof(Block), 1, ms);
                        meta->tailleEnRecord++;
                        return;
                    }
                }
                if (buffer.next == -1 || newStudent.ID < buffer.next) {
                    if (buffer.num < D->bf) {
                        buffer.student[buffer.num] = newStudent;
                        buffer.student[buffer.num].deleted = false;
                        buffer.num++;
                        fseek(ms, -sizeof(Block), SEEK_CUR);
                        fwrite(&buffer, sizeof(Block), 1, ms);

                        meta->tailleEnRecord++;
                        return;
                    }
                }
                 // le prochain blc
                i = buffer.next;
            }

               buffer.num = 0;
        buffer.next = -1;
        for (int i = 0; i < D->bf; i++) {
        buffer.student[i].ID = 0;
        buffer.student[i].group = 0;
        buffer.student[i].deleted = false;
        strcpy(buffer.student[i].name, "");
    }
            buffer.student[0] = newStudent;
            buffer.student[0].deleted = false;
            buffer.num = 1;
            buffer.next = -1;

            // ajt un nv blc
            fseek(ms, 0, SEEK_END);
            fwrite(&buffer, sizeof(Block), 1, ms);

            // misaj  le pointeur du dernier bloc
            if (D->blocks > 0) {
               fseek(ms, sizeof(bool)*D->blocks + (D->blocks - 1) * sizeof(Block)+sizeof(int), SEEK_SET);
                int nvblcidx = D->blocks;
                fwrite(&nvblcidx, sizeof(int), 1, ms);
            }

            // misaj metadata
            meta->tailleEnRecord++;
            D->blocks++;
            meta->tailleEnBlock++;
            return;
        }
    }
    else if (meta->orgGlobal == 2) {
        if (meta->orgInterne == 2) {
            for (int i = 0; i< D->blocks; i++) {
                // Seek to the specific block
                offset(ms, *D,i);
                // lire le bloc
                fread(&buffer, sizeof(Block), 1, ms);
                 // verifier si le bloc a de lespace
                if (buffer.num < D->bf) {
                    buffer.student[buffer.num] = newStudent;
                    buffer.student[buffer.num].deleted = false;
                    buffer.num++;
                    buffer.next = -1;
                    fseek(ms, -sizeof(Block), SEEK_CUR);
                    fwrite(&buffer, sizeof(Block), 1, ms);

                    // misaj metadata
                    meta->tailleEnRecord++;
                    return;
                }
            }

            // si toutes les blocs ne conitent pas despace en ajoute un nv blc
            memset(&buffer, 0, sizeof(Block));
            buffer.student[0] = newStudent;
            buffer.student[0].deleted = false;
            buffer.num = 1;
            buffer.next = -1;

            // Aajjouter un nv blc
            fseek(ms, 0, SEEK_END);
            fwrite(&buffer, sizeof(Block), 1, ms);

            // misaj metadata
            meta->tailleEnRecord++;
            D->blocks++;
            meta->tailleEnBlock++;
            return;
        }
        else if (meta->orgInterne == 1) {
            for (int i = 0; i< D->blocks; i++) {
                offset(ms, *D, i);
                fread(&buffer, sizeof(Block), 1, ms);
               // trouverle point dinsertion dans les blocs
                for (int j = 0; j < buffer.num; j++) {
                    if (newStudent.ID < buffer.student[j].ID) {
                        // Shift records to make space
                        for (int k = buffer.num; k > j; k--) {
                            buffer.student[k] = buffer.student[k-1];
                        }

                        // Inserer un nv etudiant
                        buffer.student[j] = newStudent;
                        buffer.student[j].deleted = false;
                        buffer.num++;

                        // Overwrite block
                        fseek(ms, -sizeof(Block), SEEK_CUR);
                        fwrite(&buffer, sizeof(Block), 1, ms);

                        // misaj metadata
                        meta->tailleEnRecord++;
                        return;
                    }
                }
            }

            // Isiya pas despace en ajoute un nv
           buffer.num = 0;
    buffer.next = -1;
    for (int i = 0; i < D->bf; i++) {
        buffer.student[i].ID = 0;
        buffer.student[i].group = 0;
        buffer.student[i].deleted = false;
        strcpy(buffer.student[i].name, "");
    }
            buffer.student[0] = newStudent;
            buffer.student[0].deleted = false;
            buffer.num = 1;
            buffer.next = -1;

            //ajouter un nv bloc
            fseek(ms, 0, SEEK_END);
            fwrite(&buffer, sizeof(Block), 1, ms);

            // misajour metadata
            meta->tailleEnRecord++;
            D->blocks++;
            meta->tailleEnBlock++;
            return;
        }
    }
}
*/

void deleteFile(FILE *ms, Disk *D, char fName[20]) {
    int pos = fileExists(ms, *D, fName); // Verification de l'existance du fichier
    if (pos == -1) {
        printf("The file '%s' does not exist.\n", fName);
        return;
    }
    Meta meta = readMeta(ms, *D, pos); // Lecture des métadonnées

    // Suppression des blocs alloués au fichier
    Block defaultBlock;
    InitializeBlock(*D, &defaultBlock); // Bloc initialisé aux valeurs par défait
    if (meta.orgGlobal == CONTIG_FILE) {
        printf("Re-initializing blocks (Contiguous organization)...\n");
        for (int i = 0; i < meta.tailleEnBlock; i++) {
            offset(ms, *D, meta.adress1stBlock + i); // Déplacer le curseur au bloc spécifique
            // Réinitialiser le bloc en écrivant un bloc vide
            fwrite(defaultBlock.student, sizeof(Student), D->bf, ms);
            fwrite(&defaultBlock.num, sizeof(int), 1, ms);
            fwrite(&defaultBlock.next, sizeof(int), 1, ms);

            // Mettre à jour la FAT pour libérer le bloc
            Update_FAT(ms, meta.adress1stBlock + i, false);
        }
    } else if (meta.orgGlobal == CHAINED_FILE) {
        printf("Re-Initializing (Chained organization)...\n");
        int currentBlock = meta.adress1stBlock;
        Block buffer;

        while (currentBlock != -1) {
            offset(ms, *D, currentBlock);
            fread(&buffer, sizeof(Block), 1, ms);

            // Réinitialiser le bloc en écrivant un bloc vide
            offset(ms, *D, currentBlock);
            fwrite(defaultBlock.student, sizeof(Student), D->bf, ms);
            fwrite(&defaultBlock.num, sizeof(int), 1, ms);
            fwrite(&defaultBlock.next, sizeof(int), 1, ms);
            Update_FAT(ms, currentBlock, false); // Mettre à jour la FAT pour libérer le bloc
            currentBlock = buffer.next; // Passage au bloc suivant
        }
    }
    // Suppression des métadonnées
    if (D->nbrFiles > 1 && pos != D->nbrFiles) {
        Meta lastMeta = readMeta(ms, *D, D->nbrFiles); // Lire la dernière métadonnée
        lastMeta.position = pos; // Mettre à jour sa position
        fseek(ms, -(D->nbrFiles - pos + 1) * sizeof(Meta), SEEK_END); // Aller à la position de la métadonnée supprimée
        fwrite(&lastMeta, sizeof(Meta), 1, ms); // Écrire la dernière métadonnée à la place
    }

    D->nbrFiles--; // Diminution du nombre de fichiers par 1
    printf("File named '%s' deleted succesfully.\n", fName);
}


int binarySearch(Student *students, int numRecords, int ID) { // Fonction de recherche dichotomique (division du tableau par 2 à chaque recherche pour but d'optimisation)
    int start = 0, end = numRecords - 1;

    // Vérification de si l'ID est en dehors de l'intervalle
    if (ID < students[start].ID || ID > students[end].ID) {
        return -1; // L'ID est en dehors de l'intervalle

    while (start <= end) {
        int mid = start + (end - start) / 2;
        if (students[mid].ID == ID) {
            return mid; // Étudiant trouvé
        }
        if (students[mid].ID < ID) {
            start = mid + 1;
        } else {
            end = mid - 1;
        }
    }
    return -1; // Étudiant non trouvé
    }
}

posStudent searchStudentID(FILE *ms, Disk D, Meta meta, int ID) {
    Block buffer;
    InitializeBlock(D, &buffer); // Initialisation du buffer
    posStudent pos;
    pos.numBlock = -1;      // Valeurs par défaut si l'étudiant n'est pas trouvé
    pos.deplacement = -1;

    if (meta.orgInterne == ORDONE_FILE) {
        if (meta.orgGlobal == CONTIG_FILE) {
            for (int blockNum = 0; blockNum < meta.tailleEnBlock; blockNum++) {
                offset(ms, D, meta.adress1stBlock + blockNum); // Aller au bloc nécessaire
                fread(&buffer.num, sizeof(int), 1, ms); // Lire le nombre de students
                fread(buffer.student, sizeof(Student), D.bf, ms); // Lire students

                int foundPos = binarySearch(buffer.student, buffer.num, ID);
                if (foundPos != -1) { // Étudiant trouvé
                    pos.numBlock = blockNum;      // Numéro du Bloc où l'étudiant est trouvé
                    pos.deplacement = foundPos;   // Position dans le bloc
                    free(buffer.student);
                    return pos;
                }
            }
        } else if (meta.orgGlobal == CHAINED_FILE) {
            int currentBlock = meta.adress1stBlock;
            while (currentBlock != -1) {
                offset(ms, D, currentBlock); // Aller au bloc nécessaire
                fread(&buffer.num, sizeof(int), 1, ms); // Lire le nombre de students
                fread(buffer.student, sizeof(Student), D.bf, ms); // Lire les étudiants
                fread(&buffer.next, sizeof(int), 1, ms); // Lire le pointeur vers le prochain bloc

                int foundPos = binarySearch(buffer.student, buffer.num, ID);
                if (foundPos != -1) { // Étudiant trouvé
                    pos.numBlock = currentBlock;   // Numéro du bloc où l'étudiant est trouvé
                    pos.deplacement = foundPos;    // Position dans le bloc
                    free(buffer.student);
                    return pos;
                }
                currentBlock = buffer.next;
            }
        }
    } else if (meta.orgInterne == NONORDONE_FILE) {
            // Code te3 Adem
    }
    printf("Étudiant non trouvé.\n");
    free(buffer.student);
    return pos; // Retour de la valeur par défaut (-1, -1)
}
