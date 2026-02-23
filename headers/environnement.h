#ifndef ENVIRONNEMENT_H
#define ENVIRONNEMENT_H

#include "particules.h"

/// type abstrait représentant un environnement, ses paramètres,
/// et les particules qui le composent.
typedef void* env;
typedef const void* const_env;

/*
@requires : n, w, h, r et dt sont positifs. 
@assigns : rien 
@ensures : crée un environnement de taille w*h et de durée des itérations de déplacement dt,
avec n particules initialement au centre de l'environnement,
de vitesse vers la droite, de rayon d'intéraction r.
*/
env create_environnement(int n, float w, float h, float r, float dt);

/*
@requires : e a été créé avec create_environnement 
@assigns : e 
@ensures : libère e
*/
void free_environnement(env e);

/*
@requires : e a été créé avec create_environnement
@assigns : rien 
@ensures : renvoie le nombre de particules dans e
*/
int get_n(const_env e);

/*
@requires : e a été créé avec create_environnement, il y a au moins i+1 particules dans e 
@assigns : rien 
@ensures : renvoie la i-ème particule de e
*/
particule get_particule(const_env e, int i);

/*
@requires : e a été créé avec create_environnement
@assigns : rien 
@ensures : renvoie la largeur de l'environnement
*/
float get_w(const_env e);

/*
@requires : e a été créé avec create_environnement
@assigns : rien 
@ensures : renvoie la hauteur de l'environnement
*/
float get_h(const_env e);

/*
@requires : e a été créé avec create_environnement
@assigns : rien 
@ensures : renvoie la durée des itérations des particules dans l'environnement e
*/
float get_dt(const_env e);

/*
@requires : e a été créé avec create_environnement
@assigns : rien 
@ensures : renvoie le rayon d'intéraction des particules dans l'environnement e
*/
float get_r(const_env e);

/*
@requires : e a été créé avec create_environnement
@assigns : e 
@ensures : effectue une itération de déplacement dans e
*/
void move_particules(env e);

#endif
