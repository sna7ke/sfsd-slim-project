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


void creatFile (FILE *ms, Disk *D){
    //NOTE : THIS FUNCTION'S STRUCTURE IS TEMPORARY UNTIL WE WORK ON THE GUI
Meta meta;
 // Demander nom du fichier
    printf("File Name : "); // Temporaire avant l'implémentation de l'interface graphique
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
                        buffer.student[buffer.num] = newStudent;
                        buffer.student[buffer.num].deleted = false;
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
            return;
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
                WriteBlock(ms,D,buffer,k);
                meta->tailleEnBlock++;
                meta->tailleEnRecord++;
                return;

            }
        }
         else if (meta->orgInterne == ORDONE_FILE) {

            for (int i = 0; i< D.blocks; i++) {
                offset(ms, D, i);
                fread(&buffer, sizeof(Block), 1, ms);//trouverle point dinsertion dans les blocs
                for (int j = 0; j < buffer.num; j++) {

                        if (newStudent.ID < buffer.student[j].ID) { // Shift records to make space

                            for (int k = buffer.num; k > j; k--) {

                            buffer.student[k] = buffer.student[k-1];

                            }

                            // Inserer un nv etudiant
                            buffer.student[j] = newStudent;
                            buffer.student[j].deleted = false;
                            buffer.num++;// Overwrite block
                            fseek(ms, -sizeof(Block), SEEK_CUR);
                            fwrite(&buffer, sizeof(Block), 1, ms);// misaj metadata
                            meta->tailleEnRecord++;
                            return;
                        }
                }
            }
        // Isiya pas despace en ajoute un nv
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
         return;
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
                printf("mochkila\n");
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

