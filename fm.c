#include "fm.h"
#include "md.h"

//TODO : there is a small memory issue which we could optimize , instead of doing checkfat inside allocate block we could do it once outside and pass taht array as argument for allocate block

Block fillBuffer(Disk d) {
    Block buffer;

   InitializeBlock(d,&buffer);
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


void creatFile (FILE *ms, Disk *D,Meta *m){
    //NOTE : THIS FUNCTION'S STRUCTURE IS TEMPORARY UNTIL WE WORK ON THE GUI
Meta meta;


    meta.tailleEnRecord=0;

    meta=*m;

    printf("File Name : %s\n",meta); // Temporaire avant l'impl�mentation de l'interface graphique
    printf("Number of Blocks you wish to initialize your file with :%d \n",meta.tailleEnBlock);
    printf("Choose global organisation mode  :%d \n",meta.orgGlobal);

    // Demande du mode d'organisation interne
    printf("Choose intern organisation mode :%d \n",meta.orgInterne);

    // position du fichier
    meta.position=D->nbrFiles+1;

        //int nbBlock = (nombreDeRecord/D.bf)+1; //calculer le nombre de block

        int * space = checkFAT(ms, *D,meta.tailleEnBlock,meta.orgGlobal);
        // meta.adress1stBlock = *space;
        if(space == NULL){
        printf("Not enough space to create the file!!! \n");
        }
        else{
                printf("check \n");



        Allocate_Block(ms, *D,& meta.tailleEnBlock,&meta.orgGlobal,&meta);

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
        printf("this is going to be the name from comparing");
        printf("%s \n",fName);
        printf("this is going to be the name from meta");
        printf("%s \n",met.nomF);
        if( strcmp( met.nomF, fName) == 0) {
            printf("FILE FOUND \n");
            return met.position;
        }
    }
    // Si aucun fichier n'a le même nom
    printf("The file does not exist.\n");
    return -1;
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
            if (meta.orgGlobal == 2) {
                int i=meta.adress1stBlock;
                for (int k = 0; k < D.blocks; k++) {
                    offset(ms, D, i);
                    fread(&buffer, sizeof(Block), 1,ms);
                    for (int j = 0; j < buffer.num; j++) {
                        if (buffer.student[j].ID == ID) {
                        pos.deplacement=j;
                        pos.numBlock=i;
                        free(buffer.student);
                    return pos;                }
                    }
                i=i+1;
                }
            } else if(meta.orgGlobal==1){
                int i=meta.adress1stBlock;
                while (i!=-1) {
                    offset(ms,D,i);
                    fread(&buffer,sizeof(Block),1,ms);
                    for (int j = 0; j < buffer.num; j++) {
                        if (buffer.student[j].ID == ID) {
                            pos.deplacement=j;
                            pos.numBlock=i;
                            free(buffer.student);
                            return pos;
                        }
                    }
                    i=buffer.next;
                }
            }
    }
    printf("Étudiant non trouvé.\n");
    free(buffer.student);
    return pos; // Retour de la valeur par défaut (-1, -1)
}

