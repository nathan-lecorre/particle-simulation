#ifndef OPTIMIZATION_H
#define OPTIMIZATION_H

#include "../../headers/environnement.h"

/*
@requires : e est initialisé avec create_environnement, T >= 0
@assigns : positions et vitesses des particules dans e 
@ensures : modifie les positions et les vitesses des particules dans e de sorte à minimiser
la distance maximale entre deux particules lors de la première itération qui dépasse T secondes.
*/
void l_innovation(env e, float T);

#endif
