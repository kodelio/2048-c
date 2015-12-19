#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "fonctions.h"

/*
But de la procédure : Lancer une partie ou reprendre une partie
int *grille : Tableau contenant les cases de la partie
int *score : Score de la partie
*/

void gaming (int *grille, int *score)
{
    // Initialisation des variables
    int successMove = 0;
    int gameOver = 0;
    int toucheAppuyee = 0;
    int toucheFleches = 0;
    int meilleurScore = getMeilleurScore();

    // Tant que la partie n'est pas perdue, on continue le jeu
    while (gameOver == 0)
    {
        // On récupère la touche appuyée
        while((toucheAppuyee=getch()) == 224)
        {
            toucheFleches=getch();

            // Touches directionnelles
            switch (toucheFleches)
            {
            // Touche gauche
            case 75:
                successMove = moveLeft(grille, score);
                break;
            // Touche droite
            case 77:
                successMove = moveRight(grille, score);
                break;
            // Touche haut
            case 72:
                successMove = moveUp(grille, score);
                break;
            // Touche bas
            case 80:
                successMove = moveDown(grille, score);
                break;
            }

            // Le déplacement a pu être effectué donc on peut générer un nouveau nombre
            if (successMove == 1)
            {
                genereNombre(grille);
            }
            resetFlags (grille);
            successMove = 0;
            // Affichage da le grille
            clearScreen();
            afficheGrille(grille, score, meilleurScore);

            // Partie gagnée
            if (*(grille+32)==0 && testVictoire(grille)==1)
            {
                *(grille+32)=1;
                victoireScreen(grille, score);
                clearScreen();
                afficheGrille(grille, score, meilleurScore);
            }

            // Patie perdue
            if (testGameOver(grille)==1)
            {
                gameOver=1;
                break;
            }
            // Partie non perdue: enregistrement de la partie dans le fichier save.txt
            else
            {
                ecritureGrille(grille,score);
            }
        }

        // Touche Esc: quitter la partie
        if(toucheAppuyee==27)
        {
            menu(grille, score);
        }
    }
    // Affichage du l'écran de partie perdue
    gameOverScreen(grille, score);
    // Retour au menu
    menu(grille,score);

}

/*
But de la procédure : Initialise la grille et le score à 0
int *grille : Tableau contenant les cases de la partie
int *score : Score de la partie
*/

void initZero (int *grille, int *score)
{
    *score = 0;
    int i;
    for (i=0; i<33; i++)
    {
        *(grille+i)=0;
    }
}

/*
But de la procédure : Procédure pour faire des sauts de ligne
nbLigne : Nombre de lignes à sauter
*/

void sauteLigne (int nbLigne)
{
    int i;
    for (i=0; i<nbLigne; i++)
    {
        printf("\n");
    }
}

/*
But de la procédure : Affichage de la grille avec le score
int *grille : Tableau contenant les cases de la partie
int *score : Score de la partie
int meilleurScore : Meilleur score
*/

void afficheGrille(int *grille, int *score, int meilleurScore)
{
    // Affichage du score actuel
    printf("Score actuel : %d\n", *score);

    // Affichage du meilleur score
    if (*score>=meilleurScore)
    {
        printf("Meilleur Score : %d\n", *score);
    }
    else
    {
        printf("Meilleur Score : %d\n", meilleurScore);
    }

    printf("\n");
    // Affichage des lignes de la grille
    bordureH();
    afficheLigne(0,grille);
    bordureH();
    afficheLigne(1,grille);
    bordureH();
    afficheLigne(2,grille);
    bordureH();
    afficheLigne(3,grille);
    bordureH();
}

/*
But de la procédure : Affiche les bordures horizontales de la grille
*/

void bordureH()
{
    int i;
    for (i=0; i<29; i++)
    {
        printf("-");
    }
    printf("\n");
}

