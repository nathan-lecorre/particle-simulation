#include "../headers/environnement.h"
#include "../headers/particules.h"
#include "../headers/distribution.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

#define SPEED 1.0f // pour être un peut plus future-proof

#ifndef M_PI
#define M_PI atan(1)*4
#endif

typedef struct {
    float w, h;
    float r;
    float dt;
    int n;
    particule* particules;
} s_env;

env create_environnement(int n, float w, float h, float r, float dt) {
    // vérification des préconditions
    assert(n >= 0 && w >= 0 && h >= 0 && r >= 0 && dt >= 0);

    // allocation et initialisation de l'environnement
    s_env* e = malloc(sizeof(s_env)); // pas trop de risque d'echec du malloc
    e->w = w;
    e->h = h;
    e->r = r;
    e->dt = dt;
    e->n = n;

    // allocation et initialisation des particules
    e->particules = malloc(n * sizeof(particule));
    if (!e->particules) {
        // dommage, on m'interdit d'utiliser les flux standards
        // fprintf(stderr, "[create_environnement] Can't malloc %d particles\n", n);
        exit(1);
    }
    for (int i = 0; i < n; i++) {
        e->particules[i] = create_particule(w/2, h/2, SPEED, 0);
    }

    return (env)e;
}

void free_environnement(env e_voidptr) {
    // vérification (partielle) de la précondition
    assert(e_voidptr);

    s_env* e = (s_env*)e_voidptr;

    // déallocation des particules et du tableau de particules
    for (int i = 0; i < e->n; i++) {
        free_particule(e->particules[i]);
    }
    free(e->particules);

    // déallocation de l'environnement
    free(e);
}

int get_n(const_env e_voidptr) {
    // vérification (partielle) de la précondition
    assert(e_voidptr);

    const s_env* e = (const s_env*)e_voidptr;

    return e->n;
}

particule get_particule(const_env e_voidptr, int i) {
    const s_env* e = (const s_env*)e_voidptr;

    // vérification (partielle) des préconditions
    assert(e && e->n >= i+1);

    return e->particules[i];
}
    
float get_w(const_env e_voidptr) {
    // vérification (partielle) de la précondition
    assert(e_voidptr);

    const s_env* e = (const s_env*)e_voidptr;

    return e->w;
}


float get_h(const_env e_voidptr) {
    // vérification (partielle) de la précondition
    assert(e_voidptr);

    const s_env* e = (const s_env*)e_voidptr;

    return e->h;
}

float get_dt(const_env e_voidptr) {
    // vérification (partielle) de la précondition
    assert(e_voidptr);

    const s_env* e = (const s_env*)e_voidptr;

    return e->dt;
}

float get_r(const_env e_voidptr) {
    // vérification (partielle) de la précondition
    assert(e_voidptr);

    const s_env* e = (const s_env*)e_voidptr;

    return e->r;
}

static inline bool petit_devant(float d, float D) {
    return fabs(d) < 0.001f*fabs(D);
}

static inline bool distance_négligeable(float d, const s_env* e) {
    // SPEED*e->dt, e->w et e->h sont les distances caractéristiques de l'environnement
    // on peut considérer toute distance petite devant ces distances caractéristiques nulle
    return petit_devant(d, SPEED*e->dt) && petit_devant(d, e->w) && petit_devant(d, e->h);
}

static inline bool in_bounds(particule p, const s_env* e) {
    return get_x(p) >= 0 && get_y(p) >= 0 && get_x(p) <= e->w && get_y(p) <= e->h;
}

static bool was_on_edge(particule p, const s_env* e, float dt) {
    float x = get_x(p) - get_vx(p)*dt;
    float y = get_y(p) - get_vy(p)*dt;
    if (distance_négligeable(x, e)) x = 0;
    if (distance_négligeable(y, e)) y = 0;
    if (distance_négligeable(x-e->w, e)) x = e->w;
    if (distance_négligeable(y-e->h, e)) y = e->h;
    return (x >= 0 && x <= e->w && y >= 0 && y <= e->h)
        && (x == 0 || y == 0 || x == e->w || y == e->h);
}

