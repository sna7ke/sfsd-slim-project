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


bool creatFile (FILE *ms, Disk *D,Meta *m){
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

    int * space = checkFAT(ms, *D, meta.tailleEnBlock,meta.orgGlobal);

    if(space == NULL){
        printf("ERREUR cannot find space to allocate the file !!! \n");
        return false;
    }
    else{


    Allocate_Block(ms, *D, meta.tailleEnBlock,meta.orgGlobal,&meta);


    createMeta(ms, meta); //creer un fichier de metadonnee pour ce fichier
    D->nbrFiles++;
   free(space);
    return true;
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



                if (buffer.num < D.bf){ // si il ya de place

                    buffer.student[buffer.num] = newStudent;
                    buffer.num++;
                    offset(ms, D, i); 

                    fwrite(buffer.student,sizeof(Student),D.bf,ms);
                    fwrite(&buffer.num,sizeof(int),1,ms);
                    fwrite(&buffer.next,sizeof(int),1,ms);

                    meta->tailleEnRecord++;
                   

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

            
            buffer.next= *i2;
            offset(ms,D,i);
            writeblock(ms,buffer,D);

            

            Display_Block(*i2,ms,D,&buffer);



            Update_FAT(ms, *i2, true);

            // fill new student


            buffer.student[0] = newStudent;


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

    // Étape 1 : Vérification si tous les blocs sont pleins (parcours contigue

    while (currentBlock < meta->tailleEnBlock+ meta->adress1stBlock && !inserted) {
        offset(ms, D, currentBlock);
        Display_Block(currentBlock, ms, D, &buffer);

        if (buffer.num < D.bf) {
                
        printf("%d",currentBlock);

           // Si un bloc n'est pas plein, il y a de l'espace pour insérer
            allBlocksFull = false;
            break;  // On peut arrêter la recherche, il suffit de trouver un bloc avec de l'espace
        }

        currentBlock++; // Passer au bloc suivant (organisation contiguë)
    }


    if (!allBlocksFull) {
           
        // �tape 2 : Insertion dans un bloc existant avec un d�calage intra-bloc

        currentBlock = meta->adress1stBlock; // Revenir au premier bloc
        while (currentBlock < meta->tailleEnBlock + meta->adress1stBlock && !inserted) {
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
    if (allBlocksFull && currentBlock == meta->tailleEnBlock + meta->adress1stBlock) {
        // Il n'y a plus de place dans les blocs existants
        printf("Tous les blocs sont pleins. Reallocation de l'espace...\n");

        // Réallouer un bloc supplémentaire pour tout le fichier
        //int newBlock = meta->tailleEnBlock; // Le prochain bloc disponible
        int newblock=currentBlock;

        if(newblock>=D.blocks){
            printf("the main memory is full return \n");
            return;
        }
        else {
        Block TempBlock;
        InitializeBlock(D,&TempBlock);
        //we need to update the block that we have allocated
        Update_FAT(ms,newblock,true);
        Display_Block(newblock,ms,D,&TempBlock);
        TempBlock.num=1;
        
        offset(ms,D,newblock);
        writeblock(ms,TempBlock,D);

        meta->tailleEnBlock++;               // Augmenter la taille du fichier en blocs

        // On doit copier tous les étudiants actuels dans un nouvel espace
        Student *newFileData = malloc(sizeof(Student) * D.bf *(meta->tailleEnBlock-1)+sizeof(Student)); // Allouer de l'espace pour tous les enregistrements, y compris le nouveau bloc

        // Copier les étudiants des blocs existants dans le nouvel espace contigu
        currentBlock = meta->adress1stBlock; // Revenir au premier bloc
        int i = 0;  // Compteur pour les enregistrements dans le nouvel espace

        while (currentBlock < meta->tailleEnBlock +meta->adress1stBlock-1) {
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
        meta->tailleEnRecord++;    // Mettre à jour le nombre d'enregistrements

        // Réécriture du fichier dans le nouvel espace
        currentBlock = meta->adress1stBlock;


        int k =0;
        while(currentBlock<meta->adress1stBlock+meta->tailleEnBlock && k<i){
            Display_Block(currentBlock,ms,D,&TempBlock);

            for (int f=0;f<TempBlock.num;f++){
                strcpy(TempBlock.student[f].name,newFileData[k].name);
                TempBlock.student[f].deleted=newFileData[k].deleted;
                TempBlock.student[f].ID=newFileData[k].ID;
                TempBlock.student[f].group=newFileData[k].group;
                k++;
            }

            offset(ms,D,currentBlock);
            writeblock(ms,TempBlock,D);
            currentBlock++;
        }

        free(newFileData);  // Libérer la mémoire alloué
        free(TempBlock.student);
        inserted = true;  // Étudiant inséré
        }
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

void deleteStudentLogic(FILE *ms, Disk D,char fName[20], int index, Meta fileMeta) {
    int filePos = fileExists(ms , D , fName); // Vérifie si le fichier existe

    if (filePos == -1) {
        printf("Erreur : Fichier non trouvé.\n");
        return;
    }


    // Recherche du student grace a sa position
    posStudent pos = searchStudentID(ms , D , fileMeta,index);


    Block buffer;
    Display_Block(pos.numBlock,ms,D,&buffer);

    if (!buffer.student[pos.deplacement].deleted) {
        buffer.student[pos.deplacement].deleted = true;
        WriteBlockwPos(ms,D,buffer,pos.numBlock);
        printf("L'étudiant à l'index %d a été supprimé logiquement.\n", index);
    } else {
        printf("L'étudiant à l'index %d est déjà supprimé.\n", index);
    }
}

void defragmentFile(FILE *ms, Disk D, Meta *meta) {

    int pos = fileExists(ms, D, meta->nomF);
    if (pos == -1) {
        printf("File '%s' does not exist.\n", meta->nomF);
        return;
    }

    Block buffer1;
    Block buffer2;

   InitializeBlock(D,&buffer1);
   InitializeBlock(D,&buffer2);

    if (meta->orgGlobal == 1 && meta->orgInterne == 2) {
        int currentBlock = meta->adress1stBlock;
        while (currentBlock != -1) {
            Display_Block(currentBlock, ms, D, &buffer1);
            bool needsUpdate = false;

            for (int i = 0; i < buffer1.num; i++) {
                if (buffer1.student[i].deleted == 1) {
                    int lastBlock = meta->adress1stBlock + meta->tailleEnBlock - 1;
                    bool foundValid = false;
                    while (lastBlock >= currentBlock && !foundValid) {
                        Display_Block(lastBlock, ms, D, &buffer2);

                        for (int j = buffer2.num - 1; j >= 0; j--) {
                            if (buffer2.student[j].deleted == 0) {
                                // Copier le dernier enregistrement valide
                                buffer1.student[i] = buffer2.student[j];
                                foundValid = true;
                                needsUpdate = true;

                                // Marquer l'original comme initialisé
                                strcpy(buffer2.student[j].name, "INITIALIZED !");
                                buffer2.student[j].ID = 0;
                                buffer2.student[j].group = 0;
                                buffer2.student[j].deleted = 0;

                                // Réduire le nombre d'enregistrements
                                buffer2.num--;

                                // Mettre à jour le dernier bloc
                                WriteBlockwPos(ms, D, buffer2, lastBlock);

                                // Si c'était le dernier enregistrement du bloc
                                if (buffer2.num == 0) {
                                    meta->tailleEnBlock--;
                                }
                                break;
                            }
                        }

                        if (!foundValid) {
                            lastBlock--;
                        }
                    }

                    if (!foundValid) {
                        // Si aucun enregistrement valide trouvé, initialiser celui-ci
                        strcpy(buffer1.student[i].name, "INITIALIZED !");
                        buffer1.student[i].ID = 0;
                        buffer1.student[i].group = 0;
                        buffer1.student[i].deleted = 0;
                        buffer1.num = i;
                        needsUpdate = true;
                        break;
                    }
                }
            }

            // Vérifier les doublons et les initialiser
            for (int i = 0; i < buffer1.num; i++) {
                for (int j = i + 1; j < buffer1.num; j++) {
                    if (buffer1.student[i].ID == buffer1.student[j].ID &&
                        strcmp(buffer1.student[i].name, buffer1.student[j].name) == 0 &&
                        buffer1.student[i].group == buffer1.student[j].group) {
                        // Initialiser le doublon
                        strcpy(buffer1.student[j].name, "INITIALIZED !");
                        buffer1.student[j].ID = 0;
                        buffer1.student[j].group = 0;
                        buffer1.student[j].deleted = 0;
                        needsUpdate = true;
                    }
                }
            }

            if (needsUpdate) {
                WriteBlockwPos(ms, D, buffer1, currentBlock);
            }
            currentBlock = buffer1.next;
        }

        // Mise à jour du nombre total d'enregistrements
        meta->tailleEnRecord = 0;
        currentBlock = meta->adress1stBlock;
        while (currentBlock != -1) {
            Display_Block(currentBlock, ms, D, &buffer1);
            meta->tailleEnRecord += buffer1.num;
            currentBlock = buffer1.next;
        }
    }
  else   if (meta->orgGlobal == 1 && meta->orgInterne == 1) {
        int writePos = meta->adress1stBlock;
        int currentBlock = meta->adress1stBlock;
        buffer2.num = 0;
        int lastWrittenPos = -1;

        int validRecords = 0;
        while (currentBlock != -1) {
            Display_Block(currentBlock, ms, D, &buffer1);
            for (int i = 0; i < buffer1.num; i++) {
                if (buffer1.student[i].deleted == 0) {
                    validRecords++;
                }
            }
            currentBlock = buffer1.next;
        }

        // Reset for actual defragmentation
        currentBlock = meta->adress1stBlock;
        buffer2.num = 0;
        writePos = meta->adress1stBlock;

        // Calculate needed blocks
        int neededBlocks = (validRecords + D.bf - 1) / D.bf;
        while (currentBlock != -1) {
            Display_Block(currentBlock, ms, D, &buffer1);
            for (int i = 0; i < buffer1.num; i++) {
                if (buffer1.student[i].deleted == 0) {
                    buffer2.student[buffer2.num] = buffer1.student[i];
                    buffer2.num++;
                    if (buffer2.num == D.bf) {
                        buffer2.next = (writePos < meta->adress1stBlock + neededBlocks - 1) ? writePos + 1 : -1;
                        WriteBlockwPos(ms, D, buffer2, writePos);
                        lastWrittenPos = writePos;
                        writePos++;
                        buffer2.num = 0;
                    }
                }
            }
            currentBlock = buffer1.next;
        }

        if (buffer2.num > 0) {
            // Initialize just the last element
            strcpy(buffer2.student[D.bf - 1].name, "INITIALIZED !");
            buffer2.student[D.bf - 1].ID = 0;
            buffer2.student[D.bf - 1].group = 0;
            buffer2.student[D.bf - 1].deleted = 0;

            buffer2.next = -1;
            WriteBlockwPos(ms, D, buffer2, writePos);
            lastWrittenPos = writePos;
            writePos++;
        }

        for (int i = writePos; i < meta->adress1stBlock + meta->tailleEnBlock; i++) {
            InitializeBlock(D, &buffer1);
            buffer1.next = -1;
            WriteBlockwPos(ms, D, buffer1, i);
        }

        meta->tailleEnBlock = writePos - meta->adress1stBlock;
        meta->tailleEnRecord = validRecords;
    } else if ( meta->orgGlobal == CONTIG_FILE ) {
        Block buffer;
        InitializeBlock(D, &buffer);

        // Temporary block to store non-deleted records
        Block tempBlock;
        InitializeBlock(D, &tempBlock);

        int totalRecords = 0;
        int currentBlock = meta->adress1stBlock;

        // First pass: collect all non-deleted records
        for (int i = 0; i < meta->tailleEnBlock; i++) {
            offset(ms, D, currentBlock + i);
            Display_Block(currentBlock + i, ms, D, &buffer);

            for (int j = 0; j < buffer.num; j++) {
                if (!buffer.student[j].deleted) {

                    // Copy non-deleted student to temp block
                    tempBlock.student[tempBlock.num] = buffer.student[j];
                    tempBlock.num++;
                    totalRecords++;

                    // If temp block is full, start a new one
                    if (tempBlock.num == D.bf) {
                        offset(ms, D, currentBlock + (totalRecords-1)/D.bf);
                        writeblock(ms, tempBlock, D);
                        tempBlock.num = 0;
                    }


                }
            }
        }

        // Write the last partial block if it exists
        if (tempBlock.num > 0) {
            offset(ms, D, currentBlock + (totalRecords-1)/D.bf);
            writeblock(ms, tempBlock, D);
        }

        // Calculate new number of blocks needed
        int newBlockCount = (totalRecords + D.bf - 1) / D.bf;

        // If ordered, we need to sort the records
        if (meta->orgInterne == ORDONE_FILE) {
            Student *allStudents = malloc(sizeof(Student) * totalRecords);
            int studentIndex = 0;

            // Read all records into array
            for (int i = 0; i < newBlockCount; i++) {
                offset(ms, D, currentBlock + i);
                Display_Block(currentBlock + i, ms, D, &buffer);

                for (int j = 0; j < buffer.num; j++) {
                    allStudents[studentIndex++] = buffer.student[j];
                }
            }

            // Bubble Sort by ID
            for (int i = 0; i < totalRecords - 1; i++) {
                for (int j = 0; j < totalRecords - i - 1; j++) {
                    if (allStudents[j].ID > allStudents[j + 1].ID) {
                        Student temp = allStudents[j];
                        allStudents[j] = allStudents[j + 1];
                        allStudents[j + 1] = temp;
                    }
                }
            }

            // Write back sorted records
            tempBlock.num = 0;
            for (int i = 0; i < totalRecords; i++) {
                tempBlock.student[tempBlock.num++] = allStudents[i];

                if (tempBlock.num == D.bf || i == totalRecords - 1) {
                    offset(ms, D, currentBlock + i/D.bf);
                    writeblock(ms, tempBlock, D);
                    tempBlock.num = 0;
                }
            }

            free(allStudents);
        }

        // Clear any remaining blocks if we're using fewer blocks now
        Block emptyBlock;
        InitializeBlock(D, &emptyBlock);
        for (int i = newBlockCount; i < meta->tailleEnBlock; i++) {
            offset(ms, D, currentBlock + i);
            writeblock(ms, emptyBlock, D);
            Update_FAT(ms, currentBlock + i, false);  // Mark block as free in FAT
        }

        // Update metadata
        meta->tailleEnRecord = totalRecords;
        meta->tailleEnBlock = newBlockCount;
        majmeta(ms, D, *meta, meta->position);

        // Cleanup
        free(buffer.student);
        free(tempBlock.student);
        free(emptyBlock.student);

        printf("Defragmentation complete. Removed all logically deleted records.\n");
        printf("New record count: %d\n", totalRecords);
        printf("New block count: %d\n", newBlockCount);
    }
    free(buffer1.student);
    free(buffer2.student);
}
