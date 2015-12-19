#ifndef FONCTIONS_H_INCLUDED
#define FONCTIONS_H_INCLUDED

void gaming (int *grille, int *score);
void initZero (int *grille, int *score);
void sauteLigne (int nbLigne);
void afficheGrille(int *grille, int *score, int meilleurScore);
void bordureH();
void afficheLigne(int ligne,int *grille);
int tailleNombre(int nombre);
void clearScreen ();
void lectureGrille (int *grille, int *score);
void ecritureGrille (int *grille, int *score);
int moveLeft (int *grille,int *score);
int moveRight (int *grille,int *score);
int moveUp (int *grille,int *score);
int moveDown (int *grille,int *score);
void genereNombre(int *grille);
void resetFlags (int *grille);
int testGameOver(int *grille);
void menu (int *grille, int *score);
void triScore(int score, char *TabPseudo);
void gameOverScreen(int *grille, int *score);
int testVictoire(int *grille);
int victoireScreen(int *grille, int *score);
void changeColor(int nombre);
void color(int c);
void afficheScores(int *grille, int *score);
int testSave(int *grille, int *score);
void noSave(int *grille, int *score);
void tutoriel(int *grille, int *score);
int victoireTuto(int *grille);
int getMeilleurScore();
void initScores();

#endif // FONCTIONS_H_INCLUDED
