#include <stdio.h>
#include <time.h>

#include "../research_10/experiments/optimization.h"


float test_innovation(int n, float T) {
    env environnement = create_environnement(n,100,100,5,0.1);
    l_innovation(environnement,T);
    float maxi = 0.0;

    for (int i = 0 ; i < n ; i++) {
        particule p1 = get_particule(environnement,i);

        for (int j = i+1 ; j < n ; j++) {
            particule p2 = get_particule(environnement,j);
            float dist = distance(p1,p2);
            
            if (dist > maxi) {
                maxi = dist;
            }
        }
    }

    free_environnement(environnement);
    return maxi;
}

int main() {
    int nb_exp = 100;
    float T = 10.0;

    float resultats[100];
    float temps[100];

    for (int i = 2 ; i < nb_exp + 2 ; i++) {

        float t1 = time(NULL);
        resultats[i] = test_innovation(i,T);
        float t2 = time(NULL);

        temps[i] = t2 - t1;

        printf("n = %d : distance max = %f , temps d'éxécution = %f\n",i,resultats[i],temps[i]);
    }
}