/*
But de la procédure : Affiche la ligne contenant la valeur ainsi que les deux lignes vides qui l'entourent
int ligne : Numéro de la ligne à afficher (0, 1, 2 ou 3)
int *grille : Tableau contenant les cases de la partie
*/

void afficheLigne(int ligne,int *grille)
{
    int i,j,k,l,nombre;
    l=0;
    for(i=0; i<3; i++)
    {
        // Affichage des deux lignes vides
        if (i!=1)
        {
            for (j=0; j<4; j++)
            {
                printf("|      ");
            }
            printf("|\n");
        }
        // Affichage de la ligne contenant le nombre à afficher
        else
        {
            for (j=0; j<4; j++)
            {
                printf("|");
                nombre=*(grille + l + (ligne * 8));
                for(k=0; k<6-tailleNombre(nombre); k++)
                {
                    printf(" ");
                }
                if (nombre==0)
                {
                    printf(" ");
                }
                else
                {
                    changeColor(nombre);
                    printf("%d", nombre);
                    color(15);
                }
                l+=2;
            }
            printf("|\n");
        }
    }
}

/*
But de la fonction : Renvoie la taille d'un nombre (en digit)
int nombre : Nombre dont on souhaite connaitre la taille
*/

int tailleNombre(int nombre)
{
    int i=1;
    int j=0;
    while(nombre>=i)
    {
        i*=10;
        j++;
    }
    if (j==0) return 1;
    return j;
}

/*
But de la procédure : Procédure pour effacer le contenu de la console
*/

void clearScreen ()
{
    system("cls");
}

/*
But de la procédure : Lecture du fichier save.txt pour récupérer la grille et le score enregistrés
int *grille : Tableau contenant les cases de la partie
int *score : Score de la partie
*/

void lectureGrille (int *grille, int *score)
{
    int i;
    FILE *fichierSave = NULL;
    fichierSave = fopen("save.txt", "r");

    for (i=0; i<16; i++)
    {
        fscanf(fichierSave, "%d", (grille + i*2));
    }
    fscanf(fichierSave, "%d", score);
    fscanf(fichierSave, "%d", (grille+32));
    fclose(fichierSave);
}

/*
But de la procédure : Enregistre la grille et le score dans le fichier save.txt
int *grille : Tableau contenant les cases de la partie
int *score : Score de la partie
*/

void ecritureGrille (int *grille, int *score)
{
    int i;
    FILE *fichierSave = NULL;
    fichierSave = fopen("save.txt", "w");

    for (i=0; i<16; i++)
    {
        fprintf(fichierSave, "%d\n", *(grille+i*2));
    }
    fprintf(fichierSave, "%d\n", *score);
    fprintf(fichierSave, "%d", *(grille+32));
    fclose(fichierSave);
}

/*
But des fonctions : Fonctions de déplacement et d'addition dans la grille puis incrémentation du score
Return: renvoie 1 si un mouvement a été effectué, sinon 0
int *grille : Tableau contenant les cases de la partie
int *score : Score de la partie
*/

