#include <stdlib.h>
#include <time.h>
#include <math.h> 
#include "../headers/distribution.h" 
#include <stdio.h>

float uniform(float a, float b){
    float alea = ((float) rand() )/RAND_MAX; // alea est un float aléatoire entre 0 et 1 choisi uniformément.
    return a + alea*(b-a);; // Ce float est entre a et b si alea est entre 0 et 1.
}

float normal(float mu, float sigma){
    // Si X1 et X2 sont deux varibles aléatoires indépendantes de loi uniforme sur [0,1],
    // X = cos(2*pi*X1)*sqrt(-2*log(X2)) suit une loi normal N(0,1).
    // Donc mu + sigma*X suit une loi normal N(mu,sigma). 
    float x1 = uniform(0,1);
    float x2 = uniform(0,1);
    float x = cos(2*3.14*x1)*sqrt(-2*log(x2));
    return mu + sigma*x;
}

float exponential(float k){
    // Si X suit loi uniforme sur [0,1],
    // X = -log(X)/k suit une loi exponentielle de paramètre k.
    float x = uniform(0,1);
    return -log(x)/k;
}

float discrete(int n, float* t, float* p){
    // Si note p1, ... , pn les probabilités du tableau p, comme la somme des probabilités vaut 1,
    // on peut partitionner l'intervalle [0,1] en [O, p1[ , [p1, p1+p2[ , ... , [p1+p2+...+p(n-1),  p1+...+p(n-1)+pn].
    // La probabilité qu'un float aléatoire entre 0 et 1 choisi uniformément soit dans l'intervalle [p1+...+p(i-1), p1+...+p(i-1)+p(i)[ est de pi.
    // On peut donc associer à chaque t[i-1] cet intervalle, la probabilité de tirer l'élément t[i-1] est bien de pi.    
    float alea = ((float) rand() ) /RAND_MAX; // On choisit un float entre 0 et 1 uniformément.
    float accumulateur = 0.0;
    int i=-1;
    // On regarde dans quel intervalle appartient alea.
    while((accumulateur<alea)&&(i<(n-1))){
        // Cette boucle termine car à chaque itération on incrémente i de 1.
        i+=1;
        accumulateur += p[i];
    }
    return t[i];
}
