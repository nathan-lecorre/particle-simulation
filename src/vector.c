#include <stdio.h> 
#include <stdlib.h> 
#include <math.h> 
#include <stdbool.h> 

#include "../headers/vector.h"

/* -------------------------------------------------------*/
/********** Déclarations concrètes des types  *************/
/* -------------------------------------------------------*/

struct vecteur {
    float x; // Coordonnée x du vecteur 
    float y; // Coordonnée y du vecteur  
    float z; // Coordonnée z du vecteur  
};

struct point {
    float x; // Coordonnée absolue x du point 
    float y; // Coordonnée absolue y du point 
    float z; // Coordonnée absolue z du point 
};

struct droite {
    point p; // Point appartenant à la droite  
    vecteur v; // Vecteur directeur de la droite 
};

struct plan {
    vecteur n; // vecteur normal au plan 
    float d; // distance par rapport à un point du plan  
};

// Définition réelle de la Tagged Union
struct ResultatIntersection { 
    TypeResultat type; // 1. On stocke le TYPE (l'ID)
    union {            // 2. On stocke la DONNÉE (l'espace mémoire partagé)
        point p; 
        droite d;
        plan pl; 
    }; 
}; 

// Implémentation de l'accesseur
TypeResultat get_type(ResultatIntersection res) {return res->type;}

point get_res_point(ResultatIntersection res) {
    assert (res != NULL && res->type == RESULT_POINT);
    return res->p;
}

droite get_res_droite(ResultatIntersection res) {
    assert (res != NULL && res->type == RESULT_DROITE);
    return res->d;
}

/* -------------------------------------------------------*/
/********** Fonctions basiques pour les vecteurs **********/
/* -------------------------------------------------------*/

vecteur create_vecteur(float x, float y, float z) {
    vecteur v = malloc(sizeof(struct vecteur));
    v->x=x;
    v->y=y;
    v->z=z; 
    return v;
}

void free_vecteur(vecteur v) {
    free(v);
}

vecteur copie(vecteur v) {
    float x=v->x;
    float y=v->y;
    float z=v->z; 
    return (create_vecteur(x,y,z));
}

float vect_get_x(vecteur v) {
    return v->x;
}

float vect_get_y(vecteur v) {
    return v->y;
}

float vect_get_z(vecteur v) {
    return v->z;
}

vecteur ajout(vecteur v1, vecteur v2) {
    float xs = v1->x + v2->x;
    float ys = v1->y + v2->y;
    float zs = v1->z + v2->z;
    return (create_vecteur(xs,ys,zs));
}

void multivect(vecteur v, float lambda) {
    v->x = lambda*v->x;
    v->y = lambda*v->y;
    v->z = lambda*v->z;
}

float norme(vecteur v) {
    struct vecteur *ve = (struct vecteur *) v;
    float vx = ve->x;
    float vy = ve->y;
    float vz = ve->z;
    float norme = sqrt(vx*vx + vy*vy + vz*vz);
    return norme; 
}

float angle(vecteur v1, vecteur v2) {
    float rad = acos(produit_scalaire(v1,v2)/norme(v1)*norme(v2));
    return rad;
}

float produit_scalaire(vecteur v1, vecteur v2) {
    float pvx = v1->x*v2->x;
    float pvy = v1->y*v2->y;
    float pvz = v1->z*v2->z;
    float prod = pvx + pvy + pvz; 
    return prod; 
}

vecteur produit_vectoriel(vecteur v1, vecteur v2) {
    vecteur v = malloc(sizeof(struct vecteur));
    v->x = v1->y*v2->z-v1->z*v2->y;
    v->y = v1->z*v2->x-v1->x*v2->z;
    v->z = v1->x*v2->y-v1->y*v2->x;
    return v; 
}

/* ---------------------------------------------------------------------------------*/
/********** Fonctions basiques pour les points ***********/
/* ---------------------------------------------------------------------------------*/

point create_point(float x, float y, float z) {
    point p = malloc(sizeof(struct point));
    p->x=x;
    p->y=y;
    p->z=z; 
    return p;
}

void free_point(point p) {
    free(p);
}