int moveLeft (int *grille,int *score)
{
    int ligne;
    int colonne;

    int continuer;
    int aajoute = 0;
    int adeplace = 0;
    int mouvementvalable=0;

    for(ligne=0; ligne<4; ligne++)
    {
        continuer = 1;

        while (continuer == 1)
        {
            // Déplacement des cases
            for (colonne=1; colonne<4; colonne++)
            {
                if (*(grille + (colonne*2-2) + (ligne * 8)) == 0 && *(grille + colonne*2 + (ligne * 8)) != 0 )
                {
                    *(grille + (colonne*2-2) + (ligne * 8))=*(grille + colonne*2 + (ligne * 8));//deplace le nb
                    *(grille + (colonne*2-1) + (ligne * 8))=*(grille + (colonne*2+1) + (ligne * 8));// deplace le flag
                    *(grille + colonne*2 + (ligne * 8)) = 0;
                    *(grille + (colonne*2+1) + (ligne * 8)) = 0;
                    adeplace = 1;
                    mouvementvalable=1;
                }
            }

            // Ajout des cases
            for (colonne=1; colonne<4; colonne++)
            {
                if (*(grille + (colonne*2-2) + (ligne * 8)) == *(grille + colonne*2 + (ligne * 8)) && *(grille + (colonne*2-2) + (ligne * 8)) !=0 && *(grille + (colonne*2-1) + (ligne * 8)) == 0 && *(grille + colonne*2+1 + (ligne * 8)) == 0 )
                {
                    *(grille + (colonne*2-2) + (ligne * 8)) = *(grille + (colonne*2-2) + (ligne * 8))*2;
                    *score += *(grille + (colonne*2-2) + (ligne * 8));
                    *(grille + (colonne*2-1) + (ligne * 8)) = 1;
                    *(grille + colonne*2 + (ligne * 8)) = 0;
                    aajoute = 1;
                    mouvementvalable=1;
                }
            }

            // Sortie de la boucle: aucun déplacement + aucun ajout
            if(adeplace == 0 && aajoute == 0)
            {
                continuer = 0;
            }
            adeplace = 0;
            aajoute = 0;
        }
    }
    return mouvementvalable;
}

int moveRight (int *grille,int *score)
{
    int ligne;
    int colonne;

    int continuer;
    int aajoute = 0;
    int adeplace = 0;
    int mouvementvalable=0;

    for(ligne=0; ligne<4; ligne++)
    {
        continuer = 1;

        while (continuer == 1)
        {
            // Déplacement des cases
            for (colonne=3; colonne>0; colonne--)
            {
                if (*(grille + (colonne*2) + (ligne * 8)) == 0 && *(grille + colonne*2-2 + (ligne * 8)) != 0 )
                {
                    *(grille + (colonne*2) + (ligne * 8))=*(grille + colonne*2-2 + (ligne * 8));//deplace le nb
                    *(grille + (colonne*2+1) + (ligne * 8))=*(grille + (colonne*2-1) + (ligne * 8));// deplace le flag
                    *(grille + colonne*2-2 + (ligne * 8)) = 0;
                    *(grille + (colonne*2-1) + (ligne * 8)) = 0;
                    adeplace = 1;
                    mouvementvalable=1;
                }
            }

            // Ajout des cases
            for (colonne=3; colonne>0; colonne--)
            {
                if (*(grille + (colonne*2-2) + (ligne * 8)) == *(grille + colonne*2 + (ligne * 8)) && *(grille + (colonne*2-2) + (ligne * 8)) != 0 && *(grille + (colonne*2-1) + (ligne * 8)) == 0 && *(grille + colonne*2+1 + (ligne * 8)) == 0 )
                {
                    *(grille + (colonne*2) + (ligne * 8)) = *(grille + (colonne*2) + (ligne * 8))*2;
                    *score += *(grille + (colonne*2) + (ligne * 8));
                    *(grille + (colonne*2+1) + (ligne * 8)) = 1;
                    *(grille + colonne*2-2 + (ligne * 8)) = 0;
                    aajoute = 1;
                    mouvementvalable=1;
                }
            }

            // Sortie de la boucle: aucun déplacement + aucun ajout
            if(adeplace == 0 && aajoute == 0)
            {
                continuer = 0;
            }
            adeplace = 0;
            aajoute = 0;
        }
    }
   return mouvementvalable;
}

