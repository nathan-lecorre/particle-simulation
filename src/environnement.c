#include "../headers/environnement.h"
#include "../headers/particules.h"
#include "../headers/distribution.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

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

static inline bool distance_négligeable(float d, s_env* e) {
    // SPEED*e->dt, e->w et e->h sont les distances caractéristiques de l'environnement
    // on peut considérer toute distance petite devant ces distances caractéristiques nulle
    return petit_devant(d, SPEED*e->dt) && petit_devant(d, e->w) && petit_devant(d, e->h);
}

static void corrected_move(particule, float, s_env*);

// fait rebondir une particule sur le bord du haut
// si et seulement si celle-ci a rencontré le bord du haut en premier dans son mouvement
static void bounce_on_top(particule p, s_env* e) {
    float x = get_x(p), y = get_y(p);
    float vx = get_vx(p), vy = get_vy(p);
    // si vy = 0, alors la particule n'a pas pu dépasser la bordure du haut
    assert(vy != 0);

    // distances et temps que la particule a parcouru de trop ( y = h + dy )
    float dy = y - e->h;
    float dx = dy*vx/vy;
    float dt = sqrt(dx*dx+dy*dy) / SPEED;

    // traitement de edge-cases qui ne devraient probablement pas arriver
    if (distance_négligeable(x-dx - 0, e))
        dx = x; // de sorte que x-dx = 0
    if (distance_négligeable(x-dx - e->w, e))
        dx = x - e->w; // de sorte que x-dx = e->w

    if (x-dx < 0 || x-dx > e->w) {
        // la particule n'a pas rencontré le bord du haut en premier
        return;
    }

    // la particule revient sur le bord du haut
    set_position(p, x-dx, y-dy);
    // sa vitesse verticale s'oppose
    set_speed(p, vx, -vy);
    // et la particule finit le mouvement qui lui reste a parcourir
    corrected_move(p, dt, e);
}

// move la particule et la fait rebondir au besoin
// plusieurs rebonds sont possibles dans la même itération,
// d'où les fonctions mutuellement récursives
static void corrected_move(particule p, float dt, s_env* e) {
    move(p, dt);
    float x = get_x(p), y = get_y(p);
    float vx = get_vx(p), vy = get_vy(p);
    float w = e->w, h = e->h;

    if (y > h) {
        bounce_on_top(p, e);
    }
    // pour les cas suivants, on tourne l'environnement pour toujours
    // se retrouver dans le cas y > h, on effectue le (ou les) rebond(s),
    // puis on retourne l'environnement
    if (y < 0) {
        set_position(p, x, h-y);
        set_speed(p, vx, -vy);

        bounce_on_top(p, e);

        set_position(p, get_x(p), h-get_y(p));
        set_speed(p, get_vx(p), -get_vy(p));
    }
    if (x > w) {
        set_position(p, y, x);
        set_speed(p, vy, vx);
        e->w = h;
        e->h = w;

        bounce_on_top(p, e);

        set_position(p, get_y(p), get_x(p));
        set_speed(p, get_vy(p), get_vx(p));
        e->h = h;
        e->w = w;
    }
    if (x < 0) {
        set_position(p, y, w-x);
        set_speed(p, vy, -vx);
        e->w = h;
        e->h = w;

        bounce_on_top(p, e);

        set_position(p, w-get_y(p), get_x(p));
        set_speed(p, -get_vy(p), get_vx(p));
        e->h = h;
        e->w = w;
    }
}

static inline bool in_bounds(particule p, const s_env* e) {
    return get_x(p) >= 0 && get_y(p) >= 0 && get_x(p) <= e->w && get_y(p) <= e->h;
}

void move_particules(env e_voidptr) {
    // vérification (partielle) de la précondition
    assert(e_voidptr);

    s_env* e = (s_env*)e_voidptr;

    for (int i = 0; i < e->n; i++) {
        corrected_move(e->particules[i], e->dt, e);
        assert(in_bounds(e->particules[i], e));
    }

    for (int i = 0; i < e->n; i++) {
        float barycentre_x = 0, barycentre_y = 0;
        int nb_voisins = 0;
        for (int j = 0; j < e->n; j++) {
            // on ne veut pas que la particule interragisse avec elle même
            if (j == i) continue; 

            if (distance(e->particules[i], e->particules[j]) < e->r) {
                barycentre_x += get_x(e->particules[j]);
                barycentre_y += get_y(e->particules[j]);
                nb_voisins++;
            }
        }
        if (nb_voisins != 0) {
            barycentre_x /= nb_voisins;
            barycentre_y /= nb_voisins;

            // vecteur (barycentre des voisins)->(particule)
            float bary_part_x = get_x(e->particules[i]) - barycentre_x;
            float bary_part_y = get_y(e->particules[i]) - barycentre_y;
            float norme = sqrt(bary_part_x*bary_part_x + bary_part_y*bary_part_y);

            if (distance_négligeable(norme, e)) {
                // le barycentre est sur la particule

                float teta = uniform(0.0f, 2*M_PI); // angle du vecteur de norme SPEED
                set_speed(e->particules[i], SPEED*cos(teta), SPEED*sin(teta));
            } else {
                // vecteur de norme SPEED qui s'éloigne du barycentre
                set_speed(e->particules[i], SPEED*bary_part_x/norme, SPEED*bary_part_y/norme);
            }
        }
    }
}