float point_get_x(point p) {
    float x = p->x;
    return x;
}

float point_get_y(point p) {
    float y = p->y;
    return y;
}

float point_get_z(point p) {
    float z = p->z;
    return z;
}

/* ---------------------------------------------------------------------------------*/
/********** Fonctions basiques pour les droites ***********/
/* ---------------------------------------------------------------------------------*/

droite create_droite(point p, vecteur v) {
    droite d = malloc(sizeof(struct droite));
    d->p = p;
    d->v = v;
    return d;
}

void free_droite(droite d) {
    free(d);
}

point get_point(droite d) {return d->p;}

vecteur droite_get_vecteur(droite d) {return d->v;}

/* ---------------------------------------------------------------------------------*/
/********** Fonctions basiques pour les plans ***********/
/* ---------------------------------------------------------------------------------*/

plan create_plan(float d, vecteur n) {
    plan pl = malloc(sizeof(struct plan));
    pl->n = n;
    pl->d = d;
    return pl;
}

void free_plan(plan pl) {
    free(pl);
}

vecteur plan_get_vecteur(plan p) {return p->n;}

float get_distance(plan p) {return p->d;}

/* ---------------------------------------------------------------------------------*/
/********** Fonctions basiques pour les opérations de rotation d'éléments ***********/
/* ---------------------------------------------------------------------------------*/

// Fonction auxiliaire faisant la rotation d'un vecteur dans le sens trigo 
void rotate_vecteur(vecteur v, float d_teta, float d_phi) {
    float x = v->x;
    float y = v->y;
    float z = v->z;

    float rho = sqrt(x*x + y*y + z*z);
    float teta = acos(z/rho);
    float phi = atan(y/x);

    teta += d_teta;
    phi += d_phi;

    v->x = rho*sin(teta)*cos(phi);
    v->y = rho*sin(teta)*sin(phi);
    v->z = rho*cos(teta); 
}

void rotation_droite(droite d, float d_teta, float d_phi){
    rotate_vecteur(d->v, d_teta, d_phi);  
}

/* --------------------------------------------------------------------------------------*/
/********** Fonctions basiques pour les opérations d'intersection d'éléments *************/
/* --------------------------------------------------------------------------------------*/

// Fonction auxiliaire calculant le vecteur directeur de p1 et p2 
vecteur directeur(point p1, point p2) {
    vecteur v = malloc(sizeof(struct vecteur));
    v->x = p2->x - p1->x;
    v->y = p2->y - p1->y;
    v->z = p2->z - p1->z; 
    return v; 
}

// Fonctions auxiliaires pour inter_droite

// Fonction calculant du déterminant d'une matrice 2x2 
float det(float a1, float a2, float b1, float b2) {
    return (a1*b2 - b1*a2);
}

// Fonction résolvant la première inconnue (t1) d'un système de Cramer 2x2 : a1*t1 + b1*t2 = c1 et a2*t1 + b2*t2 = c2
float solution_epa1(float a1, float a2, float b1, float b2, float c1, float c2) {
    return (det(c1,c2,b1,b2)/det(a1,a2,b1,b2));
}

// Fonction calculant l'intersection de deux droites dans l'espace 3D et résolvant l'équation : P1 + t1*V1 = P2 + t2*V2
float solution_epa2(float a1, float a2, float b1, float b2, float c1, float c2) {
    return (det(a1,a2,c1,c2)/det(a1,a2,b1,b2));
}