int moveUp (int *grille,int *score)
{
    int ligne;
    int colonne;

    int continuer;
    int aajoute = 0;
    int adeplace = 0;
    int mouvementvalable = 0;

    for(colonne=0; colonne<4; colonne++)
    {
        continuer = 1;

        while (continuer == 1)
        {
            // Déplacement des cases
            for (ligne=1; ligne<4; ligne++)
            {
                if (*(grille + colonne*2 + ((ligne-1) * 8)) == 0 && *(grille + colonne*2 + (ligne * 8)) != 0 )
                {
                    *(grille + (colonne*2) + ((ligne-1) * 8))=*(grille + colonne*2 + (ligne * 8));//deplace le nb
                    *(grille + (colonne*2+1) + ((ligne-1) * 8))=*(grille + (colonne*2+1) + (ligne * 8));// deplace le flag
                    *(grille + colonne*2 + (ligne * 8)) = 0;//reset le nombre
                    *(grille + (colonne*2+1) + (ligne * 8)) = 0;//reset le flag
                    mouvementvalable=1;
                    adeplace = 1;
                }
            }

            // Ajout des cases
            for (ligne=1; ligne<4; ligne++)
            {
                if (*(grille + (colonne*2) + ((ligne-1) * 8)) == *(grille + colonne*2 + (ligne * 8)) && *(grille + (colonne*2) + ((ligne-1) * 8)) != 0 && *(grille + (colonne*2+1) + ((ligne-1) * 8)) == 0 && *(grille + colonne*2+1 + (ligne * 8)) == 0 )
                {
                    *(grille + (colonne*2) + ((ligne-1) * 8)) = *(grille + (colonne*2) + (ligne * 8))*2;//ajout
                    *score += *(grille + (colonne*2) + ((ligne-1) * 8));
                    *(grille + (colonne*2+1) + ((ligne-1) * 8)) = 1;//chmt flag
                    *(grille + colonne*2 + (ligne * 8)) = 0;//reset nombre
                    aajoute = 1;
                    mouvementvalable=1;
                }
            }

            // Sortie de la boucle: aucun déplacement + aucun ajout
            if(adeplace == 0 && aajoute == 0)
            {
                continuer = 0;
            }
            adeplace = 0;
            aajoute = 0;
        }
    }
   return mouvementvalable;
}

int moveDown (int *grille,int *score)
{
    int ligne;
    int colonne;

    int continuer;
    int aajoute = 0;
    int adeplace = 0;
    int mouvementvalable = 0;

    for(colonne=0; colonne<4; colonne++)
    {
        continuer = 1;

        while (continuer == 1)
        {
            // Déplacement des cases
            for (ligne=3; ligne>0; ligne--)
            {
                if (*(grille + colonne*2 + ((ligne-1) * 8)) != 0 && *(grille + colonne*2 + (ligne * 8)) == 0 )
                {
                    *(grille + (colonne*2) + (ligne * 8))=*(grille + colonne*2 + ((ligne-1) * 8));//deplace le nb
                    *(grille + (colonne*2+1) + (ligne * 8))=*(grille + (colonne*2+1) + ((ligne-1) * 8));// deplace le flag
                    *(grille + colonne*2 + ((ligne-1) * 8)) = 0;//reset le nombre
                    *(grille + (colonne*2+1) + ((ligne-1) * 8)) = 0;//reset le flag
                    adeplace = 1;
                    mouvementvalable=1;
                }
            }

            // Ajout des cases
            for (ligne=3; ligne>0; ligne--)
            {
                if (*(grille + (colonne*2) + ((ligne-1) * 8)) == *(grille + colonne*2 + (ligne * 8)) && *(grille + (colonne*2) + ((ligne-1) * 8)) != 0 && *(grille + (colonne*2+1) + ((ligne-1) * 8)) == 0 && *(grille + colonne*2+1 + (ligne * 8)) == 0 )
                {
                    *(grille + (colonne*2) + (ligne * 8)) = *(grille + (colonne*2) + (ligne * 8))*2;//ajout
                    *score += *(grille + (colonne*2) + (ligne * 8));
                    *(grille + (colonne*2+1) + (ligne * 8)) = 1;//chmt flag
                    *(grille + colonne*2 + ((ligne-1) * 8)) = 0;//reset nombre
                    aajoute = 1;
                    mouvementvalable=1;
                }
            }

            // Sortie de la boucle: aucun déplacement + aucun ajout
            if(adeplace == 0 && aajoute == 0)
            {
                continuer = 0;
            }
            adeplace = 0;
            aajoute = 0;
        }
    }
   return mouvementvalable;
}

