#include "../headers/distribution.h"
#include "../headers/environnement.h"
#include "../headers/particules.h"
#include "../headers/interface.h"

#include <stdlib.h>

int main(int argc, char* argv[]) {

    if (argc != 8) {
        exit(EXIT_FAILURE);
    }

    int n = atoi(argv[1]);
    float w = atof(argv[2]);
    float h = atof(argv[3]);
    float r = atof(argv[4]);
    float dt = atof(argv[5]);

    env e = create_environnement(n, w, h, r, dt);

    for (int i = 0 ; i<10 ; i++)
        move_particules(e);
    
    char* path = argv[7];
    int t = atoi(argv[6]);

    animate(e,path,get_w(e),get_h(e),t);
    free_environnement(e);
    return 0;
}