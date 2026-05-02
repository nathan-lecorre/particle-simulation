#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "../research_10/experiments/optimization.h"

// Je met le code des tests ici car il est dit que mon code ne sera pas partagé, je ne sais pas 
// où il est censé vraiment être.


float test_innovation(int n, float T) {
    env environnement = create_environnement(n,100,100,5,0.1);
    
    l_innovation(environnement,T);
    float dt = get_dt(environnement);

    for (float t = 0.0 ; t < T ; t += dt) {
        move_particules(environnement);
    }

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

void make_experiment(float T) {

    int nb_exp = 100;

    float resultats[100];
    float temps[100];
    
    char* path = malloc(4096);
    snprintf(path,4096,"./test_%f.txt",T);

    FILE* f = fopen(path,"w");

    fwrite("Nb particules | Distance max | Temps d'éxécution\n",1,51,f);

    for (int i = 0 ; i < nb_exp ; i++) {

        float t1 = clock();
        resultats[i] = test_innovation((i+1)*5,T);
        float t2 = clock(); 

        temps[i] = (t2 - t1) / CLOCKS_PER_SEC;

        char* line = malloc(4096);
        snprintf(line,4096,"%d\t%f\t%f\n",(i+1)*10,resultats[i],temps[i]);
        fwrite(line,1,strlen(line),f);

        free(line);
    }

    free(path);
    fclose(f);
}

int main() {
    make_experiment(1);
    make_experiment(10);
    make_experiment(50);
    make_experiment(100);
    return 0;
}