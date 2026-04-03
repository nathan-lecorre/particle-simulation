#include <stdio.h> 
#include <stdlib.h> 
#include <math.h> 

#include "vector.h"

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
    create_vecteur(x,y,z);
}

vecteur ajout(vecteur v1, vecteur v2) {
    float xs = v1->x + v2->x;
    float ys = v1->y + v2->y;
    float zs = v1->z + v2->z;
    return (create_vecteur(xs,ys,zs));
}

vecteur multivect(vecteur v, float lambda) {
    v->x=lambda*v->x;
    v->y=lambda*v->y;
    v->z=lambda*v->z;
    return v; 
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
    float rad = arccos(produit_scalaire(v1,v2)/norme(v1)*norme(v2));
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
/********** Fonctions basiques pour les opérations de rotation d'éléments ***********/
/* ---------------------------------------------------------------------------------*/




/* -------------------------------------------------------*/
/********** Fonctions basiques pour les plans *************/
/* -------------------------------------------------------*/
