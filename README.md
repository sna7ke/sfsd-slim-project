## structure of the project :
- fm.c : contains the functions that handle the file handling logic.
- ms.c: contains the functions that handle the secondary memory handling logic.
- md.c : contains the functions that handle the meta data handling logic.
- util.h : contains the structures and utilities we used in the project.
- main.c : contains only the test of some functionalities, the project is finished but we didn't link all the functions together.
- maininterface.c : contains the source code for the GUI written in c with the raygui library (a graphical library written in C) but it is not finished.
- inlcude/lib folders : contains the raylib/raygui files that are needed to use the library.
- raylib.exe: the executable of maininterface.c, it isn't finished and the functions aren't linked.

## OUR NAMES+USERNAMES : (Our full names are in the PROJET SFSD FILE GIVEN)
- BELABDELOUAHAB Farouk Benyoucef : FaroukBel22
- SAHEB Yacine : yacinesaheb
- RIZOU Adem : ademrzz
- AIT MEZIANE El Moataz Bellah Amir : sna7ke
- CHETTAB Abderrahmane : INFRAbderrahmane

## TO DO LIST :

### 1.Mémoire secondaire MS :
- [x] **Function "InitializeDisk"** : Nbr total de blocs + Facteur de blockage (all blocks are free in start) ;
- [x] **Function "UpdateFat"** : Mise à jour d'état des blocks aprés chaque opération ;
- [x] **Function "InitializeBlock"** : Sets block with initial values.
- [x] **Function "ReadFAT"** : Lire et retourne tableau d'allocation de fichier ;
- [x] **Function "compactage"** : Compactage des fichiers sur le disque sans laisser d'espace entre eux ;
- [x] **Function "EmptyMS"** : Vider complétement la mémoire secondaire (effacer toutes les données du disque) ;
**_Notes : Table d'allocation stockée au début de la MS (premier bloc) / Numérotation des blocks à partir de 0._**

### 2.Fichiers de données :
- [x] **Function "CreateFile"** : Nom / Nombres de records / Mode d'orga globale et interne ;
- [x] **Function "LoadFile"** : Allocations de blocks en MS / Block contient des records d'un seul fichier seulement ;
- [x] **Function "InsertStudent"** : Insertion d'un nouvel enregistrement en fonction du mode d'organisation global/interne ;
- [x] **Function "SearchStudentID"** : *(Pour tout les modes d'orga)* Retourne position de l'enregistrement sous forme (NumBloc,Déplacement). Si enreg n'existe pas: afficher message. ;
- [x] **Function "DeleteStudentLogic"** : Suppression logique d'un enregistrement ;
- [x] **Function "Defragmentation"** : Réorganiser les blocks d'un fichier pour récuperer l'espace inutilisé aprés des suppressions logiques ;
- [x] **Function "RenameFile"** : Renommer un fichier ;
- [x] **Function "DeleteFile"** : Supprimer un fichier ;

### 3.Fichiers de métadonnées
- [x] **Function "createMeta"** : Create a new metadata for a newfile.
- [x] **Function "readMeta"**   : Reads metadata of a new file.
- [x] **Function "majmeta"**    : Updates metadata of a file.

### 4.Menu Principal :
- [x] **Initialisation de mémoire secondaire** : Relié à la fonction "InitializeDisk" ;
- [x] **Création d'un fichier + Chargement en MS** : Relié aux fonctions "CreateFile" + "LoadFile" ;
- [x] **Affichage graphique de l'état de la MS** : Si bloc libre : Vert. Si bloc occupé : Rouge. Pour chaque bloc , afficher le nom du fichier qui l'occupe + nombre d'enregistrements ;
- [x] **Affichage textuel des métadonnées** : Sous forme d'un tableau contenant pour chaque case une information parmis les 6 nécessaires ;
- [x] **Recherche d'un record par ID** : Relié à la function "SearchStudentID" ;
- [x] **Insérer un nouveau record dans un fichier** : Relié à la function "InsertNewStudent" ;
- [x] **Suppression logique d'un record** : Relié à la fonction "DeleteStudentLogic" ;
- [x] **Suppression physique d'un record** : Relié à la fonction "DeleteStudentPhysic" ;
- [x] **Defragmentation d'un fichier** : Relié à la function "Defragmentation" ;
- [x] **Supprimer un fichier** : Relié à la function "DeleteFile" ;
- [x] **Renommer un fichier** : Relié à la function "RenameFile" ;
- [x] **Compactage de la mémoire secondaire** : Relié à la function "Compacting" ;
- [x] **Vider la mémoire secondaire** : Relié à la function "EmptyMS" ;
- [x] **Quitter le programme** : Fermer l'interface du programme ;

### 5.Tests et compte rendu :
- [x] **Test des fonctions une à une sans puis avec interface graphique** ;
- [x] **Test du code en entier dans les cas normaux** ;
- [x] **Test du code dans les cas d'erreurs / disque plein / fragmentation etc...**;
- [x] **Réécriture du fichier README.**
- [x] **Ecriture du rapport** : *(Dans un fichier word probablement)* Résultats des tests avec screenshots / Nom + Prénom + Matricule.
