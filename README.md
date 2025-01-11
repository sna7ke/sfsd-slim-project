## OUR NAMES+USERNAMES : (Our full names are in the PROJET SFSD FILE GIVEN)
- BELABDELOUAHAB Farouk Benyoucef : FaroukBel22
- SAHEB Yacine : yacinesaheb
- RIZOU Adem : ademrzz
- AIT MEZIANE El Moataz Bellah Amir : sna7ke
- CHETTAB Abderrahmane : INFRAbderrahmane

## TO DO LIST :

### 1.M�moire secondaire MS :
- [x] **Function "InitializeDisk"** : Nbr total de blocs + Facteur de blockage (all blocks are free in start) ;
- [x] **Function "UpdateFat"** : Mise � jour d'�tat des blocks apr�s chaque op�ration ;
- [x] **Function "InitializeBlock"** : Sets block with initial values.
- [x] **Function "ReadFAT"** : Lire et retourne tableau d'allocation de fichier ;
- [x] **Function "compactage"** : Compactage des fichiers sur le disque sans laisser d'espace entre eux ;
- [x] **Function "EmptyMS"** : Vider compl�tement la m�moire secondaire (effacer toutes les donn�es du disque) ;
**_Notes : Table d'allocation stock�e au d�but de la MS (premier bloc) / Num�rotation des blocks � partir de 0._**

### 2.Fichiers de donn�es :
- [x] **Function "CreateFile"** : Nom / Nombres de records / Mode d'orga globale et interne ;
- [x] **Function "LoadFile"** : Allocations de blocks en MS / Block contient des records d'un seul fichier seulement ;
- [x] **Function "InsertStudent"** : Insertion d'un nouvel enregistrement en fonction du mode d'organisation global/interne ;
- [x] **Function "SearchStudentID"** : *(Pour tout les modes d'orga)* Retourne position de l'enregistrement sous forme (NumBloc,D�placement). Si enreg n'existe pas: afficher message. ;
- [x] **Function "DeleteStudentLogic"** : Suppression logique d'un enregistrement ;
- [x] **Function "Defragmentation"** : R�organiser les blocks d'un fichier pour r�cuperer l'espace inutilis� apr�s des suppressions logiques ;
- [x] **Function "RenameFile"** : Renommer un fichier ;
- [x] **Function "DeleteFile"** : Supprimer un fichier ;

### 3.Fichiers de m�tadonn�es
- [x] **Function "createMeta"** : Create a new metadata for a newfile.
- [x] **Function "readMeta"**   : Reads metadata of a new file.
- [x] **Function "majmeta"**    : Updates metadata of a file.

### 4.Menu Principal :
- [x] **Initialisation de m�moire secondaire** : Reli� � la fonction "InitializeDisk" ;
- [x] **Cr�ation d'un fichier + Chargement en MS** : Reli� aux fonctions "CreateFile" + "LoadFile" ;
- [x] **Affichage graphique de l'�tat de la MS** : Si bloc libre : Vert. Si bloc occup� : Rouge. Pour chaque bloc , afficher le nom du fichier qui l'occupe + nombre d'enregistrements ;
- [x] **Affichage textuel des m�tadonn�es** : Sous forme d'un tableau contenant pour chaque case une information parmis les 6 n�cessaires ;
- [x] **Recherche d'un record par ID** : Reli� � la function "SearchStudentID" ;
- [x] **Ins�rer un nouveau record dans un fichier** : Reli� � la function "InsertNewStudent" ;
- [x] **Suppression logique d'un record** : Reli� � la fonction "DeleteStudentLogic" ;
- [x] **Suppression physique d'un record** : Reli� � la fonction "DeleteStudentPhysic" ;
- [x] **Defragmentation d'un fichier** : Reli� � la function "Defragmentation" ;
- [x] **Supprimer un fichier** : Reli� � la function "DeleteFile" ;
- [x] **Renommer un fichier** : Reli� � la function "RenameFile" ;
- [x] **Compactage de la m�moire secondaire** : Reli� � la function "Compacting" ;
- [x] **Vider la m�moire secondaire** : Reli� � la function "EmptyMS" ;
- [x] **Quitter le programme** : Fermer l'interface du programme ;

### 5.Tests et compte rendu :
- [x] **Test des fonctions une � une sans puis avec interface graphique** ;
- [x] **Test du code en entier dans les cas normaux** ;
- [x] **Test du code dans les cas d'erreurs / disque plein / fragmentation etc...**;
- [x] **R��criture du fichier README.**
- [x] **Ecriture du rapport** : *(Dans un fichier word probablement)* R�sultats des tests avec screenshots / Nom + Pr�nom + Matricule.


### 6.Fichiers .c and executables :
- maininterface.c : RAYGUI interface, most buttons work, the insert button redirect you to the console to fill the infos, defrag button is missing.
- raylib.exe : Executable of the maininterface.c , uses raylib and raygui.h (graphical lib written in c).
- main.c : the main that we tested basically everything with. We let only the base function tests to it.
- main.exe : Executable of main.c that tests the main functions of our program, such as createfile , deletefile , insertFile, fileexists...
- TerminalUI.c : The terminal of our program. This is a replacement to the GUI interface, so plaese use with this if you want to test our full program.
- TerminalUI.exe : Executable of TerminalUI.c.
