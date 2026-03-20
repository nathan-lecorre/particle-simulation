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
};

particule create_particule(float x, float y, float z, float vx, float vy, float vz) {
    struct particule *p = malloc(sizeof(struct particule));
    p->x=x;
    p->y=y;
    p->z=z;
    float normev=sqrt(vx*vx+vy*vy+vz*vz); // Norme de la vitesse v de la particule de composantes vx et vy
    p->vx=vx/(normev*normev); // Normalisation de vx
    p->vy=vy/(normev*normev); // Normalisation de vy 
    p->vz=vz/(normev*normev); // Normalisation de vz 
    return (particule) p;  
}

void free_particule(particule p) {
    free((struct particule *) p);
}

float get_x(particule p) {
    struct particule *part= (struct particule *) p;
    float x=part->x;
    return x; 
}

float get_y(particule p) {
    struct particule *part= (struct particule *) p;
    float y=part->y;
    return y;
}

float get_z(particule p) {
    struct particule *part= (struct particule *) p;
    float z=part->z;
    return z; 
}

float get_vx(particule p) {
    struct particule *part= (struct particule *) p;
    float vx=part->vx;
    return vx;
}

float get_vy(particule p) {
    struct particule *part= (struct particule *) p;
    float vy=part->vy;
    return vy;
}

float get_vz(particule p) {
    struct particule *part= (struct particule *) p;
    float vz=part->vz;
    return vz; 
}
void set_position(particule p, float x, float y, float z) {
    struct particule *part= (struct particule *) p;
    part->x=x;
    part->y=y;
    part->z=z;
}

void set_speed(particule p, float vx, float vy, float vz) {
    struct particule *part= (struct particule *) p;
    float normev=sqrt(vx*vx+vy*vy+vz*vz);
    part->vx=vx/(normev*normev); // Normalisation de vx
    part->vy=vy/(normev*normev); // Normalisation de vy 
    part->vz=vz/(normev*normev); // Normalisation de vz 
}

void move(particule p, float dt) {
    struct particule *part= (struct particule *) p;
    part->x=(part->vx)/dt; // Calcul de la position x de p après un temps infinitésimal dt
    part->y=(part->vy)/dt;  // Calcul de la position y de p après un temps infinitésimal dt  
    part->z=(part->vz)/dt; 
}

float distance(particule p1, particule p2) {
    struct particule *part1= (struct particule *) p1;
    struct particule *part2= (struct particule *) p2; 
    float distx2=fabsf((part1->x)-(part2->x)); // Calcul de la différence en valeur absolue de la position x entre p1 et p2
    float disty2=fabsf((part1->y)-(part2->y)); // Calcul de la différence en valeur absolue de la position y entre p1 et p2 
    float distz2=fabsf((part1->z)-(part2->z)); // Calcul de la différence en valeur absolue de la position z entre p1 et p2 
    float dist=sqrt(distx2+disty2+distz2); // Calcul de la distance 
    return dist;
}
