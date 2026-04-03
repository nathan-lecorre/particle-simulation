#ifndef PARTICULES_H
#define PARTICULES_H 

typedef struct particule* particule;

/*
@requires : rien 
@assigns : rien 
@ensures : crée une particule avec ses coordonnées x,y,z et sa vitesse v de coordonnées vx,vy,vz
si la norme de v est non unitaire on la rend unitaire  
*/
particule create_particule(float x, float y, float z, float vx, float vy, float vz, float mass); 

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
@ensures : retourne la hauteur z de p  
*/
float get_z(particule p);

/*
@requires : p est un pointeur valide pour la particule 
@assigns : rien 
@ensures : retourne l'abscisse vx de la vitesse v de p 
*/
float get_vx(particule p);

/*
@requires : p est un pointeur valide pour la particule 
@assigns : rien 
@ensures : retourne l'ordonnée vy de la vitesse v de p 
*/
float get_vy(particule p);

/*
@requires : p est un pointeur valide pour la particule 
@assigns : rien 
@ensures : retourne la hauteur vz de la vitesse v de p 
*/
float get_vz(particule p);

/*
@requires : p est un pointeur valide pour la particule 
@assigns : rien 
@ensures : retourne l'attribut "mass" de p 
*/
float get_mass(particule p);

/*
@requires : p est un pointeur valide pour la particule 
@assigns : rien 
@ensures : retourne le rayon de la particule
*/
float get_radius(particule p);

/*
@requires : p est un pointeur valide pour la particule 
@assigns : x,y,z dans p
@ensures : rien mais p a les coordonnées x,y et z 
*/
void set_position(particule p, float x, float y, float z);

/*
@requires : p est un pointeur valide pour la particule 
@assigns : vx,vy,vz dans p 
@ensures : rien mais la vitesse v de p a le coordonnées vx,vy et vz  
*/
void set_speed(particule p, float vx, float vy, float vz);

/*
@requires : p est un pointeur valide pour la particule 
@assigns : mass dans p
@ensures : modifie l'attribut "mass" de la particule p
*/
void set_mass(particule p, float mass);

/*
@requires : p est un pointeur valide pour la particule 
@assigns : x,y,z,vx,vy,vz dans p 
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