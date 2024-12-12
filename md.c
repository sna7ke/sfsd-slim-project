#include "Md.h"
//TODO : the current functions only work with one metadata in mind , once we add more files it stops working , one of you should update it to be flexible
void createMeta(FILE *ms, Meta mymeta){
    fseek(ms, 0, SEEK_END);
    fwrite(&mymeta, sizeof(Meta), 1, ms);
}


Meta* readMeta(FILE *ms, Disk D) { // La fonction retourne un tableau ou sont stockées les métadonnées de chaque fichier
    Meta* fMeta = (Meta*)malloc(D.nbrFiles * sizeof(Meta));

    // Déplacer le curseur à la fin du fichier
    fseek(ms, 0, SEEK_END);

    // Lire les métadonnées depuis la fin du fichier
    for (int i = D.nbrFiles - 1; i >= 0; i--) {
        long pos = ftell(ms) - sizeof(Meta); // Calculer la position des métadonnées
        fseek(ms, pos, SEEK_SET);  // Se déplacer à pos
        fread(&fMeta[i], sizeof(Meta), 1, ms); // Lire les métadonnées
    }

    return fMeta;  // Retourner le tableau de métadonnées
}



void majmeta(FILE *ms, Disk D, const char* fName, int newBlocks, int newRecords) { // Si nous ne voulons pas changer le nom du fichier , intialisez newName a NULL
    if (fName == NULL || strlen(fName) == 0) { // Verification de la validité du nom du fichier
        printf("The name of the file is empty.\n");
        return;
    }

    bool found = false;
    fseek(ms, 0, SEEK_END); // Déplacer le curseur à la fin du fichier
    for (int i = D.nbrFiles - 1; i >= 0; i--) { // Parcourir les métadonnées du fichier
        long pos = ftell(ms) - sizeof(Meta); // Calculer la position de la strucutre Meta
        fseek(ms, pos, SEEK_SET);  // Se déplacer à la position des métadonnées du fichier actuel
        Meta fMeta;
        fread(&fMeta, sizeof(Meta), 1, ms); // Lire les métadonnées

        // Vérifier si le nom du fichier correspond
        if (strcmp(fMeta.nomF, fName) == 0) {
            // Mettre à jour les métadonnées
            fMeta.tailleEnBlock = newBlocks;
            fMeta.tailleEnRecord = newRecords;
            }
            // Revenir à la position pour réécrire les métadonnées mises à jour
            fseek(ms, pos, SEEK_SET);
            fwrite(&fMeta, sizeof(Meta), 1, ms); // Réécrire les métadonnées
            found = true;
            break; // Arrêter la recherche après avoir trouvé le fichier
        }


    if (!found) {
        printf("File with name '%s' not found.\n", fName);
    }
}
