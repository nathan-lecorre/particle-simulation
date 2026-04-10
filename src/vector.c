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

vecteur ajout(vecteur v1, vecteur v2) {
    float xs = v1->x + v2->x;
    float ys = v1->y + v2->y;
    float zs = v1->z + v2->z;
    return (create_vecteur(xs,ys,zs));
}

void multivect(vecteur v, float lambda) {
    v->x += lambda*v->x;
    v->y += lambda*v->y;
    v->z += lambda*v->z;
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

point get_point(droite d) {return d->p;}

vecteur droite_get_vecteur(droite d) {return d->v;}

/* ---------------------------------------------------------------------------------*/
/********** Fonctions basiques pour les plans ***********/
/* ---------------------------------------------------------------------------------*/

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

// Fonction auxiliaire pour résoudre une ligne d'une équation paramétrique de deux lignes 
float solution_epa(float a1, float b1, float a2, float b2) {return ((b2-b1)/(a1-a2));}

ResultatIntersection inter_droite(droite d1, droite d2) {

    float vx1 = d1->v->x; float vx2 = d2->v->x;
    float vy1 = d1->v->y; float vy2 = d2->v->y;
    float vz1 = d1->v->z; float vz2 = d2->v->z;

    float x1 = d1->p->x; float x2 = d2->p->x;
    float y1 = d1->p->y; float y2 = d2->p->y; 
    float z1 = d1->p->z; float z2 = d2->p->z; 

    float t =0.0;
    bool found = false; 

    ResultatIntersection res = malloc(sizeof(struct ResultatIntersection));

    if (res == NULL) return NULL;

    // Cas 1 : d1 et d2 sont confondues 
    vecteur prod_vect_v12 = produit_vectoriel(d1->v,d2->v); // produit vectoriel du vecteur directeur de d1 et du vecteur directeur de d2 
    vecteur dir12 = directeur(d1->p,d2->p); // vecteur directeur du point appartenant à d1 et du point appartenant à d2 
    vecteur prod_vect_p12 = produit_vectoriel(dir12,d2->v); // produit vectoriel du vecteur directeur calculé ci-dessus 
    
    if (prod_vect_v12 == 0 && prod_vect_p12 == 0) { 
        res->type = RESULT_DROITE;
        res->d = *d1; 
        return res;
    }
    
    // Cas autre : 

    if (vx1 == vx2 && x1 != x2) {return NULL;}
    else if (vx1 != vx2) {t = solution(vx1,vx2,x1,x2);}

    if (vx1 == vx2 && y1 != y2) {return NULL;}
    else if (vy1 != vy2) {
        float tmp_sol = solution(vy1,vy2,y1,y2);
        if (!found) {t = tmp_sol;}
        else if (fabs(t - tmp_sol > 0.001)) {return NULL;}
    }

    if (vz1 == vz2 && z1 != z2) {return NULL;}
        float tmp_sol = solution(vz1,vz2,z1,z2);
        if (!found) {t = tmp_sol;}
        else if (fabs(t - tmp_sol > 0.001)) {return NULL;}

    res->type = RESULT_POINT; 
    return res;

}