/*
But de la procédure : Génère un nombre aléatoire dans une case libre
int *grille : Tableau contenant les cases de la partie
*/

void genereNombre (int *grille)
{
    int nombreAjouter = 0, i=0, compteur = 0, position = 0, compteur2 = 0;
    int positionTrouvee = 0;

    // On genere un nombre aléatoire: 2 ou 4
    nombreAjouter = ((rand()%2) *2)+2;

    // On récupère le nombre de cases vides
    for (i=0; i<16; i++)
    {
        if (*(grille+i*2) == 0)
        {
            compteur ++;
        }
    }

    // On génère aléatoirement la postion du nouveau nombre en fonction du nombre de cases vides
    position = (rand()%compteur)+1;
    if(position!=0)
    {
        i=0;
        // Tant que la nouvelle position n'est pas une case vide
        while(positionTrouvee==0)
        {
            if (*(grille+i*2) == 0)
            {
                compteur2 ++;
            }
            if (compteur2 == position)
            {
                *(grille+i*2) = nombreAjouter;
                positionTrouvee=1;
            }
            i++;
        }
    }
}

/*
But de la procédure : Initialise les booléens qui gèrent la mise en mémoire d'une addition
int *grille : Tableau contenant les cases de la partie
*/

void resetFlags (int *grille)
{
    int i;
    for (i=0; i<16; i++)
    {
        *(grille + i*2 + 1)=0;
    }
}

/*
But de la fonction : Renvoie 1 si la partie est perdue, sinon 0
int *grille : Tableau contenant les cases de la partie
*/

int testGameOver(int* grille)
{
    int i,j;
    int casePrecedente, caseSuivante;

    // Renvoie 0 si une case est vide
    for (i=0; i<16; i++)
    {
        if (*(grille + i*2)==0)
        {
            return 0;
        }
    }

    // Renvoie 0 si deux cases adjacentes horizontalement sont égales
    for (i=0; i<4; i++)
    {
        for (j=0; j<3; j++)
        {
            casePrecedente = *(grille + j*2 + i*8);
            caseSuivante = *(grille + j*2 + i*8 + 2);
            if (casePrecedente == caseSuivante)
            {
                return 0;
            }
        }
    }

    // Renvoie 0 si deux cases adjacentes verticalement sont égales
    for (i=0; i<3; i++)
    {
        for (j=0; j<4; j++)
        {
            casePrecedente = *(grille + j*2 + i*8);
            caseSuivante = *(grille + j*2 + i*8 + 8);
            if (casePrecedente == caseSuivante)
            {
                return 0;
            }
        }
    }
    return 1;
}

/*
But de la procédure : Affiche le menu principal
int *grille : Tableau contenant les cases de la partie
int *score : Score de la partie
*/

void menu (int *grille, int *score)
{
    int menu = 0;
    // Affichage du menu
    clearScreen();
    printf("Bienvenue dans le 2048 !");

    sauteLigne(5);
    printf("1) Commencer une nouvelle partie");
    sauteLigne(2);
    printf("2) Reprendre la partie");
    sauteLigne(2);
    printf("3) Afficher les scores");
    sauteLigne(2);
    printf("4) Demarrer le tutoriel");
    sauteLigne(2);
    printf("5) Quitter");
    sauteLigne(3);

    // On récupère la touche appuyée
    while (menu<49 || menu>53)
    {
        menu = getch();
    }

    switch(menu)
    {
    // Nouvelle partie
    case 49:
        initZero(grille, score);
        genereNombre(grille);
        genereNombre(grille);
        clearScreen();
        afficheGrille(grille, score , getMeilleurScore());
        gaming(grille, score);
        break;

    // Charger une partie
    case 50:
        initZero(grille, score);
        if (testSave(grille, score) == 1)
        {
            lectureGrille(grille, score);
            clearScreen();
            afficheGrille(grille, score, getMeilleurScore());
            gaming(grille, score);
        }
        else
        {
            clearScreen();
            noSave(grille, score);
        }
        break;

    // 10 Meilleurs Scores
    case 51:
        afficheScores(grille, score);
        break;

    // Tutoriel
    case 52:
        tutoriel(grille, score);
        break;

    // Quitter
    case 53:
        exit(0);
        break;
    }
}

