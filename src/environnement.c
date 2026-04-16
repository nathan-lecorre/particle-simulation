#include "../headers/environnement.h"
#include "../headers/particules.h"
#include "../headers/distribution.h"
#include "../headers/vector.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#define SPEED 1.0f // pour être un peut plus future-proof

#define MU 1.8e-5

#define CONST_GRAV 6.67e-11

#ifndef M_PI
#define M_PI atan(1)*4
#endif

struct perturbateur {
    point position;
    int nature; // 1 pour répulseur, -1 pour attracteur
};

struct perturbateurs_list {
    perturbateur obj;
    perturbateurs_list next;
};

typedef struct {
    float w,h,d;
    float r;
    float dt;
    int n;
    particule* particules;
    perturbateurs_list objets;
} s_env;

env create_environnement(int n, float w, float h, float d, float r, float dt, float mass) {
    // vérification des préconditions
    assert(n >= 0 && w >= 0 && h >= 0 && r >= 0 && d>=0 && dt >= 0);

    // allocation et initialisation de l'environnement
    s_env* e = malloc(sizeof(s_env)); // pas trop de risque d'echec du malloc
    e->w = w;
    e->h = h;
    e->d = d; 
    e->r = r;
    e->dt = dt;
    e->n = n;
    e->objets = NULL;

    // allocation et initialisation des particules
    e->particules = malloc(n * sizeof(particule));
    if (!e->particules) {
        // dommage, on m'interdit d'utiliser les flux standards
        // fprintf(stderr, "[create_environnement] Can't malloc %d particles\n", n);
        exit(1);
    }
    for (int i = 0; i < n; i++) {
        e->particules[i] = create_particule(w/2, h/2, d/2, SPEED, 0, 0, mass);
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

float get_d(const_env e_voidptr) {
    // vérification (partielle) de la précondition
    assert(e_voidptr);

    const s_env* e = (const s_env*)e_voidptr;

    return e->d;
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

static inline bool in_bounds_point(point p, const s_env* e) {
    return point_get_x(p) >= 0 && point_get_y(p) >= 0 && point_get_z(p) >= 0 && point_get_x(p) <= e->w && point_get_y(p) <= e->h && point_get_z(p) <= e->d;
}

void add_objet(env environnement_ptr, point p, int nature) {
    
    assert (abs(nature) == 1);

    s_env* environnement = (s_env*) environnement_ptr;

    assert (in_bounds_point(p,environnement));

    perturbateur obj = malloc(sizeof(struct perturbateur));
    obj->position = p;
    obj->nature = nature;

    perturbateurs_list obj_list = malloc(sizeof(struct perturbateurs_list));
    obj_list->obj = obj;
    obj_list->next = environnement->objets;

    environnement->objets = obj_list;
}

static inline bool petit_devant(float d, float D) {
    return fabs(d) < 0.001f*fabs(D);
}

static inline bool distance_négligeable(float d, s_env* e) {
    // SPEED*e->dt, e->w et e->h sont les distances caractéristiques de l'environnement
    // on peut considérer toute distance petite devant ces distances caractéristiques nulle
    return petit_devant(d, SPEED*e->dt) && petit_devant(d, e->w) && petit_devant(d, e->h) && petit_devant(d, e->d);
}

static void corrected_move(particule, float, s_env*);

// fait rebondir une particule sur le bord du haut
// si et seulement si celle-ci a rencontré le bord du haut en premier dans son mouvement
static void bounce_on_top(particule p, s_env* e) {
    float x = get_x(p), y = get_y(p), z = get_z(p);
    float vx = get_vx(p), vy = get_vy(p), vz = get_vz(p);
    // si vy = 0, alors la particule n'a pas pu dépasser la bordure du haut
    assert(vy != 0);

    // distances et temps que la particule a parcouru de trop ( y = h + dy )
    float dy = y - e->h;
    float dz = dy*vz/vy; 
    float dx = dy*vx/vy;
    float dt = sqrt(dx*dx+dy*dy+dz*dz) / sqrt(vx*vx+vy*vy+vz*vz);

    // traitement de edge-cases qui ne devraient probablement pas arriver
    if (distance_négligeable(x-dx - 0, e))
        dx = x; // de sorte que x-dx = 0
    if (distance_négligeable(x-dx - e->w, e))
        dx = x - e->w; // de sorte que x-dx = e->w

    if (x-dx < 0 || x-dx > e->w) {
        // la particule n'a pas rencontré le bord du haut en premier
        return;
    }

    if (distance_négligeable(z-dz - 0, e))
        dz = z; // de sorte que z-dz = 0
    if (distance_négligeable(z-dz - e->d, e))
        dz = z - e->d; // de sorte que z-dz = e->d

    if (z-dz < 0 || z-dz > e->d) {
        // la particule n'a pas rencontré le bord du haut en premier
        return;
    }

    // la particule revient sur le bord du haut
    set_position(p, x-dx, y-dy, z-dz);
    // sa vitesse verticale s'oppose
    set_speed(p, vx, -vy, vz);
    // et la particule finit le mouvement qui lui reste a parcourir
    corrected_move(p, dt, e);
}

// move la particule et la fait rebondir au besoin
// plusieurs rebonds sont possibles dans la même itération,
// d'où les fonctions mutuellement récursives
static void corrected_move(particule p, float dt, s_env* e) {
    move(p, dt);
    float w = e->w, h = e->h, d = e->d; 

    if (get_y(p) > h) {
        bounce_on_top(p, e);
    }
    // pour les cas suivants, on tourne l'environnement pour toujours
    // se retrouver dans le cas y > h, on effectue le (ou les) rebond(s),
    // puis on retourne l'environnement
    if (get_y(p) < 0) {
        set_position(p, get_x(p), h-get_y(p), get_z(p));
        set_speed(p, get_vx(p), -get_vy(p), get_vz(p));

        bounce_on_top(p, e);

        set_position(p, get_x(p), h-get_y(p), get_z(p));
        set_speed(p, get_vx(p), -get_vy(p), get_vz(p));
    }
    if (get_x(p) > w) {
        set_position(p, get_y(p), get_x(p), get_z(p));
        set_speed(p, -get_vy(p), get_vx(p), get_vz(p));
        e->w = h;
        e->h = w;
        e->d = d; 

        bounce_on_top(p, e);

        set_position(p, get_y(p), get_x(p), get_z(p));
        set_speed(p, get_vy(p), -get_vx(p), get_vz(p));
        e->h = h;
        e->w = w;
        e->d = d; 
    }
    if (get_x(p) < 0) {
        set_position(p, get_y(p), w-get_x(p), get_z(p));
        set_speed(p, get_vy(p), -get_vx(p), get_vz(p));
        e->w = h;
        e->h = w;
        e->d = d; 

        bounce_on_top(p, e);

        set_position(p, w-get_y(p), get_x(p), get_z(p));
        set_speed(p, -get_vy(p), get_vx(p), get_vz(p));
        e->h = h;
        e->w = w;
        e->d = d; 
    }

      if (get_z(p) > d) {
        set_position(p, get_x(p), get_z(p), get_y(p));
        set_speed(p, get_vx(p), get_vz(p), -get_vy(p));
        e->w = h;
        e->h = w;
        e->d = d; 

        bounce_on_top(p, e);

        set_position(p, get_x(p), get_z(p), get_y(p));
        set_speed(p, get_vx(p), -get_vz(p), get_vy(p));
        e->h = h;
        e->w = w;
        e->d = d; 
    }
    if (get_z(p) < 0) {
        set_position(p, get_x(p), d - get_z(p), get_y(p));
        set_speed(p, get_vx(p), -get_vz(p), get_vy(p));
        e->w = h;
        e->h = w;
        e->d = d; 

        bounce_on_top(p, e);

        set_position(p, get_x(p), d - get_z(p), get_y(p));
        set_speed(p, get_vx(p), get_vz(p), -get_vy(p));
        e->h = h;
        e->w = w;
        e->d = d; 
    }
}

vecteur frottements_fluides(particule p) {
    float coeff = -6*M_PI*MU*get_radius(p);
    vecteur res = create_vecteur(get_vx(p),get_vy(p),get_vz(p));
    multivect(res,coeff);
    return res;
}

vecteur force_attraction(particule p1, particule p2, s_env* environnement) {

    float dist = distance(p1,p2);
    vecteur res;

    // Si les deux particules sont suffisamment éloignées, on y ajoute une force de gravitation

    if (!distance_négligeable(dist,environnement)) {
        float coeff = CONST_GRAV * (get_mass(p1) * get_mass(p2)) / pow(dist,3);
        res = create_vecteur(get_x(p1) - get_x(p2), get_y(p1) - get_y(p2), get_z(p1) - get_z(p2));
        multivect(res,coeff);
    }

    // Sinon, les particules sont pratiquement au même endroit, on applique donc une force éloignant la particule dans une 
    // direction aléatoire en conservant la vitesse
    else {
        float norme = sqrt(get_vx(p1)*get_vx(p1) + get_vy(p1)*get_vy(p1) + get_vz(p1)*get_vz(p1));
        float teta = uniform(0.0f, 2*M_PI);
        float phi = uniform(0.0f, 2*M_PI);

        res = create_vecteur(norme*get_mass(p1)*cos(phi)*sin(teta)/environnement->dt, 
                            norme*get_mass(p1)*sin(teta)*sin(phi)/environnement->dt, 
                            norme*get_mass(p1)*cos(teta)/environnement->dt);
    }
    return res;
}

vecteur force_perturbateurs(particule p, s_env* environnement) {

    vecteur res = create_vecteur(0.0,0.0,0.0);

    perturbateurs_list objets = environnement->objets;

    while (objets != NULL) {
        perturbateur obj = objets->obj;
        point pos_obj = obj->position;

        particule tmp = create_particule(point_get_y(pos_obj),point_get_y(pos_obj),point_get_y(pos_obj),0,0,0,0);
        vecteur force_sur_obj = force_attraction(p,tmp,environnement);
        multivect(force_sur_obj,obj->nature);
        
        vecteur res_tmp = ajout(res,force_sur_obj);
        free_vecteur(res);
        free_vecteur(force_sur_obj);
        free_particule(tmp);
        res = res_tmp;
        
        objets = objets->next;
    }

    return res;
}

vecteur forces_totales(int i, s_env* environnement) {

    particule p = environnement->particules[i];

    vecteur frottements = frottements_fluides(p);
    vecteur perturbations = force_perturbateurs(p,environnement);
    vecteur attraction = create_vecteur(0,0,0);

    for (int j = 0; j < environnement->n; j++) {
        // on ne veut pas que la particule interragisse avec elle même
        if (j == i) continue; 

        particule pj = environnement->particules[j];

        if (distance(p, pj) < environnement->r) {
            vecteur force = force_attraction(p,pj,environnement);
            vecteur tmp = ajout(force,attraction);
            free_vecteur(attraction);
            free_vecteur(force);
            attraction = tmp;
            
        }
    }

    vecteur add1 = ajout(frottements,perturbations);
    vecteur res = ajout(add1,attraction);
    free_vecteur(attraction);
    free_vecteur(perturbations);
    free_vecteur(frottements);
    free_vecteur(add1);

    return res;
}

vecteur get_acceleration(int i, s_env* environnement) {

    vecteur res = forces_totales(i,environnement);

    particule p = environnement->particules[i];
    float mass = get_mass(p);

    multivect(res,1/mass);

    return res;
}

static inline bool in_bounds(particule p, const s_env* e) {
    return get_x(p) >= 0 && get_y(p) >= 0 && get_z(p) >= 0 && get_x(p) <= e->w && get_y(p) <= e->h && get_z(p) <= e->d;
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
        vecteur new_v = get_acceleration(i, e);
        multivect(new_v,e->dt);

        particule p = get_particule(e_voidptr,i);
        set_speed(p,vect_get_x(new_v) + get_vx(p),vect_get_y(new_v) + get_vy(p),vect_get_z(new_v) + get_vz(p));

        free_vecteur(new_v);
    }
}
