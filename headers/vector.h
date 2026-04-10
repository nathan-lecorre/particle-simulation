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
} TypeResultat;
typedef struct ResultatIntersection* ResultatIntersection;
 

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
@assigns : v 
@ensures : multiplie v par lambda 
*/
void multivect(vecteur v, float lambda);

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
/********** Fonctions basiques pour les points ***********/
/* ---------------------------------------------------------------------------------*/

/*
@requires : p est un pointeur valide pour le point
@assigns : rien 
@ensures : retourne l'abscisse x de p  
*/

float point_get_x(point p); 

/*
@requires : p est un pointeur valide pour le point
@assigns : rien 
@ensures : retourne l'ordonnée y de p  
*/

float point_get_y(point p);

/*
@requires : p est un pointeur valide pour le point
@assigns : rien 
@ensures : retourne la profondeur z de p  
*/

float point_get_z(point p);

/* ---------------------------------------------------------------------------------*/
/********** Fonctions basiques pour les droites ***********/
/* ---------------------------------------------------------------------------------*/

/*
@requires : d est un pointeur valide pour la droite 
@assigns : rien 
@ensures : retourne le point défini appartenant à d  
*/

point get_point(droite d);

/*
@requires : d est un pointeur valide pour la droite 
@assigns : rien 
@ensures : retourne le vecteur directeur de d 
*/

vecteur droite_get_vecteur(droite d);

/* ---------------------------------------------------------------------------------*/
/********** Fonctions basiques pour les plans ***********/
/* ---------------------------------------------------------------------------------*/

/*
@requires : p est un pointeur valide pour le plan  
@assigns : rien 
@ensures : retourne le vecteur normal de p 
*/

vecteur plan_get_vecteur(plan p); 

/*
@requires : p est un pointeur valide pour le plan 
@assigns : rien 
@ensures : retourne la distance par rapport à un point de p
*/

float get_distance(plan p);

/* ---------------------------------------------------------------------------------*/
/********** Fonctions pour les opérations de rotation d'éléments ***********/
/* ---------------------------------------------------------------------------------*/

/*
@requires : d est un pointeur valide pour la droite 
@assigns : rien 
@ensures : retourne une droite tournée d'un angle dans le sens trigonométrique
*/

droite rotation_droite(droite d, float angle);

/*
@requires : p est un pointeur valide pour le plan 
@assigns : rien 
@ensures : retourne un plan tourné d'un angle dans le sens trigonométrique 
*/

plan rotation_plan(plan p, float angle);

/* ---------------------------------------------------------------------------------*/
/********** Fonctions pour les opérations de rotation d'éléments ***********/
/* ---------------------------------------------------------------------------------*/

/*
@requires : d est un pointeur valide pour les droites 
@assigns : rien 
@ensures : retourne d d'angle d_teta (latitude) et d_phi (longitude) dans le sens trigo 
*/

void rotation_droite(droite d, float d_teta, float d_phi); 

/* --------------------------------------------------------------------------------------*/
/********** Fonctions pour les opérations d'intersection d'éléments *************/
/* --------------------------------------------------------------------------------------*/

/*
@requires : d1 et d2 sont deux pointeurs valides pour les droites 
@assigns : rien 
@ensures : retourne l'intersection de d1 et d2 de nature différente selon le résultat :
- si les deux droites sont confondues alors retourne l'une des deux droites 
- si les deux droites se croisent en un point retourne ce point 
- si les deux droites ne sont pas concourantes alors retourne null 
*/

TypeResultat inter_droite(droite d1, droite d2); 

#endif 
