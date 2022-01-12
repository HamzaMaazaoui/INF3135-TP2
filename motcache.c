/**
Il s'agit d'un programme de résolution de mots cachés en C
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAILLE_GRILLE 12
#define TAILLE_MOTS 20
#define MAX_MOTS 500

/**
  Ouvrir un fichier.
  @param nomFichier: le nom de fichier
  @return le fichier
 */
FILE* ouvrirFichier(char* nomFichier);

/**
  Obtenir le contenu du fichier (la grille des lettres et la liste des mots).
  @param grille: la grille
  @param listeMots: la liste de mots
  @param tailleListeMots: la taille de la liste des mots
  @param fichier: le fichier
 */
void obtenirContenu(char grille[][TAILLE_GRILLE], char listeMots[MAX_MOTS][TAILLE_MOTS], int* tailleListeMots, FILE* fichier);

/**
  Chercher un mot dans la grille
  @param mot: le mot à chercher
  @param grille: la grille des lettres
  @param matrice: matrice des lettres de la grille
 */
void chercher(const char* mot, char grille[][TAILLE_GRILLE], int matrice[TAILLE_GRILLE][TAILLE_GRILLE]);

/**
  Extraire un mot à partir d'une position dans la grille
  @param mot le mot
  @param tailleMot la taille du mot
  @param grille la grille des mots
  @param i la ligne
  @param j la colonne
  @return retourne la direction dans laquelle le mot à été trouvé ou 0 sinon.
 */
int extraireMot(char* mot, const char* motListe,int tailleMot, char grille[][TAILLE_GRILLE], int ligne, int colonne);

/**
  trouver le mot dans la grille.
  @param motCache: le mot caché
  @param grille: la grille des lettres
  @param listeMots: la liste des mots
  @param tailleMot: la taille du mot
  @param matrice: matrice des lettres de la grille
 */
void trouverMotCache(char** motCache, char grille[][TAILLE_GRILLE], char listeMots[MAX_MOTS][TAILLE_MOTS], int tailleMot, int matrice[TAILLE_GRILLE][TAILLE_GRILLE]);

/**
  trouver un mot a partir d'une position et une direction.
  @param mot le mot extrait
  @param tailleMot la taille du mot à trouver.
  @param grille la grille des lettres
  @param ligne la ligne 
  @param colonne la colonne
  @param direction la direction dans laquelle le mot est extrait (haut,bas,gauche,droite)
 */
void trouverParDirection(char** mot, int tailleMot, char grille[][TAILLE_GRILLE], int ligne, int colonne, int direction);

/**
  Mettre à jour la matrice.
  @param matrice  la matrice
  @param ligne la ligne
  @param colonne la colonne 
  @param tailleMot la taille du mot
  @param direction la direction du mot
 */
void mettreAjour(int matrice[TAILLE_GRILLE][TAILLE_GRILLE], int ligne, int colonne, int tailleMot, int direction);

FILE* ouvrirFichier(char* nomFichier) {
   FILE* fichier = fopen(nomFichier, "r");
   if(fichier == NULL) {
      printf("Le fichier n\'existe pas\n");
      exit(-1);
   }
   return fichier;
}

void obtenirContenu(char grille [][TAILLE_GRILLE], char listeMots[MAX_MOTS][TAILLE_MOTS], int* tailleListeMots, FILE* fichier) {
   char buffer[256];

   for(int i = 0; i < TAILLE_GRILLE; ++i) {
      fgets(buffer, sizeof(buffer), fichier);
      strcpy(grille[i], buffer);
   }

   int taille = 0;
   while(fgets(buffer, 256, fichier) != NULL) {
      int i = 0;
      do{
         i++;
      } while (buffer[i] != '\n') ;
      buffer[i] = '\0';
      if(buffer[0] != '\0') {
         strcpy(listeMots[taille++], buffer);
      }
   }
   *tailleListeMots = taille;
}
void chercher(const char* mot, char grille[][TAILLE_GRILLE], int matrice[TAILLE_GRILLE][TAILLE_GRILLE]) {
   char premier = mot[0];
   int estTrouve = 0;
   char* motTrouve = NULL;
   for(int i = 0; i < TAILLE_GRILLE && !estTrouve; i++) {
      for(int j = 0; j < TAILLE_GRILLE && !estTrouve; j++) {
         if(premier == grille[i][j]) {
                estTrouve = extraireMot(motTrouve, mot, (int)strlen(mot), grille, i, j);
            if(estTrouve) {
             mettreAjour(matrice, i, j, (int) strlen(mot), estTrouve);
            }
         }
      }
   }
}


