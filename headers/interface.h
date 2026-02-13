#ifndef _INTERFACE_H
#define _INTERFACE_H

#include <particules.h>
#include <environnement.h>

/*  @requires: rien
    @assigns: rien
    @ensures: affiche l'état de part dans stdout */
void print_particule(particule part);

/*  @requires: rien
    @assigns: rien
    @ensures: affiche l'état de env dans stdout */
void print_environnement(env environnement);

/*  @requires: message est une chaîne de caractère correctement allouée se terminant par la sentinelle nulle.
    @assigns: rien
    @ensures: affiche message dans stdout */
void print_message(char* message);

/*  @requires: rien
    @assigns: ret 
    @ensures: renvoie l'entier écrit dans la console */
int get_int();

/*  @requires: rien
    @assigns: ret 
    @ensures: renvoie le flottant écrit dans la console */
float get_float();

/*  @requires: path est une chaîne de caractère correctement allouée se terminant par la sentinelle nulle et est le chemin d'un fichier de type pbm avec les permissions d'écriture
    @assigns: path
    @ensures: écrit dans le fichier path l'état actuel de environnement */
void snapshot(env environnement, char* path, int wp, int hp);

/*  @requires: path est une chaîne de caractère correctement allouée se terminant par la sentinelle nulle et est le chemin d'un répertoire vide avec les permissions d'écriture et de lecture
    @assigns: path
    @ensures: écrit dans le répertoire path t fichier pbm représentant t états successif de environnement */
void animate(env environnement, char* path, int wp, int hp, int t);


#endif
