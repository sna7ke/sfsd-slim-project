#include "Md.h"
//TODO : the current functions only work with one metadata in mind , once we add more files it stops working , one of you should update it to be flexible
void createMeta(FILE *ms, Meta mymeta){
    fseek(ms, 0, SEEK_END);
    fwrite(&mymeta, sizeof(Meta), 1, ms);
}


Meta* readMeta(FILE *ms, Disk D) { // La fonction retourne un tableau ou sont stock�es les m�tadonn�es de chaque fichier
    Meta* fMeta = (Meta*)malloc(D.nbrFiles * sizeof(Meta));

    // D�placer le curseur � la fin du fichier
    fseek(ms, 0, SEEK_END);

    // Lire les m�tadonn�es depuis la fin du fichier
    for (int i = D.nbrFiles - 1; i >= 0; i--) {
        long pos = ftell(ms) - sizeof(Meta); // Calculer la position des m�tadonn�es
        fseek(ms, pos, SEEK_SET);  // Se d�placer � pos
        fread(&fMeta[i], sizeof(Meta), 1, ms); // Lire les m�tadonn�es
    }

    return fMeta;  // Retourner le tableau de m�tadonn�es
}



void majmeta(FILE *ms, Disk D, const char* fName, int newBlocks, int newRecords) { // Si nous ne voulons pas changer le nom du fichier , intialisez newName a NULL
    if (fName == NULL || strlen(fName) == 0) { // Verification de la validit� du nom du fichier
        printf("The name of the file is empty.\n");
        return;
    }

    bool found = false;
    fseek(ms, 0, SEEK_END); // D�placer le curseur � la fin du fichier
    for (int i = D.nbrFiles - 1; i >= 0; i--) { // Parcourir les m�tadonn�es du fichier
        long pos = ftell(ms) - sizeof(Meta); // Calculer la position de la strucutre Meta
        fseek(ms, pos, SEEK_SET);  // Se d�placer � la position des m�tadonn�es du fichier actuel
        Meta fMeta;
        fread(&fMeta, sizeof(Meta), 1, ms); // Lire les m�tadonn�es

        // V�rifier si le nom du fichier correspond
        if (strcmp(fMeta.nomF, fName) == 0) {
            // Mettre � jour les m�tadonn�es
            fMeta.tailleEnBlock = newBlocks;
            fMeta.tailleEnRecord = newRecords;
            }
            // Revenir � la position pour r��crire les m�tadonn�es mises � jour
            fseek(ms, pos, SEEK_SET);
            fwrite(&fMeta, sizeof(Meta), 1, ms); // R��crire les m�tadonn�es
            found = true;
            break; // Arr�ter la recherche apr�s avoir trouv� le fichier
        }


    if (!found) {
        printf("File with name '%s' not found.\n", fName);
    }
}
