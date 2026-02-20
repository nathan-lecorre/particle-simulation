#ifndef _DISTRIBUTION_H
#define _DISTRIBUTION_H

/*@requires rien
  @assigns rien
  @ensures Renvoie un float aléatoire entre a et b choisi uniformément. */
float uniform(float a, float b);

/*@requires rien
  @assigns rien
  @ensures Renvoie un float aléatoire qui suit une loi normale N(mu,
sigma). */
float normal(float mu, float sigma);

/*@requires rien
  @assigns rien
  @ensures Renvoie un float aléatoire qui suit une loi exponentielle de paramètre
k. */
float exponential(float k);

/*@requires n est supérieur ou égal à 1, t et p sont des tableaux de float de taille n.
            De plus les éléments de p doivent être positif, de somme égale à 1.
  @assigns rien
  @ensures Renvoie un float aléatoire de t, le float t[i] doit être renvoyé avec une
probabilité p[i]. */
float discrete(int n, float* t, float* p);

#endif 
