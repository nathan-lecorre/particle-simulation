#ifndef _CAMERA_H
#define _CAMERA_H

#include "vector.h"


typedef struct Camera {
    point p; // Position de la caméra
    vecteur  c ; // Vecteur de visée unitaire 
    int w ; // Largeur de l'écran
    int h; // Longueur de l'écran   
} camera;

/*  @requires: cam et v sont des pointeurs valides
    @assigns: *cam est modifié
    @ensures: translate la position p de la caméra de v, ne change pas son vecteur de visée */
void translation(camera* cam, vecteur v);


#endif 