/*
But de la procédure : Procédure pour trier les 10 meilleurs scores dans le fichier scores.txt avec ajout du dernier si besoin
int score : Score de la partie
char *TabPseudo : Pseudo du joueur
*/

void triScore(int score, char *TabPseudo)
{
    int i, j;
    int flagMove=0;
    int TabScores[11];
    char TabPseudos[11][20];
    int SaveScore;

    FILE *fichierScore = NULL;
    fichierScore = fopen("scores.txt", "r");
    i=0;

    while (i<11)
    {
        fscanf(fichierScore, "%d", &SaveScore );

        if (score<SaveScore || flagMove==1)
        {
            TabScores[i]=SaveScore;
            fscanf(fichierScore, "%s", TabPseudos[i] );
        }
        else if (flagMove == 0)
        {
            flagMove=1;
            TabScores[i]=score;
            for (j=0; j<20; j++)
            {
                TabPseudos[i][j]=TabPseudo[j];
            }
            i++;
            TabScores[i]=SaveScore;
            fscanf(fichierScore, "%s", TabPseudos[i] );
        }
        i++;
    }
    fclose(fichierScore);

    fichierScore = fopen("scores.txt", "w");

    // On réécrit les 10 meilleurs scores avec les pseudos correspondant
    for (i=0; i<10; i++)
    {
        fprintf(fichierScore, "%d\n", TabScores[i]);
        fprintf(fichierScore, "%s\n", TabPseudos[i]);
    }
    fclose(fichierScore);
}

/*
But de la procédure : Ecran affiché lorsqu'une partie est perdue
int *grille : Tableau contenant les cases de la partie
int *score : Score de la partie
*/

void gameOverScreen(int *grille, int *score)
{
    int i;
    char TabPseudo[20];

    printf("Vous avez perdu\n");
    printf("Votre score : %d\n", *score);
    printf("Entrez votre pseudo (max 19 caracteres) : ");
    scanf("%s", TabPseudo);

    i=0;
    // Entree du pseudo
    while(TabPseudo[i]!='\0')
    {
        i++;
        if (i==20)
        {
            clearScreen();
            printf("\nErreur vous n'avez pas respecte la limite !\n");
            exit(99);
        }
    }

    triScore(*score, TabPseudo);
    initZero(grille, score);
    ecritureGrille(grille, score);
}

/*
But de la fonction : Renvoie 1 si la partie est gagnée, sinon 0
int *grille : Tableau contenant les cases de la partie
*/

int testVictoire(int *grille)
{
    int i;
    for (i=0; i<16; i++)
    {
        if (*(grille + i*2)==2048)
        {
            return 1;
        }
    }
    return 0;
}

/*
But de la fonction : Ecran affiché lorsqu'une partie est gagnée
Return: Renvoie 0 pour sortir de la fonction
int *grille : Tableau contenant les cases de la partie
int *score : Score de la partie
*/

