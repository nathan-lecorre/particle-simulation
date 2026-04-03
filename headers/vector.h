#ifndef VECTOR_H
#define VECTOR_H

/* -------------------------------------------------------*/
/********** Déclarations abstraites des types *************/
/* -------------------------------------------------------*/

typedef struct vecteur* vecteur; 
typedef struct point* point; 
typedef struct droite* droite;
typedef struct plan* plan; 
typedef enum {
    RESULT_VIDE,
    RESULT_POINT, 
    RESULT_DROITE, 
    RESULT_PLAN
} TypeTest; 

/* -------------------------------------------------------*/
/********** Fonctions basiques pour les vecteurs **********/
/* -------------------------------------------------------*/

/*
@requires : rien 
@assigns : rien 
@ensures : crée un vecteur avec ses coordonnées x,y,z  
*/
vecteur create_vecteur(float x, float y, float z);

/* 
@requires : rien 
@assigns : rien 
@ensures : libère la mémoire d'un vecteur 
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
vecteur produit_vectoriel(vecteur v1, vecteur v2);

/* ---------------------------------------------------------------------------------*/
/********** Fonctions basiques pour les opérations de rotation d'éléments ***********/
/* ---------------------------------------------------------------------------------*/

/*
@requires : d est un pointeur valide pour les droites 
@assigns : rien 
@ensures : retourne d d'angle alpha dans le sens trigo 
*/

droite rotation_droite(droite d, float alpha); 

/*
@requires : P est un pointeur valide pour les plans 
@assigns : rien 
@ensures : retourne un plan d'angle alpha dans le sens trigo 
*/

plan rotation_plan(plan P, float alpha);

/* --------------------------------------------------------------------------------------*/
/********** Fonctions basiques pour les opérations d'intersection d'éléments *************/
/* --------------------------------------------------------------------------------------*/

/*
@requires : d1 et d2 sont deux pointeurs valides pour les droites 
@assigns : rien 
@ensures : retourne l'intersection de d1 et d2 de nature différente selon le résultat :
- si les deux droites sont confondues alors retourne l'une des deux droites 
- si les deux droites se croisent en un point retourne ce point 
- si les deux droites ne sont pas concourantes alors retourne null 
*/

TypeTest inter_droite(droite d1, droite d2); 

/*
@requires : P1 et P2 sont deux pointeurs valides pour les plans 
@assigns : rien 
@ensures : retourne l'intersection de P1 et P2 de nature différente selon le résultat : 
- si les deux plans sont confondus alors retourne l'un des deux plans 
- si les deux plans se croisent en une droite alors retourne ce plan 
- si les deux plans ne se croisent pas alors retourne null 
*/

TypeTest plan_inter_plan(plan P1, plan P2); 

#endif 