void insertStudent(FILE *ms, Disk D, Student newStudent, Meta *meta) {
    Block buffer;
    InitializeBlock(D,&buffer);

    if (meta->orgGlobal == CHAINED_FILE) {

        if (meta->orgInterne == NONORDONE_FILE) {


            int i = meta->adress1stBlock;
            while (i != -1) {
                offset(ms, D, i);
                Display_Block(i,ms,D,&buffer);

                //printf("stuck azbi \n");

                if (buffer.num < D.bf){ // si il ya de place

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

                if(buffer.next==-1){
                    printf("going to allocate a new block now \n");
                    break;
                }
                i = buffer.next;

            }
            //si ya pas de place

               //chainage

            int * i2 = checkFAT(ms, D, 1, CHAINED_FILE);

              if (i2==NULL) {
            printf("Erreur : Impossible de trouver un bloc libre dans la FAT\n");
            return;
            }

            //trying to fix this shit
            buffer.next= *i2;
            offset(ms,D,i);
            writeblock(ms,buffer,D);

            //offset(ms, D, *i2);

            Display_Block(*i2,ms,D,&buffer);

            /*fseek(ms, -sizeof(Block), SEEK_CUR);
            fwrite(buffer.student,sizeof(Student),D.bf,ms);
            fwrite(&buffer.num,sizeof(int),1,ms);
            fwrite(&buffer.next,sizeof(int),1,ms);*/


            Update_FAT(ms, *i2, true);

            //meta->tailleEnBlock++;

            // fill new student


            //InitializeBlock(D, &buffer);
            buffer.student[0] = newStudent;

            /*buffer.student[0].deleted = false;*/

            buffer.num = 1;
            buffer.next = -1;


            offset(ms, D, *i2);

            fwrite(buffer.student,sizeof(Student),D.bf,ms);
            fwrite(&buffer.num,sizeof(int),1,ms);
            fwrite(&buffer.next,sizeof(int),1,ms); //le nv  bloc est ajoutee just apres le dernier blc

            meta->tailleEnRecord++;
            free(i2);
            free(buffer.student);
            printf("success\n");
            return;

        }

      else if (meta->orgInterne == ORDONE_FILE) {
      Block buffer;
        InitializeBlock(D, &buffer);
        posStudent pos = {0, 0};
        int currentBlock = meta->adress1stBlock;
        int blockCount = 0;
        bool found = false;
        if (D.bf == 1) {
                int insertBlock = meta->adress1stBlock;
                Student tempStudent = newStudent;
                Student nextStudent;
                while (insertBlock != -1 && !found) {
                    offset(ms, D, insertBlock);
                    Display_Block(insertBlock, ms, D, &buffer);
                    if (buffer.num > 0) {
                        if (newStudent.ID < buffer.student[0].ID) {
                            found = true;
                        } else {
                            insertBlock = buffer.next;
                        }
                    } else {

                        found = true;
                    }
                }// If we haven't found a position, it goes at the end
                if (!found) {// Find the last block
                    insertBlock = meta->adress1stBlock;
                    while (buffer.next != -1) {
                        insertBlock = buffer.next;
                        offset(ms, D, insertBlock);
                        Display_Block(insertBlock, ms, D, &buffer);
                    }
                }// Second pass: insert and shift
                while (insertBlock != -1) {
                    offset(ms, D, insertBlock);
                    Display_Block(insertBlock, ms, D, &buffer);
                    if (buffer.num == 0) {// Empty block, just insert
                        buffer.student[0] = tempStudent;
                        buffer.num = 1;
                        WriteBlockwPos(ms , D , buffer,insertBlock);
                        break;
                    } else {// Save current student and insert new one
                        nextStudent = buffer.student[0];
                        buffer.student[0] = tempStudent;
                        WriteBlockwPos(ms , D , buffer,insertBlock);
                        tempStudent = nextStudent;
                    }
                    if (buffer.next == -1 && insertBlock < D.blocks - 1) {// Need to chain to next block if available
                        bool* fat = ReadFAT(ms, D.blocks);
                        int nextBlock = insertBlock + 1;
                        if (!fat[nextBlock]) {// Update current block's next pointer
                            buffer.next = nextBlock;
                            offset(ms, D, insertBlock);
                            fseek(ms, sizeof(Student) * D.bf + sizeof(int), SEEK_CUR);
                            fwrite(&buffer.next, sizeof(int), 1, ms);// Set up new block
                            Update_FAT(ms, nextBlock, true);
                            insertBlock = nextBlock;
                        }
                        free(fat);
                    } else {
                        insertBlock = buffer.next;
                    }
                }
                meta->tailleEnRecord++;
                free(buffer.student);
                return;
            }// First find insertion position
        while (currentBlock != -1 && !found) {
            offset(ms, D, currentBlock);
            Display_Block(currentBlock, ms, D, &buffer);// Find first position where new ID is less than existing
            for (int j = 0; j < buffer.num; j++) {
                if (newStudent.ID < buffer.student[j].ID) {
                    pos.deplacement = j;
                    pos.numBlock = blockCount;
                    found = true;
                    break;
                }
            }
            if (!found) {// If block has space, insert at end
                if (buffer.num < D.bf) {
                    pos.deplacement = buffer.num;
                    pos.numBlock = blockCount;
                    found = true;
                } else if (buffer.next == -1) {// Last block and full - will need new block
                    pos.deplacement = D.bf;
                    pos.numBlock = blockCount;
                    found = true;
                } else {
                    currentBlock = buffer.next;
                    blockCount++;
                }
            }
        }// Now handle the insertion with proper shifting
        currentBlock = meta->adress1stBlock;
        Student tempStudent = newStudent;
        blockCount = 0;
        while (currentBlock != -1) {
            offset(ms, D, currentBlock);
            Display_Block(currentBlock, ms, D, &buffer);
            if (blockCount == pos.numBlock) {// Check next block for available space
                Block nextBuffer;
                bool nextHasSpace = false;
                if (buffer.next != -1) {
                    InitializeBlock(D, &nextBuffer);
                    offset(ms, D, buffer.next);
                    Display_Block(buffer.next, ms, D, &nextBuffer);
                    nextHasSpace = nextBuffer.num < D.bf;
                }
                if (buffer.num < D.bf) {// Simple insertion - space available
                    for (int i = buffer.num; i > pos.deplacement; i--) {
                        buffer.student[i] = buffer.student[i-1];
                    }
                    buffer.student[pos.deplacement] = tempStudent;
                    buffer.num++;
                    offset(ms, D, currentBlock);
                    writeblock(ms, buffer, D);
                    meta->tailleEnRecord++;
                    break;
                } else if (nextHasSpace) {// Shift to next block that has space
                    Student shiftStudent = buffer.student[D.bf - 1];
                    for (int i = D.bf - 1; i > pos.deplacement; i--) {
                        buffer.student[i] = buffer.student[i-1];
                    }
                    buffer.student[pos.deplacement] = tempStudent;// Write current block
                    offset(ms, D, currentBlock);
                    writeblock(ms, buffer, D);// Shift into next block
                    for (int i = nextBuffer.num; i > 0; i--) {
                        nextBuffer.student[i] = nextBuffer.student[i-1];
                    }
                    nextBuffer.student[0] = shiftStudent;
                    nextBuffer.num++;
                    offset(ms, D, buffer.next);
                    writeblock(ms, nextBuffer, D);
                    meta->tailleEnRecord++;
                    break;
                } else {// Need new block
                    int* newBlockPos = checkFAT(ms, D, 1, CHAINED_FILE);
                    if (newBlockPos == NULL) {
                        printf("Error: No space available for new block\n");
                        free(buffer.student);
                        return;
                    }// Handle overflow
                    Student overflowStudent = buffer.student[D.bf - 1];
                    for (int i = D.bf - 1; i > pos.deplacement; i--) {
                        buffer.student[i] = buffer.student[i-1];
                    }
                    buffer.student[pos.deplacement] = tempStudent;// Update current block
                    buffer.next = *newBlockPos;
                    offset(ms, D, currentBlock);
                    writeblock(ms, buffer, D);// Create new block with overflow
                    Block newBlock;
                    InitializeBlock(D, &newBlock);
                    newBlock.student[0] = overflowStudent;
                    newBlock.num = 1;
                    newBlock.next = -1;
                    offset(ms, D, *newBlockPos);
                    writeblock(ms, newBlock, D);
                    Update_FAT(ms, *newBlockPos, true);
                    meta->tailleEnRecord++;
                    meta->tailleEnBlock++;
                    free(newBlock.student);
                    free(newBlockPos);
                    break;
                }
            }
            currentBlock = buffer.next;
            blockCount++;
        }
        free(buffer.student);
            }
            }

    else if (meta->orgGlobal == CONTIG_FILE) {

        if (meta->orgInterne == NONORDONE_FILE) {

            int k=meta->adress1stBlock;

            for (int i = 0; i< meta->tailleEnBlock; i++) {
                // Seek to the specific blocks
                Display_Block(k,ms,D,&buffer);
                printf("the number of shit in the buffer : %d \n",buffer.num);
                printf("the number of shit in the blocking factor : %d \n",D.bf);
                 // verifier si le bloc a de lespace
                if (buffer.num < D.bf) {

                    printf("HELLO?");
                    buffer.student[buffer.num] = newStudent;
                    buffer.student[buffer.num].deleted = false;
                    buffer.num++;

                    buffer.next = -1;
                    offset(ms,D,k);

                    fwrite(buffer.student,sizeof(Student),D.bf,ms);
                    printf("the num of buffer incremented : %d \n",buffer.num);
                    fwrite(&buffer.num,sizeof(int),1,ms);
                    fwrite(&buffer.next,sizeof(int),1,ms);

                    // misaj metadata
                    meta->tailleEnRecord++;
                    return;
                }
                k++;
            }
            //if the next block is free we allocate it , if not we make an error ( we might work on a reallocate function)
            bool * b = ReadFAT(ms,D.blocks);

            //this here means that : we are either at the last block or, the adjaçante block is occupied
            if (k>D.blocks || b[k]==true){
                printf("THE DISK IS FULL WE CANNOT ALLOCATE MORE BLOCKS\n");
                free(b);
                return;
            }
            else {
                //if the next block is free and not in the end then we are going to allocate it manually
                printf("AYO I AM HERE ");
                Update_FAT(ms,k,true);
                offset(ms,D,k);
                Display_Block(k,ms,D,&buffer);

                buffer.student[0]=newStudent;
                buffer.student[0].deleted=false;
                buffer.num=1;

                /*offset(ms, D,k);
                fwrite(buffer.student,sizeof(Student),D.bf,ms);
                fwrite(&buffer.num,sizeof(int),1,ms);
                fwrite(&buffer.next,sizeof(int),1,ms);*/
                WriteBlockwPos(ms,D,buffer,k);
                meta->tailleEnBlock++;
                meta->tailleEnRecord++;
                return;

            }
        }
        else if (meta->orgInterne == ORDONE_FILE) {
        Block buffer;
    InitializeBlock(D, &buffer);

    int currentBlock = meta->adress1stBlock; // Premier bloc du fichier
    bool allBlocksFull = true;               // Indicateur de bloc plein
    bool inserted = false;                   // Indicateur d'insertion réussie
    Student tempStudent = newStudent;        // Étudiant à insérer

    // Étape 1 : Vérification si tous les blocs sont pleins (parcours contigu)
    while (currentBlock < meta->tailleEnBlock+ meta->adress1stBlock && !inserted) {
        offset(ms, D, currentBlock);
        Display_Block(currentBlock, ms, D, &buffer);

        if (buffer.num < D.bf) {
            // Si un bloc n'est pas plein, il y a de l'espace pour insérer
            allBlocksFull = false;
            break;  // On peut arrêter la recherche, il suffit de trouver un bloc avec de l'espace
        }

        currentBlock++; // Passer au bloc suivant (organisation contiguë)
    }

    if (!allBlocksFull) {
        // Étape 2 : Insertion dans un bloc existant avec un décalage intra-bloc
        currentBlock = meta->adress1stBlock; // Revenir au premier bloc
        while (currentBlock < meta->tailleEnBlock && !inserted) {
            offset(ms, D, currentBlock);
            Display_Block(currentBlock, ms, D, &buffer);

            if (buffer.num < D.bf) {
                // Le bloc a de l'espace, décaler et insérer
                int j = buffer.num - 1;
                // Décalage des enregistrements pour insérer l'étudiant au bon emplacement
                while (j >= 0 && tempStudent.ID < buffer.student[j].ID) {
                    buffer.student[j + 1] = buffer.student[j]; // Décalage
                    j--;
                }
                buffer.student[j + 1] = tempStudent; // Insertion
                buffer.num++;

                // Mise à jour du bloc dans la mémoire système
                offset(ms, D, currentBlock);
                fwrite(buffer.student, sizeof(Student), D.bf, ms);
                fwrite(&buffer.num, sizeof(int), 1, ms);
                fwrite(&buffer.next, sizeof(int), 1, ms);

                inserted = true; // Étudiant inséré
                meta->tailleEnRecord++; // Mettre à jour les métadonnées
            } else {
                // Passer au bloc suivant (toujours en contigu)
                currentBlock++;
            }
        }
    }

    // Étape 3 : Si tous les blocs sont pleins, réallouer de l'espace
    if (allBlocksFull && currentBlock == meta->tailleEnBlock) {
        // Il n'y a plus de place dans les blocs existants
        printf("Tous les blocs sont pleins. Réallocation de l'espace...\n");

        // Réallouer un bloc supplémentaire pour tout le fichier
        int newBlock = meta->tailleEnBlock;  // Le prochain bloc disponible
        meta->tailleEnBlock++;               // Augmenter la taille du fichier en blocs

        // On doit copier tous les étudiants actuels dans un nouvel espace
        Student *newFileData = malloc(sizeof(Student) * D.bf * (meta->tailleEnBlock)); // Allouer de l'espace pour tous les enregistrements, y compris le nouveau bloc

        // Copier les étudiants des blocs existants dans le nouvel espace contigu
        currentBlock = meta->adress1stBlock; // Revenir au premier bloc
        int i = 0;  // Compteur pour les enregistrements dans le nouvel espace
        while (currentBlock < meta->tailleEnBlock) {
            offset(ms, D, currentBlock);
            Display_Block(currentBlock, ms, D, &buffer);

            for (int j = 0; j < buffer.num; j++) {
                newFileData[i++] = buffer.student[j]; // Copier les étudiants
            }

            currentBlock++;  // Passer au bloc suivant (toujours contigu)
        }

        // Ajouter le nouvel étudiant à la bonne position dans le nouvel espace
        int j = i - 1;
        while (j >= 0 && tempStudent.ID < newFileData[j].ID) {
            newFileData[j + 1] = newFileData[j]; // Décalage
            j--;
        }
        newFileData[j + 1] = tempStudent; // Insertion du nouvel étudiant
        i++;

        // Mise à jour des métadonnées
        meta->tailleEnRecord = i;    // Mettre à jour le nombre d'enregistrements
        meta->adress1stBlock = 0;    // Premier bloc du fichier réécrit

        // Réécriture du fichier dans le nouvel espace
        fseek(ms, 0, SEEK_SET); // Revenir au début du fichier
        fwrite(newFileData, sizeof(Student), i, ms); // Écrire les nouveaux enregistrements
        free(newFileData);  // Libérer la mémoire allouée

        inserted = true;  // Étudiant inséré
    }

    // Libération de la mémoire tampon
    free(buffer.student);
}
    }
}

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
        InitializeBlock(*D,&buffer);

        while (currentBlock != -1) {
            Display_Block(currentBlock,ms,*D,&buffer);

            // Réinitialiser le bloc en écrivant un bloc vide
            offset(ms, *D, currentBlock);
            fwrite(defaultBlock.student, sizeof(Student), D->bf, ms);
            fwrite(&defaultBlock.num, sizeof(int), 1, ms);
            fwrite(&defaultBlock.next, sizeof(int), 1, ms);

            Update_FAT(ms, currentBlock, false); // Mettre à jour la FAT pour libérer le bloc
            currentBlock = buffer.next; // Passage au bloc suivant
        }
        free(buffer.student);
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
    free(defaultBlock.student);
}