int victoireScreen(int *grille, int *score)
{
    int continuerJouer=0;
    printf("Vous avez gagne\n");
    printf("Votre score actuel : %d\n", *score);
    printf("Voulez vous continuer a jouer ? (O/N)");

    // On récupère la touche enfoncée
    while (continuerJouer!=78 && continuerJouer!=79 && continuerJouer!=110 && continuerJouer!=111)
    {
        continuerJouer = getch();
        switch(continuerJouer)
        {
        // Touche 'n': quitter
        case 110:
            ecritureGrille(grille, score);
            menu(grille, score);
            break;
        // Touche 'N': quitter
        case 78:
            ecritureGrille(grille, score);
            menu(grille, score);
            break;
        // Touche 'o': continuer
        case 111:
            return 0;
            clearScreen();
            afficheGrille(grille, score, getMeilleurScore());
            gaming(grille, score);
            break;
        // Touche 'O': continuer
        case 79:
            return 0;
            clearScreen();
            afficheGrille(grille, score, getMeilleurScore());
            gaming(grille, score);
            break;
        }
    }
}

/*
But de la procédure : Choix de la couleur à afficher en fonction du nombre à afficher
int nombre : Nombre à afficher
*/

void changeColor(int nombre)
{
    switch (nombre)
    {
    case 2:
        color(10);
        break;
    case 4:
        color(14);
        break;
    case 8:
        color(12);
        break;
    case 16:
        color(11);
        break;
    case 32:
        color(13);
        break;
    case 64:
        color(9);
        break;
    case 128:
        color(6);
        break;
    case 256:
        color(5);
        break;
    case 512:
        color(3);
        break;
    case 1024:
        color(4);
        break;
    }
}

/*
But de la procédure : Gestion de la couleur de la console
int c : Nombre allant de 0 à 15, chaque valeur correspondant à une couleur différente
*/

void color(int c)
{
    HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H,0*16+c);
}

/*
But de la procédure : Affiche les 10 meilleurs scores enregistrés dans le fichier scores.txt
int *grille : Tableau contenant les cases de la partie
int *score : Score de la partie
*/

void afficheScores(int *grille, int *score)
{
    int i;
    int quitter;
    int scoreAffiche;
    char pseudo[20];
    FILE *fichierScore = NULL;
    fichierScore = fopen("scores.txt", "r");

    clearScreen();
    printf("10 meilleurs scores:\n\n");

    for (i=0; i<20; i++)
    {
        // Affichage des scores
        if (i%2 == 0)
        {
            fscanf(fichierScore, "%d", &scoreAffiche );
            if (scoreAffiche != 0)
            {
                printf("%d: %d ", (i/2)+1, scoreAffiche);
            }
        }
        // Affichage des pseudos
        else
        {
            fscanf(fichierScore, "%s", pseudo );
            if (scoreAffiche != 0)
            {
                printf("%s\n", pseudo);
            }
        }
    }
    fclose(fichierScore);

    printf("\n");
    printf("Quitter : Esc\n");
    // Attente de la touche Esc: quitter
    quitter=0;
    while (quitter != 27)
    {
        quitter = getch();
    }
    if (quitter == 27)
    {
        menu(grille, score);
    }
}

/*
But de la fonction : Renvoie 1 si une partie est enregistrée dans le fichier save.txt, sinon 0
int *grille : Tableau contenant les cases de la partie
int *score : Score de la partie
*/

int testSave(int *grille, int *score)
{
    int i;

    lectureGrille(grille, score);

    for (i=0; i<16; i++)
    {
        if (*(grille + i*2)!=0)
        {
            return 1;
        }
    }
    return 0;
}

/*
But de la procédure : Affichage lorsqu'il n'existe pas de partie enregistrée
int *grille : Tableau contenant les cases de la partie
int *score : Score de la partie
*/

void noSave(int *grille, int *score)
{
    int quitter;
    printf("Il n'y a pas de partie enregistree.");
    sauteLigne(2);
    printf("Quitter : Esc\n");

    quitter=0;
    while (quitter != 27)
    {
        quitter = getch();
    }
    if (quitter == 27)
    {
        menu(grille, score);
    }
}

/*
But de la procédure : Tutoriel
int *grille : Tableau contenant les cases de la partie
int *score : Score de la partie
*/

