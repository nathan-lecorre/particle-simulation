#include "optimization.h"
#include "../../headers/distribution.h"

#include <stdbool.h>
#include <assert.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>

#define SPEED 1.0f 

#ifndef M_PI
#define M_PI (atan(1)*4)
#endif

void revert_and_dup(const_env src, env dest) {
    assert(get_n(src) == get_n(dest));
    int n = get_n(src);
    for (int i = 0; i < n; i++) {
        particule p_src = get_particule(src, i);
        particule p_dest = get_particule(dest, i);
        set_position(p_dest, get_x(p_src), get_y(p_src));
        set_speed(p_dest, -get_vx(p_src), -get_vy(p_src));
    }
}

void cancel_last_collision(env e) {
    for (int i = 0; i < get_n(e); i++) {
        particule p_i = get_particule(e, i);
        for (int j = 0; j < get_n(e); j++) {
            if (j == i) continue; 
            particule p_j = get_particule(e, j);
            if (distance(p_i, p_j) <= get_r(e))
                set_speed(p_i, 0, -get_vy(p_i));
        }
    }
}

#define max2(a, b) ((a) > (b) ? (a) : (b))
#define max3(a, b, c) ((a) > (b) ? max2(a, c) : max2(b, c))
#define MAX_SELECTOR(a, b, c, d, ...) d
#define max(...) MAX_SELECTOR(__VA_ARGS__, max3, max2)(__VA_ARGS__)

#define min2(a, b) (-max2(-(a), -(b)))
#define min3(a, b, c) (-max3(-(a), -(b), -(c)))
#define min(...) MAX_SELECTOR(__VA_ARGS__, min3, min2)(__VA_ARGS__)

int compare_norme2(const void* a, const void* b) {
    const float *xa = (const float*)a, *ya = xa+1;
    const float *xb = (const float*)b, *yb = xb+1;
    return 2*(sqrt(pow(*xa, 2) + pow(*ya, 2)) >= sqrt(pow(*xb, 2) + pow(*yb, 2)))-1;
}

static inline bool in_bounds(particule p, const_env e) {
    return get_x(p) >= 0 && get_y(p) >= 0 && get_x(p) <= get_w(e) && get_y(p) <= get_h(e);
}

void l_innovation(env e, float T) {
    float r = get_r(e), dl = SPEED*get_dt(e);
    float w = get_w(e), h = get_h(e);
    int n = get_n(e);

    if (2*dl >= r) {
strategie_neuneu:
        // Dans ce cas il n'y a pas grand chose qu'on puisse faire
        // pour optimiser...
        // TODO : or IS THERE ? *Vsauce's theme starts playing*

        for (int i = 0; i < n; i++) {
            particule p_i = get_particule(e, i);
            set_position(p_i, (h>=w)*uniform(0, w), (h<w)*uniform(0, h));
            set_speed(p_i, h>=w, h<w);
        }

        return;
    }

    // 1/512 pour éviter les problèmes de précisions des flottants
    float epsilon = 1./512*min(h, w, dl);
    float a = r + epsilon; // distance inter-particules

    // Dans un cercle de rayon R, on peut placer (environ) pi*R^2 particules
    int R = ceil(sqrt(n/M_PI));

    float* positions = malloc(sizeof(float)*2*(2*R+1)*(2*R+1));
    for (int i = 0; i < 2*R+1; i++) {
        for (int j = 0; j < 2*R+1; j++) {
            int index = 2*(i*(2*R+1)+j);
            float* x = positions+index;
            float* y = positions+index+1;
            *x = (j-R)*a;
            *y = (i-R)*a-a/2;
        }
    }
    qsort(positions, (2*R+1)*(2*R+1), sizeof(float)*2, compare_norme2);

    env rev_e = create_environnement(get_n(e), get_w(e), get_h(e), get_r(e), get_dt(e));

    for (int i = 0; i < n; i++) {
        particule p_i = get_particule(rev_e, i);

        set_position(p_i, w/2+positions[2*i], h/2+positions[2*i+1]);
        set_speed(p_i, 0, positions[2*i+1] > 0 ? 1 : -1);
        if (!in_bounds(p_i, rev_e)) {
            free_environnement(rev_e);
            goto strategie_neuneu;
        }
    }

    // Il y a débat sur les off-by-ones que peuvent générer ce code
    // (ex: pour dt = 0.4 et T = 200, cette boucle itère 501 fois)
    // un code comme ```int N = ceil(T/dt); for (int i = 0; i < N; i++) ...```
    // serait sur le papier plus précis mais bon... c'est pas dans le spirit quoi..
    for (float t = 0; t < T; t += get_dt(e))
        move_particules(rev_e);
    cancel_last_collision(rev_e);

    revert_and_dup(rev_e, e);
    free_environnement(rev_e);
}