int extraireMot(char* mot, const char* motListe , int n, char grille[][TAILLE_GRILLE], int ligne, int colonne) {
   int haut = ligne+1;
   int gauche = colonne+1;
   int bas = TAILLE_GRILLE-ligne;
   int droite = TAILLE_GRILLE-colonne;
   if(haut>=n) {
      trouverParDirection(&mot, n, grille, ligne, colonne, 1);
      if(strcmp(mot, motListe) == 0)
         return 1;
   }
   if(gauche>=n ) {
      trouverParDirection(&mot, n, grille, ligne, colonne, 2);
      if(strcmp(mot, motListe) == 0)
         return 2;
   }
   if(bas>=n) {
      trouverParDirection(&mot, n, grille, ligne, colonne, 3);
      if(strcmp(mot, motListe) == 0)
         return 3;
   }
   if(droite>=n) {
      trouverParDirection(&mot, n, grille, ligne, colonne, 4);
      if(strcmp(mot, motListe) == 0)
         return 4;
   }
   return 0;
}

void trouverParDirection(char** mot, int tailleMot, char grille[][TAILLE_GRILLE], int ligne, int colonne, int direction) {
   *mot = (char*) malloc((tailleMot+5)*sizeof(char));
   int cpt=0;
   switch(direction) {
      case 1: for(int i = ligne; i > ligne-tailleMot; i--) {
            (*mot)[cpt++] = grille[i][colonne];
         }
         break;
      case 2: for(int i = colonne; i > colonne-tailleMot; i--) {
            (*mot)[cpt++] = grille[ligne][i];
         }
         break;
      case 3: for(int i = ligne; i <= ligne+tailleMot-1; i++) {
            (*mot)[cpt++] = grille[i][colonne];
         }
         break;
      case 4: for(int i = colonne; i <= colonne+tailleMot-1; i++) {
            (*mot)[cpt++] = grille[ligne][i];
         }
         break;
   }
   (*mot)[cpt] = '\0';
}

void mettreAjour(int matrice[TAILLE_GRILLE][TAILLE_GRILLE], int ligne, int colonne, int tailleMot, int direction) {
   switch(direction) {
      case 1: for(int i = ligne; i > ligne-tailleMot; i--) {
            matrice[i][colonne] = 1;
         }
         break;
      case 2: for(int i = colonne; i > colonne - tailleMot; i--) {
            matrice[ligne][i] = 1;
         }
         break;
      case 3: for(int i = ligne; i <= ligne+tailleMot-1; i++) {
            matrice[i][colonne] = 1;
         }
         break;
      case 4: for(int i = colonne; i <= colonne+tailleMot-1; i++) {
            matrice[ligne][i] = 1;
         }
         break;
   }
}

void trouverMotCache(char** motCache, char grille[][TAILLE_GRILLE], char listeMots[MAX_MOTS][TAILLE_MOTS], int n, int matrice[TAILLE_GRILLE][TAILLE_GRILLE]) {
    int x = 0;
   for(int i = 0; i < n; i++) {
      chercher(listeMots[i], grille, matrice);
   }
   *motCache = (char*)malloc(TAILLE_GRILLE*TAILLE_GRILLE*sizeof(char));
   for(int i = 0; i < TAILLE_GRILLE; i++) {
      for(int j = 0; j < TAILLE_GRILLE; j++) {
         if(matrice[i][j] == 0) {
            (*motCache)[x++] = grille[i][j];
         }
      }
   }
   (*motCache)[x] = '\0';
}


int main(int argc, char* argv[]) {
   char grille [TAILLE_GRILLE][TAILLE_GRILLE];
   char listeMots[MAX_MOTS][TAILLE_MOTS];
   int matrice[TAILLE_GRILLE][TAILLE_GRILLE] = {0};
   int tailleListeMots = 0;
   char* motCache = NULL;

   FILE* fichier = ouvrirFichier(argv[1]);
   obtenirContenu(grille, listeMots, &tailleListeMots, fichier);
   trouverMotCache(&motCache, grille, listeMots, tailleListeMots, matrice);
   printf("%s\n", motCache);

   return 0;
}