void tutoriel(int *grille, int *score)
{
    int mouvement=0;

    initZero(grille, score);
    grille[18]=2;
    grille[22]=2;

    clearScreen();
    printf("Bienvenue dans le tutoriel du 2048.\n\n");
    printf("Servez-vous des fleches directionnelles pour effectuer un deplacement.\n");
    printf("Pour sortir de la partie, appuyez sur Esc.\n\n");
    printf("La partie est automatiquement enregistree a chaque deplacement effectue.\n\n");
    printf("La partie est gagnee lorsque le nombre 2048 est atteint.\n");
    printf("La partie est perdue lorsqu'aucun deplacement n'est possible.\n\n");
    system("pause");

    clearScreen();
    printf("Maintenant, appuyez sur la touche droite pour faire combiner les deux nombres.\n\n");

    bordureH();
    afficheLigne(0,grille);
    bordureH();
    afficheLigne(1,grille);
    bordureH();
    afficheLigne(2,grille);
    bordureH();
    afficheLigne(3,grille);
    bordureH();
    // Attente de la fleche directionnelle droite
    while (mouvement != 77)
    {
        mouvement = getch();
    }
    mouvement=0;
    moveRight(grille, score);
    genereNombre(grille);

    while(victoireTuto(grille)==0)
    {
        int successMove;

        clearScreen();
        printf("Bravo ! Maintenant, essayez d'atteindre 64.\n\n");

        bordureH();
        afficheLigne(0,grille);
        bordureH();
        afficheLigne(1,grille);
        bordureH();
        afficheLigne(2,grille);
        bordureH();
        afficheLigne(3,grille);
        bordureH();

        while (mouvement==0)
        {
            mouvement=getch();
        }
        switch(mouvement)
        {
        case 75:
            successMove = moveLeft(grille, score);
            break;
        case 77:
            successMove = moveRight(grille, score);
            break;
        case 72:
            successMove = moveUp(grille, score);
            break;
        case 80:
            successMove = moveDown(grille, score);
            break;
        }
        if (successMove == 1)
        {
            genereNombre(grille);
        }
        resetFlags (grille);
        successMove = 0;
        mouvement=0;
    }
    clearScreen();
    printf("Bravo ! Maintenant, essayez d'atteindre 64.\n\n");
    bordureH();
    afficheLigne(0,grille);
    bordureH();
    afficheLigne(1,grille);
    bordureH();
    afficheLigne(2,grille);
    bordureH();
    afficheLigne(3,grille);
    bordureH();
    printf("\nFelicitations, vous avez fini le tutoriel.\n\n");

    system("pause");
    menu(grille, score);
}

/*
But de la fonction : Renvoie 1 si le tutoriel est réussi, sinon 0
int *grille : Tableau contenant les cases de la partie
*/

int victoireTuto(int *grille)
{
    int i;

    for (i=0; i<16; i++)
    {
        if (*(grille+(i*2)) == 64)
        {
            return 1;
        }
    }
    return 0;
}

/*
But de la fonction : Récupère le meilleur score du fichier score.txt
Return: Renvoie le meilleur score
*/

int getMeilleurScore()
{
    int meilleurScore;

    FILE *fichierScore = NULL;
    fichierScore = fopen("scores.txt", "r");
    fscanf(fichierScore, "%d", &meilleurScore);
    fclose(fichierScore);

    return(meilleurScore);
}

/*
But de la fonction : Test la présence du fichier scores.txt et initialise 10 scores nuls si le fichier n'existe pas
*/

void initScores()
{
    int i;

    FILE *fichierScore = NULL;
    fichierScore = fopen("scores.txt", "r");
    if (fichierScore == NULL)
    {
        fclose(fichierScore);
        fichierScore = fopen("scores.txt", "w");
        for (i=0; i<10; i++)
        {
            fprintf(fichierScore, "0\n");
            fprintf(fichierScore, " \n");
        }
        fprintf(fichierScore, "\n");
    }
    fclose(fichierScore);
}


