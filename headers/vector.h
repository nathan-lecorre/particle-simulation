#ifndef VECTOR_H
#define VECTOR_H

#include "vector.h"

typedef void* vecteur; 

/*
@requires : rien 
@assigns : rien 
@ensures : crée un vecteur avec ses coordonnées x,y,z  
*/
void create_vecteur(float x, float y, float z);

/* 
@requires : rien 
@assigns : rien 
@ensure : 
*/
void free_vecteur(vecteur v);

/*
@requires : v est un pointeur valide pour le vecteur 
@assigns : rien 
@ensures : copie un vecteur avec ses caractéristiques 
*/
vecteur copie(vecteur v);

/*
@requires : v1 et v2 sont des pointeurs valides pour les vecteurs 
@assigns : rien 
@ensures : retourne un vecteur qui est l'addition de v1 et v2 
*/
vecteur ajout(vecteur v1, vecteur v2);

/*
@requires : p est un pointeur valide pour la particule 
@assigns : rien 
@ensures : retourne le vecteur multiplié par un scalaire (ou une norme)
*/
vecteur multivect(vecteur v, float lambda);

/* 
@requires: v est un pointeur valide pour le vecteur 
@assigns : rien 
@ensures : retourne la norme de v 
*/
float norme(vecteur v);

/*
@requires : v1 et v2 sont deux pointeurs valides pour les vecteurs 
@assigns : rien
@ensures : retourne l'angle entre v1 et v2 
*/
float angle(vecteur v1, vecteur v2);

/*
@requires : v1 et v2 sont deux pointeurs valides pour les vecteurs 
@assigns : rien 
@ensures : retourne le produit scalaire entre v1 et v2 
*/
float produit_scalaire(vecteur v1, vecteur v2);

/*
@requires : v1 et v2 sont deux pointeurs valides pour les vecteurs 
@assigns : rien 
@ensures : retourne le vecteur résultant du produit vectorielle de v1 et v2 
*/
vecteur produit_vectorielle(vecteur v1, vecteur v2);

#endif 