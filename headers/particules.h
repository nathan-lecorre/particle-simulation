#ifndef PARTICULES_H
#define PARTICULES_H 

typedef struct particule* particule;

/*
@requires : rien 
@assigns : rien 
@ensures : crée une particule avec ses coordonnées x,y et sa vitesse v de coordonnées vx,vy
si la norme de v est non unitaire on la rend unitaire  
*/
particule create_particule(float x, float y, float vx, float vy); 

/*
@requires : p est un pointeur valide pour la particule 
@assigns : rien 
@ensures : libère la mémoire allouée pour p 
*/

void free_particule(particule p);

/*
@requires : p est un pointeur valide pour la particule 
@assigns : rien 
@ensures : retourne l'abscisse x de p  
*/

float get_x(particule p);

/*
@requires : p est un pointeur valide pour la particule 
@assigns : rien 
@ensures : retourne l'ordonnée y de p
*/

float get_y(particule p);

/*
@requires : p est un pointeur valide pour la particule 
@assigns : rien 
@ensures : retourne l'abscisse vx de la vitesse v de p 
*/

float get_vx(particule p);

/*
@requires : p est un pointeur valide pour la particule 
@assigns : rien 
@ensures : retourne l'ordonnée vx de la vitesse v de p 
*/

float get_vy(particule p);

/*
@requires : p est un pointeur valide pour la particule 
@assigns : x et y dans p
@ensures : rien mais p a les coordonnées x et y 
*/

void set_position(particule p, float x, float y);

/*
@requires : p est un pointeur valide pour la particule 
@assigns : vx et vy dans p 
@ensures : rien mais la vitesse v de p a le coordonnées vx et vy 
*/

void set_speed(particule p, float vx, float vy);

/*
@requires : p est un pointeur valide pour la particule 
@assigns : x,y,vx et vy dans p 
@ensures : rien mais p se déplace pendant dt 
*/

void move(particule p, float dt);

/*
@requires : p1 et p2 sont des pointeurs valides pour les particules 
@assigns : rien 
@ensures : retourne la distance euclidienne entre p1 et p2 
*/

float distance(particule p1, particule p2);

#endif 