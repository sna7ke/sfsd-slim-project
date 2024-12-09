# sfsd-slim-project
####--> A simple file system simulator that emulates the functions of a real file system by applying the common functionalities on a binary file "secondary_memory.bin"

##TO DO LIST :
###1.M�moire secondaire MS :
    - [ ] **Mode d'organisation chain�e ou contigue (choix de l'utilisateur lors de cr�ation du fichier)** ;
    - [x] **Function "InitializeDisk"** : Nbr total de blocs + Facteur de blockage (all blocks are free in start) ;
    - [x] **Function "Metajour"** : Mise � jour d'�tat des blocks apr�s chaque op�ration ;
    - [x] **Function "InitializeBlock"** :
    - [x] **Function "ReadFAT"** : Lire et retourne tableau d'allocation de fichier ;
    - [ ] **Function "Compacting"** : Compactage des fichiers sur le disque sans laisser d'espace entre eux ;
    - [ ] **Function "EmptyMS"** : Vider compl�tement la m�moire secondaire (effacer toutes les donn�es du disque) ;
    - [ ] **Function "GestionStockage"** *(A UTILISER AVANT CHAQUE OPERATION D'ENTREE/SORTIE)* : Verif si assez de blocks libres dispo. Si non : compactage. Si MS full : afficher message ;
    **_Notes : Table d'allocation stock�e au d�but de la MS (premier bloc) / Num�rotation des blocks � partir de 0._**

###2.Fichiers de donn�es :
    - [ ] **Definir organisation Globale** : Contigue ou Chain�e ;
    - [ ] **D�finir organisation Interne** : Tri�e ou Non-Tri�e ;
    - [ ] **Function "FillFile"** : Fonction qui remplit automatiquement les donn�es des fichiers ;
    - [ ] **Function "CreateFile"** : Nom / Nombres de records / Mode d'orga globale et interne ;
    - [ ] **Function "LoadFile"** : Allocations de blocks en MS / Block contient des records d'un seul fichier seulement ;
    - [ ] **Function "InsertNewStudent"** : Insertion d'un nouvel enregistrement en fonction du mode d'organisation global/interne ;
    - [ ] **Function "SearchStudentID"** : *(Pour tout les modes d'orga)* Retourne position de l'enregistrement sous forme (NumBloc,D�placement). Si enreg n'existe pas: afficher message. ;
    - [ ] **Function "DeleteStudentLogic"** : Suppression logique d'un enregistrement ;
    - [ ] **Function "DeleteStudentPhysic"** : Suppression physique d'un enregistrement ;
    - [ ] **Function "Defragmentation"** : R�organiser les blocks d'un fichier pour r�cuperer l'espace inutilis� apr�s des suppressions logiques ;
    - [ ] **Function "RenameFile"** : Renommer un fichier ;
    - [ ] **Function "DeleteFile"** : Supprimer un fichier ;

###3.Fichiers de m�tadonn�es :



###4.Menu Principal :
--> Interactions avec :
    - [ ] **Initialisation de m�moire secondaire** : Reli� � la fonction "InitializeDisk" ;
    - [ ] **Cr�ation d'un fichier + Chargement en MS** : Reli� aux fonctions "CreateFile" + "LoadFile" ;
    - [ ] **Affichage graphique de l'�tat de la MS** : Si bloc libre : Vert. Si bloc occup� : Rouge. Pour chaque bloc , afficher le nom du fichier qui l'occupe + nombre d'enregistrements ;
    - [ ] **Affichage textuel des m�tadonn�es** : Sous forme d'un tableau contenant pour chaque case une information parmis les 6 n�cessaires ;
    - [ ] **Recherche d'un record par ID** : Reli� � la function "SearchStudentID" ;
    - [ ] **Ins�rer un nouveau record dans un fichier** : Reli� � la function "InsertNewStudent" ;
    - [ ] **Suppression logique d'un record** : Reli� � la fonction "DeleteStudentLogic" ;
    - [ ] **Suppression physique d'un record** : Reli� � la fonction "DeleteStudentPhysic" ;
    - [ ] **Defragmentation d'un fichier** : Reli� � la function "Defragmentation" ;
    - [ ] **Supprimer un fichier** : Reli� � la function "DeleteFile" ;
    - [ ] **Renommer un fichier** : Reli� � la function "RenameFile" ;
    - [ ] **Compactage de la m�moire secondaire** : Reli� � la function "Compacting" ;
    - [ ] **Vider la m�moire secondaire** : Reli� � la function "EmptyMS" ;
    - [ ] **Quitter le programme** : Fermer l'interface du programme ;

###5.Tests et compte rendu :
    - [ ] **Test des fonctions une � une sans puis avec interface graphique** ;
    - [ ] **Test du code en entier dans les cas normaux** ;
    - [ ] **Test du code dans les cas d'erreurs / disque plein / fragmentation etc...**;
    - [ ] **R��criture du fichier README** : Explication d�taill�e sur l'utilisation du simulateur au lieu d'une TODOLIST ;
    - [ ] **Ecriture du rapport** : *(Dans un fichier word probablement)* R�sultats des tests avec screenshots pour tout les cas possibles / Nom+Pr�nom+Matricule+Role_dans_projet de chaque �tudiant.
