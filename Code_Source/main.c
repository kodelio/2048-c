#include <stdio.h>
#include <stdlib.h>
#include <time.h> // Biblioth�que pour le random
#include <windows.h> // Biblioth�que pour les couleurs
#include "fonctions.h"

int main()
{
    int grille[33]; // Initialise la grille de 32 cases (nombres + flags)
    int score = 0; // Initialisation du score

    srand(time(NULL)); // Initialisation de rand

    initScores(); // Initialisation du fichier score

    menu (grille, &score); // Affiche le menu

    return 0;
}// Fin du main
