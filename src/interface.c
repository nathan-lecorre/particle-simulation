#include "../headers/interface.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_PATH_LENGTH 4096

void print_particule(particule part){
	printf("Position (x, y) : (%f, %f)\nVitesse (vx, vy) : (%f, %f)\n", get_x(part), get_y(part), get_vx(part), get_vy(part));
}

void print_environnement(env environnement){
	printf("Nombre de particules : %d\nDimensions (w, h) : (%f, %f)\nRayon d'interaction : %f\nDuree d'iteration: %f\n", get_n(environnement), get_w(environnement), get_h(environnement), get_r(environnement), get_dt(environnement));
}

void print_message(char* message){
	printf("%s\n", message);
}

int get_int(){
	int user_input;
	scanf("%d\n", &user_input);
	return user_input;
}

float get_float(){
	float user_input;
	scanf("%f\n", &user_input);
	return user_input;
}

void snapshot(env environnement, char* path, int wp, int hp){
	FILE* bitmap;
	int nb_particules = get_n(environnement);
	float we = get_w(environnement); // Largeur de environnement
	float he = get_h(environnement); // Hauteur de environnement
	char* string_wp_hp[32]; // Contiendra le string "wp hp" avec wp et hp remplacé par leur valeur
	int string_wp_hp_length; // Contiendra la taille de la chaîne string_wp_hp
    char* commentaire_body = "# %s represente l'environnement a un instant t."
    int commentaire_full_length = strlen(path) + strlen(commentaire_body); // On prend un peu de marge mais c'est pas grave
    char* commentaire_full = malloc(commentaire_full_length);
	char* pixel_array = malloc(wp*hp); // Un tableau de taille wp*hp qui contiendra les pixels de l'image qu'on va écrire

    memset(pixel_array, '0', wp*hp);
	
	bitmap = fopen(path, "w");
	if(!bitmap){
		fprintf(stderr, "[snapshot] : Couldn't take snapshot %s.\n", path);
		perror("[fopen]");
		return;
	}

	string_wp_hp_length = snprintf(string_wp_hp, 32, "%d %d\n", wp, hp);

    commentaire_length = snprintf(commentaire, commentaire_length, "# %s représente l'environnement à un instant t\n", path);

	for(int i = 0; i < nb_particules; i++){
		particule part = get_particule(environnement, i);
		float xe, ye; // Les coordonnées de la particule i dans l'environnement
		int xp, yp; // Les coordonnées de la particule i projetées sur la bitmap

		xe = get_x(part);
		ye = get_y(part);
		xp = floorf(xe/(we + 1)*wp); // On prends we + 1 pour éviter un edge-case (c'est le cas de le dire :) ) quand la particule est pile au bord
		yp = floorf(ye/(he + 1)*hp);

        if(0 <= xp && xp < wp && 0 <= yp && yp < hp) pixel_array[(wp-yp-1)*wp + xp] = '1'; // On essaye pas d'afficher les particules out of bounds pour ne pas SEGFAULT
	}

	fwrite("P1\n", 1, 3, bitmap);
    fwrite(commentaire, 1, commentaire_length, bitmap);
	fwrite(string_wp_hp, 1, string_wp_hp_length, bitmap);
	fwrite(pixel_array, 1, wp*hp, bitmap);

    free(string_wp_hp);
    free(commentaire);
    free(pixel_array);
    fclose(bitmap);
}

void animate(env environnement, char* path, int wp, int hp, int t){
    char* filepath = malloc(MAX_PATH_LENGTH);

    for(int i = 0; i < t; i++){
        snprintf(filepath, MAX_PATH_LENGTH, "%s/env-%d.pbm", path, i);
        snapshot(environnement, filepath, wp, hp);
        move_particules(environnement);
    }

    free(filepath);
}
