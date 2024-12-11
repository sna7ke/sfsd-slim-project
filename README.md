# sfsd-slim-project
#### --> A simple file system simulator that emulates the functions of a real file system by applying the common functionalities on a binary file "secondary_memory.bin"
### NOTE :
main.c is a temporary file to test what we have done so far , it will be removed/edited constantly but its just a test


## TO DO LIST :

### 1.Mémoire secondaire MS :
- [x] **Mode d'organisation chainée ou contigue (choix de l'utilisateur lors de création du fichier)** ;
- [x] **Function "InitializeDisk"** : Nbr total de blocs + Facteur de blockage (all blocks are free in start) ;
- [x] **Function "Metajour"** : Mise à jour d'état des blocks aprés chaque opération ;
- [x] **Function "InitializeBlock"** :
- [x] **Function "ReadFAT"** : Lire et retourne tableau d'allocation de fichier ;
- [] **Function "Compacting"** : Compactage des fichiers sur le disque sans laisser d'espace entre eux ;
- [x] **Function "EmptyMS"** : Vider complétement la mémoire secondaire (effacer toutes les données du disque) ;
- [] **Function "GestionStockage"** *(A UTILISER AVANT CHAQUE OPERATION D'ENTREE/SORTIE)* : Verif si assez de blocks libres dispo. Si non : compactage. Si MS full : afficher message ;
**_Notes : Table d'allocation stockée au début de la MS (premier bloc) / Numérotation des blocks à partir de 0._**

### 2.Fichiers de données :
- [x] **Definir organisation Globale** : Contigue ou Chainée ;
- [x] **Définir organisation Interne** : Triée ou Non-Triée ;
- [] **Function "FillFile"** : Fonction qui remplit automatiquement les données des fichiers ;
- [] **Function "CreateFile"** : Nom / Nombres de records / Mode d'orga globale et interne ;
- [x] **Function "LoadFile"** : Allocations de blocks en MS / Block contient des records d'un seul fichier seulement ;
- [] **Function "InsertNewStudent"** : Insertion d'un nouvel enregistrement en fonction du mode d'organisation global/interne ;
- [] **Function "SearchStudentID"** : *(Pour tout les modes d'orga)* Retourne position de l'enregistrement sous forme (NumBloc,Déplacement). Si enreg n'existe pas: afficher message. ;
- [] **Function "DeleteStudentLogic"** : Suppression logique d'un enregistrement ;
- [] **Function "DeleteStudentPhysic"** : Suppression physique d'un enregistrement ;
- [] **Function "Defragmentation"** : Réorganiser les blocks d'un fichier pour récuperer l'espace inutilisé aprés des suppressions logiques ;
- [] **Function "RenameFile"** : Renommer un fichier ;
- [] **Function "DeleteFile"** : Supprimer un fichier ;

### 3.Fichiers de métadonnées :



### 4.Menu Principal :
- [] **Initialisation de mémoire secondaire** : Relié à la fonction "InitializeDisk" ;
- [] **Création d'un fichier + Chargement en MS** : Relié aux fonctions "CreateFile" + "LoadFile" ;
- [] **Affichage graphique de l'état de la MS** : Si bloc libre : Vert. Si bloc occupé : Rouge. Pour chaque bloc , afficher le nom du fichier qui l'occupe + nombre d'enregistrements ;
- [] **Affichage textuel des métadonnées** : Sous forme d'un tableau contenant pour chaque case une information parmis les 6 nécessaires ;
- [] **Recherche d'un record par ID** : Relié à la function "SearchStudentID" ;
- [] **Insérer un nouveau record dans un fichier** : Relié à la function "InsertNewStudent" ;
- [] **Suppression logique d'un record** : Relié à la fonction "DeleteStudentLogic" ;
- [] **Suppression physique d'un record** : Relié à la fonction "DeleteStudentPhysic" ;
- [] **Defragmentation d'un fichier** : Relié à la function "Defragmentation" ;
- [] **Supprimer un fichier** : Relié à la function "DeleteFile" ;
- [] **Renommer un fichier** : Relié à la function "RenameFile" ;
- [] **Compactage de la mémoire secondaire** : Relié à la function "Compacting" ;
- [] **Vider la mémoire secondaire** : Relié à la function "EmptyMS" ;
- [] **Quitter le programme** : Fermer l'interface du programme ;

### 5.Tests et compte rendu :
- [] **Test des fonctions une à une sans puis avec interface graphique** ;
- [] **Test du code en entier dans les cas normaux** ;
- [] **Test du code dans les cas d'erreurs / disque plein / fragmentation etc...**;
- [] **Réécriture du fichier README** : Explication détaillée sur l'utilisation du simulateur au lieu d'une TODOLIST ;
- [] **Ecriture du rapport** : *(Dans un fichier word probablement)* Résultats des tests avec screenshots pour tout les cas possibles / Nom+Prénom+Matricule+Role_dans_projet de chaque étudiant.
