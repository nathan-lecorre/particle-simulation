#include "./headers/distribution.h"
#include "./headers/environnement.h"
#include "./headers/particules.h"
#include "./headers/interface.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void get_coordinates(env e) {

    FILE* f = fopen("./tache_f3.csv","w");

    float w = get_w(e);
    float h = get_h(e);
    particule centre = create_particule(w/2, h/2, 0, 0);

    for (int i = 0 ; i < get_n(e) ; i++) {
        particule p = get_particule(e,i);
        float val = distance(p,centre)/10;

        char str[1024];
        sprintf(str, "%f\n", val);
        fwrite(&str,(val >= 10) ? 10 : 9,1,f);
    }

    free_particule(centre);
}

int main() {

    // Valeurs des paramètres
    int n = 1000 ; float w = 500 ; float h = 500 ; float r = 0 ; float dt = 1 ; float t = 100;
    char* path = ".";

    // Génère aléatoirement une graine
    srand(time(NULL));

    env e = create_environnement(n, w, h, r, dt);

    for (int i = 0 ; i < t ; i++) {
        move_particules_random(e);
    }

    get_coordinates(e);

    free_environnement(e);
    return 0;
}