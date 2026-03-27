#include <stdio.h> 
#include <stdlib.h> 
#include <math.h> 

#include "../headers/particules.h"

struct particule {
    float x; // Coordonnée x de la particule 
    float y; // Coordonnée y de la particule 
    float z; // Coordonnée z de la particule 
    float vx; // Composante vx de la vitesse de la particule
    float vy; // Composante vy de la vitesse de la particule
    float vz; // Composante vz de la vitesse de la particule 
    float mass; // Masse de la particule
};

particule create_particule(float x, float y, float z, float vx, float vy, float vz) {
    particule p = malloc(sizeof(struct particule));
    p->x=x;
    p->y=y;
    p->z=z;
    p->vx=vx;
    p->vy=vy;
    p->vz=vz;
    return p;  
}

void free_particule(particule p) {
    free(p);
}

float get_x(particule p) {
    float x=p->x;
    return x; 
}

float get_y(particule p) {;
    float y=p->y;
    return y;
}

float get_z(particule p) {
    float z=p->z;
    return z; 
}

float get_vx(particule p) {
    float vx=p->vx;
    return vx;
}

float get_vy(particule p) {
    float vy=p->vy;
    return vy;
}

float get_vz(particule p) {
    float vz=p->vz;
    return vz; 
}

float get_mass(particule p) {
    return p->mass;
}

void set_position(particule p, float x, float y, float z) {
    p->x=x;
    p->y=y;
    p->z=z;
}

void set_speed(particule p, float vx, float vy, float vz) {
    float normev=sqrt(vx*vx+vy*vy+vz*vz);
    p->vx=vx;
    p->vy=vy;
    p->vz=vz;
}

void move(particule p, float dt) {
    p->x += (p->vx)/dt; // Calcul de la position x de p après un temps infinitésimal dt
    p->y += (p->vy)/dt; // Calcul de la position y de p après un temps infinitésimal dt  
    p->z += (p->vz)/dt; // Calcul de la position z de p après un temps infinitésimal dt
}

float distance(particule p1, particule p2) {
    float distx2=fabsf((p1->x)-(p2->x)); // Calcul de la différence en valeur absolue de la position x entre p1 et p2
    float disty2=fabsf((p1->y)-(p2->y)); // Calcul de la différence en valeur absolue de la position y entre p1 et p2 
    float distz2=fabsf((p1->z)-(p2->z)); // Calcul de la différence en valeur absolue de la position z entre p1 et p2 
    float dist=sqrt(distx2+disty2+distz2); // Calcul de la distance 
    return dist;
}
