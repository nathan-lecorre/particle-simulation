#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define M_PI 3.14159265358979323846

typedef struct {
    double x;
    double y;
} Particule;

// Fonction initialisant la position de la particule témoin et des N voisins de cette particule dans le cercle unitaire 
void initialiser_particules(int N, Particule *particules) {
    // La particule témoin (P1) est placée en 1, soit les coordonnées (1, 0)
    particules[0].x = 1.0;
    particules[0].y = 0.0;

    // Répartition uniforme des N-1 autres particules sur le cercle [0, 2pi]
    for (int i = 1; i < N; i++) {
        double xi = ((double)rand() / (double)RAND_MAX) * 2.0 * M_PI;
        particules[i].x = cos(xi);
        particules[i].y = sin(xi);
    }
}

// Fonction pour normaliser un angle entre -PI et PI pour faciliter le test
double normaliser_angle(double angle) {
    while (angle > M_PI) angle -= 2.0 * M_PI;
    while (angle < -M_PI) angle += 2.0 * M_PI;
    return angle;
}

// Fonction calculant la moyenne empirique des positions des particules se situant dans la portion d'angle [-alpha,alpha]
Particule calculer_moyenne_empirique(int N, Particule *particules, double alpha) {
    double somme_x = 0.0;
    double somme_y = 0.0;
    int compteur = 0;

    // On parcourt les voisins (à partir de l'indice 1, car l'indice 0 est le témoin)
    for (int i = 1; i < N; i++) {
        // Calcul de l'angle de la particule à partir de ses coordonnées (x,y)
        double angle = atan2(particules[i].y, particules[i].x);
        
        // Normalisation pour vérifier si l'angle est dans [-alpha, alpha]
        double theta = normaliser_angle(angle);

        if (theta >= -alpha && theta <= alpha) {
            somme_x += particules[i].x;
            somme_y += particules[i].y;
            compteur++;
        }
    }

    Particule B = {0.0, 0.0};
    if (compteur > 0) {
        B.x = somme_x / compteur;
        B.y = somme_y / compteur;
    }
    
    printf("Nombre de voisins actifs dans le cone : %d\n", compteur);
    return B;
}

void simuler_et_sauvegarder(double alpha) {
    FILE *f = fopen("convergence.dat", "w");
    if (f == NULL) return;

    double valeur_theorique = sin(alpha) / alpha;
    srand(time(NULL));

    fprintf(f, "%s %s %s", " N ", " B_emp ", " B_th\n ");

    // On fait varier N de 100 à 100 000 avec un pas de 100
    for (int N = 100; N <= 100000; N += 100) {
        double somme_x = 0.0;
        int compteur = 0;

        for (int i = 0; i < N; i++) {
            double xi = ((double)rand() / (double)RAND_MAX) * 2.0 * M_PI;
            double theta = normaliser_angle(xi);

            if (theta >= -alpha && theta <= alpha) {
                somme_x += cos(theta);
                compteur++;
            }
        }

        if (compteur > 0) {
            double Bx = somme_x / compteur;
            // Colonnes : N | Valeur Empirique B.x | Valeur Théorique
            fprintf(f, "%d %f %f\n", N, Bx, valeur_theorique);
        }
    }

    fclose(f);
    printf("Donnees de simulation enregistrees dans 'convergence.dat'\n");
}

// ---------------------------------------------------------------------------------------------- //
// --------------------------------------------- MAIN ------------------------------------------- //
// ---------------------------------------------------------------------------------------------- // 

int main() {

    // Question 4 

    srand(time(NULL));
    int N = 10; // Exemple avec 10 particules au total
    Particule *mes_particules = malloc(N * sizeof(Particule));
    initialiser_particules(N, mes_particules);
    
    printf("Question 4 : \n\n");

    printf("Particule témoin : (%.3f, %.3f)\n", mes_particules[0].x, mes_particules[0].y);
    // Affichage pour vérification
    for (int i = 1; i < N; i++) {
        printf("Particule %d : (%.3f, %.3f)\n", i + 1, mes_particules[i].x, mes_particules[i].y);
    }

    printf("\n");

    // Question 5 

    printf("Question 5 : \n\n");

    // On appelle la fonction UNE SEULE FOIS et on stocke le résultat dans une variable locale
    Particule B = calculer_moyenne_empirique(N, mes_particules, M_PI/2); // test avec alpha = PI/2

    // On affiche les membres de la structure stockée dans B
    printf("Particule moyenne : (%.3f, %.3f)\n", B.x, B.y);
    printf("\n");

    // Question 6 

    printf("Question 6 : \n\n");
    double alpha = M_PI / 2; // test avec alpha = PI/2

    double B_théorique = sin(alpha)/alpha; 
    printf("B_théorique : %f\n", B_théorique);
    simuler_et_sauvegarder(alpha);

    free(mes_particules);
    return 0;
}