#include "../headers/distribution.h"
#include "../headers/environnement.h"
#include "../headers/particules.h"
#include "../headers/interface.h"

#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[]) {

    // Valeurs par défaut des paramètres
    int n = 10 ; float w = 50 ; float h = 50 ; float r = 1 ; float dt = 1 ; float t = 50;
    char* path = ".";

    // Génère aléatoirement une graine
    srand(time(NULL));

    // 7 arguments peuvent être donné au main
    if (argc >= 2)
        n = atoi(argv[1]);

    if (argc >= 3)
        w = atof(argv[2]);

    if (argc >= 4)
        h = atof(argv[3]);

    if (argc >= 5)
        r = atof(argv[4]);

    if (argc >= 6)
        dt = atof(argv[5]);

    if (argc >= 7)
        t = atoi(argv[6]);

    if (argc >= 8)
        path = argv[7];

    env e = create_environnement(n, w, h, r, dt);

    for (int i = 0 ; i<10 ; i++)
        move_particules(e);

    animate(e,path,get_w(e),get_h(e),t);
    free_environnement(e);
    return 0;
}