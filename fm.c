#include "fm.h"
#include "md.h"

//TODO : there is a small memory issue which we could optimize , instead of doing checkfat inside allocate block we could do it once outside and pass taht array as argument for allocate block

Block fillBuffer(Disk d) {
    Block buffer;

   InitializeBlock(d,&buffer);
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


void creatFile (FILE *ms, Disk *D){
    //NOTE : THIS FUNCTION'S STRUCTURE IS TEMPORARY UNTIL WE WORK ON THE GUI
Meta meta;
 // Demander nom du fichier
    printf("File Name : "); // Temporaire avant l'impl�mentation de l'interface graphique
    scanf("%s", meta.nomF);
    // Demander nombre de records
    //printf("Number of records : ");
    //scanf("%d", &meta.tailleEnRecord);
    printf("Number of Blocks you wish to initialize your file with : ");
    scanf("%d", &meta.tailleEnBlock);

    meta.tailleEnRecord=0;

    // Calcul du nombre de blocs
    /*if ( meta.tailleEnRecord % D->bf == 0 ){
        meta.tailleEnBlock =  meta.tailleEnRecord/D->bf ;
    }else{
        meta.tailleEnBlock = ( (meta.tailleEnRecord/D->bf)+1 );
    }*/

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
     // meta.adress1stBlock = *space;
if(space == NULL){
    printf("ERREUR f creat !!! \n");
}
else{


    Allocate_Block(ms, *D, meta.tailleEnBlock,meta.orgGlobal,&meta);

    createMeta(ms, meta); //creer un fichier de metadonnee pour ce fichier
    D->nbrFiles++;
    free(space);
}
}


int fileExists(FILE *ms, Disk D, char fName[20]) {
    Meta met;
    // Parcourir toutes les m�tadonn�es pour v�rifier si le nom existe d�j�
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
    // Si aucun fichier n'a le m�me nom
    printf("The file does not exist.\n");
    return -1;
}

int binarySearch(Student *students, int numRecords, int ID) { // Fonction de recherche dichotomique (division du tableau par 2 � chaque recherche pour but d'optimisation)
    int start = 0, end = numRecords - 1;
    // V�rification de si l'ID est en dehors de l'intervalle
    if (ID < students[start].ID || ID > students[end].ID) {
        return -1; // L'ID est en dehors de l'intervalle
    while (start <= end) {
        int mid = start + (end - start) / 2;
        if (students[mid].ID == ID) {
            return mid; // �tudiant trouv�
        }
        if (students[mid].ID < ID) {
            start = mid + 1;
        } else {
            end = mid - 1;
        }
    }
    return -1; // �tudiant non trouv�
    }
}

posStudent searchStudentID(FILE *ms, Disk D, Meta meta, int ID) {
    Block buffer;
    InitializeBlock(D, &buffer); // Initialisation du buffer
    posStudent pos;
    pos.numBlock = -1;      // Valeurs par d�faut si l'�tudiant n'est pas trouv�
    pos.deplacement = -1;
    if (meta.orgInterne == ORDONE_FILE) {
        if (meta.orgGlobal == CONTIG_FILE) {
            for (int blockNum = 0; blockNum < meta.tailleEnBlock; blockNum++) {
                offset(ms, D, meta.adress1stBlock + blockNum); // Aller au bloc n�cessaire
                fread(&buffer.num, sizeof(int), 1, ms); // Lire le nombre de students
                fread(buffer.student, sizeof(Student), D.bf, ms); // Lire students
                int foundPos = binarySearch(buffer.student, buffer.num, ID);
                if (foundPos != -1) { // �tudiant trouv�
                    pos.numBlock = blockNum;      // Num�ro du Bloc o� l'�tudiant est trouv�
                    pos.deplacement = foundPos;   // Position dans le bloc
                    free(buffer.student);
                    return pos;
                }
            }
        } else if (meta.orgGlobal == CHAINED_FILE) {
            int currentBlock = meta.adress1stBlock;
            while (currentBlock != -1) {
                offset(ms, D, currentBlock); // Aller au bloc n�cessaire
                fread(&buffer.num, sizeof(int), 1, ms); // Lire le nombre de students
                fread(buffer.student, sizeof(Student), D.bf, ms); // Lire les �tudiants
                fread(&buffer.next, sizeof(int), 1, ms); // Lire le pointeur vers le prochain bloc
                int foundPos = binarySearch(buffer.student, buffer.num, ID);
                if (foundPos != -1) { // �tudiant trouv�
                    pos.numBlock = currentBlock;   // Num�ro du bloc o� l'�tudiant est trouv�
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
    printf("�tudiant non trouv�.\n");
    free(buffer.student);
    return pos; // Retour de la valeur par d�faut (-1, -1)
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
                //i = buffer.next;
                if(buffer.next==-1){
                    printf("going to allocate a new block now \n");
                    break;
                }

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
            fwrite(buffer.student,sizeof(Student),D.bf,ms);
            fwrite(&buffer.num,sizeof(int),1,ms);
            fwrite(&buffer.next,sizeof(int),1,ms);

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
            return;

        }

      else if (meta->orgInterne == ORDONE_FILE) {
       Block buffer;
            InitializeBlock(D, &buffer);
            bool found = false;
            posStudent pos = {0, 0};// First, find the correct block and position for insertion
            int currentBlock = meta->adress1stBlock;
            int blockCount = 0;
            while (currentBlock != -1 && !found) {
                offset(ms, D, currentBlock);
                Display_Block(currentBlock, ms, D, &buffer);// Check each student in current block
                for (int j = 0; j < buffer.num; j++) {
                    if (newStudent.ID < buffer.student[j].ID) {
                        pos.deplacement = j;
                        pos.numBlock = blockCount;
                        found = true;
                        break;
                    }
                }// If we haven't found a position yet
                if (!found) {
                    // If this block isn't full
                    if (buffer.num < D.bf) {
                        pos.deplacement = buffer.num;
                        pos.numBlock = blockCount;
                        found = true;
                    }// If this is the last block and it's full
                    else if (buffer.next == -1) {
                        pos.deplacement = D.bf;  // Position after last element
                        pos.numBlock = blockCount;
                        found = true;
                    }// Move to next block
                    else {
                        currentBlock = buffer.next;
                        blockCount++;
                    }
                }
            }// Now insert the student and shift others as needed
            currentBlock = meta->adress1stBlock;
            Student tempStudent = newStudent;
            int currentPos = pos.deplacement;
            blockCount = 0;
            while (currentBlock != -1) {
                offset(ms, D, currentBlock);
                Display_Block(currentBlock, ms, D, &buffer);
                if (blockCount == pos.numBlock) {// This is the block where we need to insert
                    if (buffer.num < D.bf) {// There's space in this block
                        for (int i = buffer.num; i > currentPos; i--) {
                            buffer.student[i] = buffer.student[i-1];
                        }
                        buffer.student[currentPos] = tempStudent;
                        buffer.num++;

                        offset(ms, D, currentBlock);
                        writeblock (ms,buffer , D);
                        meta->tailleEnRecord++;
                        break;
                    } else {// Need to create new block if this is the last block
                        if (buffer.next == -1) {
                            int* newBlockPos = checkFAT(ms, D, 1, CHAINED_FILE);
                            if (newBlockPos == NULL) {
                                printf("Error: No space available for new block\n");
                                free(buffer.student);
                                return;
                            }// If inserting in middle of current block
                            if (currentPos < D.bf) {
                                Student overflowStudent = buffer.student[D.bf - 1];// Shift elements to make space
                                for (int i = D.bf - 1; i > currentPos; i--) {
                                    buffer.student[i] = buffer.student[i-1];
                                }
                                buffer.student[currentPos] = tempStudent;// Write back the modified block
                                offset(ms, D, currentBlock);
                                fwrite(buffer.student, sizeof(Student), D.bf, ms);
                                fwrite(&buffer.num, sizeof(int), 1, ms);

                                tempStudent = overflowStudent;  // The overflow student goes to new block
                            }// Update current block's next pointer
                            buffer.next = *newBlockPos;
                            offset(ms, D, currentBlock);
                            fseek(ms, sizeof(Student) * D.bf + sizeof(int), SEEK_CUR);
                            fwrite(&buffer.next, sizeof(int), 1, ms);// Create new block with overflow/new student
                            Block newBlock;
                            InitializeBlock(D, &newBlock);
                            newBlock.student[0] = tempStudent;
                            newBlock.num = 1;
                            newBlock.next = -1;
                            offset(ms, D, *newBlockPos);
                            writeblock (ms, buffer , D);
                            Update_FAT(ms, *newBlockPos, true);
                            meta->tailleEnRecord++;
                            meta->tailleEnBlock++;
                            free(newBlock.student);
                            free(newBlockPos);
                            break;
                        }// Not the last block, continue shifting
                        else {
                            Student overflowStudent = buffer.student[D.bf - 1];
                            for (int i = D.bf - 1; i > currentPos; i--) {
                                buffer.student[i] = buffer.student[i-1];
                            }
                            buffer.student[currentPos] = tempStudent;
                            offset(ms, D, currentBlock);
                            writeblock (ms, buffer ,D);
                            tempStudent = overflowStudent;
                            currentPos = 0;
                            currentBlock = buffer.next;
                            blockCount++;
                        }
                    }
                } else {
                    currentBlock = buffer.next;
                    blockCount++;
                }
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

            //this here means that : we are either at the last block or, the adja�ante block is occupied
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
    bool inserted = false;                   // Indicateur d'insertion r�ussie
    Student tempStudent = newStudent;        // �tudiant � ins�rer

    // �tape 1 : V�rification si tous les blocs sont pleins (parcours contigu)
    while (currentBlock < meta->tailleEnBlock && !inserted) {
        offset(ms, D, currentBlock);
        Display_Block(currentBlock, ms, D, &buffer);

        if (buffer.num < D.bf) {
            // Si un bloc n'est pas plein, il y a de l'espace pour ins�rer
            allBlocksFull = false;
            break;  // On peut arr�ter la recherche, il suffit de trouver un bloc avec de l'espace
        }

        currentBlock++; // Passer au bloc suivant (organisation contigu�)
    }

    if (!allBlocksFull) {
        // �tape 2 : Insertion dans un bloc existant avec un d�calage intra-bloc
        currentBlock = meta->adress1stBlock; // Revenir au premier bloc
        while (currentBlock < meta->tailleEnBlock && !inserted) {
            offset(ms, D, currentBlock);
            Display_Block(currentBlock, ms, D, &buffer);

            if (buffer.num < D.bf) {
                // Le bloc a de l'espace, d�caler et ins�rer
                int j = buffer.num - 1;
                // D�calage des enregistrements pour ins�rer l'�tudiant au bon emplacement
                while (j >= 0 && tempStudent.ID < buffer.student[j].ID) {
                    buffer.student[j + 1] = buffer.student[j]; // D�calage
                    j--;
                }
                buffer.student[j + 1] = tempStudent; // Insertion
                buffer.num++;

                // Mise � jour du bloc dans la m�moire syst�me
                offset(ms, D, currentBlock);
                fwrite(buffer.student, sizeof(Student), D.bf, ms);
                fwrite(&buffer.num, sizeof(int), 1, ms);
                fwrite(&buffer.next, sizeof(int), 1, ms);

                inserted = true; // �tudiant ins�r�
                meta->tailleEnRecord++; // Mettre � jour les m�tadonn�es
            } else {
                // Passer au bloc suivant (toujours en contigu)
                currentBlock++;
            }
        }
    }

    // �tape 3 : Si tous les blocs sont pleins, r�allouer de l'espace
    if (allBlocksFull && currentBlock == meta->tailleEnBlock) {
        // Il n'y a plus de place dans les blocs existants
        printf("Tous les blocs sont pleins. R�allocation de l'espace...\n");

        // R�allouer un bloc suppl�mentaire pour tout le fichier
        int newBlock = meta->tailleEnBlock;  // Le prochain bloc disponible
        meta->tailleEnBlock++;               // Augmenter la taille du fichier en blocs

        // On doit copier tous les �tudiants actuels dans un nouvel espace
        Student *newFileData = malloc(sizeof(Student) * D.bf * (meta->tailleEnBlock)); // Allouer de l'espace pour tous les enregistrements, y compris le nouveau bloc

        // Copier les �tudiants des blocs existants dans le nouvel espace contigu
        currentBlock = meta->adress1stBlock; // Revenir au premier bloc
        int i = 0;  // Compteur pour les enregistrements dans le nouvel espace
        while (currentBlock < meta->tailleEnBlock) {
            offset(ms, D, currentBlock);
            Display_Block(currentBlock, ms, D, &buffer);

            for (int j = 0; j < buffer.num; j++) {
                newFileData[i++] = buffer.student[j]; // Copier les �tudiants
            }

            currentBlock++;  // Passer au bloc suivant (toujours contigu)
        }

        // Ajouter le nouvel �tudiant � la bonne position dans le nouvel espace
        int j = i - 1;
        while (j >= 0 && tempStudent.ID < newFileData[j].ID) {
            newFileData[j + 1] = newFileData[j]; // D�calage
            j--;
        }
        newFileData[j + 1] = tempStudent; // Insertion du nouvel �tudiant
        i++;

        // Mise � jour des m�tadonn�es
        meta->tailleEnRecord = i;    // Mettre � jour le nombre d'enregistrements
        meta->adress1stBlock = 0;    // Premier bloc du fichier r��crit

        // R��criture du fichier dans le nouvel espace
        fseek(ms, 0, SEEK_SET); // Revenir au d�but du fichier
        fwrite(newFileData, sizeof(Student), i, ms); // �crire les nouveaux enregistrements
        free(newFileData);  // Lib�rer la m�moire allou�e

        inserted = true;  // �tudiant ins�r�
    }

    // Lib�ration de la m�moire tampon
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

    Meta meta = readMeta(ms, *D, pos); // Lecture des m�tadonn�es

    // Suppression des blocs allou�s au fichier
    Block defaultBlock;
    InitializeBlock(*D, &defaultBlock); // Bloc initialis� aux valeurs par d�fait

    if (meta.orgGlobal == CONTIG_FILE) {
        printf("Re-initializing blocks (Contiguous organization)...\n");
        for (int i = 0; i < meta.tailleEnBlock; i++) {
            offset(ms, *D, meta.adress1stBlock + i); // D�placer le curseur au bloc sp�cifique
            // R�initialiser le bloc en �crivant un bloc vide
            fwrite(defaultBlock.student, sizeof(Student), D->bf, ms);
            fwrite(&defaultBlock.num, sizeof(int), 1, ms);
            fwrite(&defaultBlock.next, sizeof(int), 1, ms);

            // Mettre � jour la FAT pour lib�rer le bloc
            Update_FAT(ms, meta.adress1stBlock + i, false);
        }
    } else if (meta.orgGlobal == CHAINED_FILE) {
        printf("Re-Initializing (Chained organization)...\n");
        int currentBlock = meta.adress1stBlock;
        Block buffer;
        InitializeBlock(*D,&buffer);

        while (currentBlock != -1) {
            Display_Block(currentBlock,ms,*D,&buffer);

            // R�initialiser le bloc en �crivant un bloc vide
            offset(ms, *D, currentBlock);
            fwrite(defaultBlock.student, sizeof(Student), D->bf, ms);
            fwrite(&defaultBlock.num, sizeof(int), 1, ms);
            fwrite(&defaultBlock.next, sizeof(int), 1, ms);

            Update_FAT(ms, currentBlock, false); // Mettre � jour la FAT pour lib�rer le bloc
            currentBlock = buffer.next; // Passage au bloc suivant
        }
        free(buffer.student);
    }
    // Suppression des m�tadonn�es
    if (D->nbrFiles > 1 && pos != D->nbrFiles) {
        Meta lastMeta = readMeta(ms, *D, D->nbrFiles); // Lire la derni�re m�tadonn�e
        lastMeta.position = pos; // Mettre � jour sa position
        fseek(ms, -(D->nbrFiles - pos + 1) * sizeof(Meta), SEEK_END); // Aller � la position de la m�tadonn�e supprim�e
        fwrite(&lastMeta, sizeof(Meta), 1, ms); // �crire la derni�re m�tadonn�e � la place
    }

    D->nbrFiles--; // Diminution du nombre de fichiers par 1
    printf("File named '%s' deleted succesfully.\n", fName);
    free(defaultBlock.student);
}