// move la particule et la fait rebondir au besoin
// plusieurs rebonds sont possibles dans la même itération,
static void corrected_move(particule p, float dt, const s_env* e) {
    move(p, dt);
    float w = e->w, h = e->h; 

    while (!in_bounds(p, e)) {

        if (get_y(p) > h && was_on_edge(p, e, (get_y(p)-h)/get_vy(p))) {
            set_position(p, get_x(p), 2*h - get_y(p));
            set_speed(p, get_vx(p), -get_vy(p));
        }

        if (get_y(p) < 0 && was_on_edge(p, e, get_y(p)/get_vy(p))) {
            set_position(p, get_x(p), -get_y(p));
            set_speed(p, get_vx(p), -get_vy(p));
        }

        if (get_x(p) > w && was_on_edge(p, e, (get_x(p)-w)/get_vx(p))) {
            set_position(p, 2*w - get_x(p), get_y(p));
            set_speed(p, -get_vx(p), get_vy(p));
        }

        if (get_x(p) < 0 && was_on_edge(p, e, get_x(p)/get_vx(p))) {
            set_position(p, -get_x(p), get_y(p));
            set_speed(p, -get_vx(p), get_vy(p));
        }
    }
}

static int compare_x(const void* a, const void* b) {
    particule p_a = *((const particule*)a);
    particule p_b = *((const particule*)b);
    return 2*(get_x(p_a) >= get_x(p_b))-1;
}

void move_particules(env e_voidptr) {
    // vérification (partielle) de la précondition
    assert(e_voidptr);

    s_env* e = (s_env*)e_voidptr;

    for (int i = 0; i < e->n; i++) {
        corrected_move(e->particules[i], e->dt, e);
        assert(in_bounds(e->particules[i], e));
    }

    // Pour les collisions, on ne va regarder que dans un abscisse relatif
    // de [-r, +r] pour optimiser un peu
    particule* sorted_parts = malloc(e->n*sizeof(particule));
    memcpy(sorted_parts, e->particules, e->n*sizeof(particule));
    qsort(sorted_parts, e->n, sizeof(particule), compare_x);

    particule* min_p = sorted_parts;

    for (int i = 0; i < e->n; i++) {
        float barycentre_x = 0, barycentre_y = 0;
        int nb_voisins = 0;
        particule p_i = sorted_parts[i];

        while(get_x(p_i)-get_x(*min_p) > e->r)
            min_p++;

        for (int j = min_p-sorted_parts; j < e->n; j++) {
            // on ne veut pas que la particule interragisse avec elle même
            if (j == i) continue; 
            particule p_j = sorted_parts[j];

            if (get_x(p_j)-get_x(p_i) > e->r) break;

            if (distance(p_i, p_j) < e->r) {
                barycentre_x += get_x(p_j);
                barycentre_y += get_y(p_j);
                nb_voisins++;
            }
        }
        if (nb_voisins != 0) {
            barycentre_x /= nb_voisins;
            barycentre_y /= nb_voisins;

            // vecteur (barycentre des voisins)->(particule)
            float bary_part_x = get_x(p_i) - barycentre_x;
            float bary_part_y = get_y(p_i) - barycentre_y;
            float norme = sqrt(bary_part_x*bary_part_x + bary_part_y*bary_part_y);

            if (norme == 0.0f) {
                // le barycentre est sur la particule

                float teta = uniform(0.0f, 2*M_PI); // angle du vecteur de norme SPEED
                set_speed(p_i, SPEED*cos(teta), SPEED*sin(teta));
            } else {
                // vecteur de norme SPEED qui s'éloigne du barycentre
                set_speed(p_i, SPEED*bary_part_x/norme, SPEED*bary_part_y/norme);
            }
        }
    }
}