ResultatIntersection inter_droite(droite d1, droite d2) {

    float vx1 = d1->v->x; float vx2 = d2->v->x;
    float vy1 = d1->v->y; float vy2 = d2->v->y;
    float vz1 = d1->v->z; float vz2 = d2->v->z;

    float x1 = d1->p->x; float x2 = d2->p->x;
    float y1 = d1->p->y; float y2 = d2->p->y; 
    float z1 = d1->p->z; float z2 = d2->p->z; 

    float a1 = vx1 ; float a2 = vy1 ; float a3 = vz1;
    float b1 = -vx2 ; float b2 = -vy2 ; float b3 = -vz2;
    float c1 = x2-x1 ; float c2 = y2-y1 ; float c3 = z2-z1;

    float t1 = 0.0;
    float t2 = 0.0;

    ResultatIntersection res = malloc(sizeof(struct ResultatIntersection));

    if (res == NULL) return NULL;

    vecteur prod_vect_v12 = produit_vectoriel(d1->v,d2->v); // produit vectoriel du vecteur directeur de d1 et du vecteur directeur de d2 
    vecteur dir12 = directeur(d1->p,d2->p); // vecteur directeur du point appartenant à d1 et du point appartenant à d2 
    vecteur prod_vect_p12 = produit_vectoriel(dir12,d2->v); // produit vectoriel du vecteur directeur calculé ci-dessus 
    
    // Cas 1 : Droites confondues (parallèles et point commun). 
    // L'intersection est donc une droite 
    if (prod_vect_v12 == 0 && prod_vect_p12 == 0) { 
        res->type = RESULT_DROITE;
        res->d = d1; 
        return res;
    }
    
    // Cas 2 : Recherche d'un point d'intersection
    // On résout le système sur 2 dimensions et on vérifie la cohérence sur la 3ème

    // Les trois clauses if correspondent à une intersection vide 

    if (det(a1,a2,b1,b2) != 0) { // Projection sur le plan XY
        t1 = solution_epa1(a1,a2,b1,b2,c1,c2);
        t2 = solution_epa2(a1,a2,b1,b2,c1,c2);
        if (fabs(a3*t1 + b3*t2 + c3) >= 10e-12) {
            return NULL;
        }
    }

    else if (det(a1,a3,b1,b3) != 0) { // Projection sur le plan XZ
        t1 = solution_epa1(a1,a2,b1,b2,c1,c2);
        t2 = solution_epa2(a1,a2,b1,b2,c1,c2);
        if (fabs(a3*t1 + b3*t2 + c3) >= 10e-12) {
            return NULL;
        }
    }

    else if (det(a2,a3,b2,b3) != 0) { // Projection sur le plan YZ 
        t1 = solution_epa1(a1,a2,b1,b2,c1,c2);
        t2 = solution_epa2(a1,a2,b1,b2,c1,c2);
        if (fabs(a3*t1 + b3*t2 + c3) >= 10e-12) {
            return NULL;
        }
    }

    // Cas 3 : Intersection trouvée donc l'intersection est un point. 
    res->type = RESULT_POINT;
    res->p = create_point(vx1*t1 + x1, vy1*t1 + y1, vz1*t1 + z1);
    return res;

}

ResultatIntersection inter_droite_plan(droite dr, plan pl) {

    float a = pl->n->x;
    float b = pl->n->y;
    float c = pl->n->z;
    float d = pl->d;

    float vx = dr->v->x;
    float vy = dr->v->y;
    float vz = dr->v->z;

    float x = dr->p->x;
    float y = dr->p->y;
    float z = dr->p->z;

    ResultatIntersection res = malloc(sizeof(struct ResultatIntersection));

    float prod = produit_scalaire(dr->v,pl->n); // produit scalaire du vecteur directeur de la droite et du vecteur normal au plan
    
    // Cas 1 : On vérifie si la droite appartient au plan, ce qui peut arriver si v et n sont orthogonaux et si au moins un point de d appartient au plan. 
    // L'intersection est donc une droite 
    if (prod == 0 && fabs(a*x + b*y + c*z + d) < 10e-12) {  
        res->type = RESULT_DROITE;
        res->d = dr; 
        return res;
    }

    // Cas 2 : Si v et n sont orthogonaux, mais qu'au moins un point de d n'appartient pas au plan, alors il n'y a pas d'intersection
    // L'intersection est donc vide 
    else if (prod == 0) {
        return NULL;
    }

    // Cas 3 : Si v et n ne sont pas orthogonaux, alors ils se croisent en un point qu'on peut déterminer avec les équations paramétriques de la droite 
    // L'intersection est donc un point 
    else {
        float t = -(a*x + b*y + c*z + d)/prod;
        res->type = RESULT_POINT;
        res-> p = create_point(vx*t + x, vy*t + y, vz*t + z);
        return res;
    }
}