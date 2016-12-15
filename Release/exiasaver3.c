// Pour compiler : argument à la fin -lncurses

#include <time.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include "libs/PBM.h"
#include <string.h> 
#include <math.h> 
#include <ncurses.h> 
#include <unistd.h>
int x;
int y;
int start = 0;
//Position de l'avion----------------------------------------------------------------
void plane_pos_save() {
	FILE * pos_avion = fopen("plane.pos", "w+"); //Suppression ancienne pos. et écriture de la nouvelle pos.
	if (pos_avion != NULL) {
		//Ecriture fichier

		fprintf(pos_avion, "%dx%d", x, y);
		fclose(pos_avion);
	}
	if (pos_avion == NULL) {
		printf("plain.pos was not found, he tried to fly between two towers \n");
	}
}
int touche() {
		char c;
		if (start) {
			//
			initscr();
			noecho();
			timeout(100);
			c = getch();

			//printf("received %c (%d)\n", c, (int) c);
			return c;
		} else {
			start = 1;
			srand(time(NULL));
			int k = rand() % 4;
			if (k == 0) {
				c = 122;
			}
			if (k == 1) {
				c = 115;
			}
			if (k == 2) {
				c = 100;
			}
			if (k == 3) {
				c = 113;
			}
			return c;
		}
	}
	// Module kill------------------------------------------------------------------------
void kill() {
	echo();
	endwin();
	int pid = getpid();
	char tmp[32];
	strcpy(tmp, "kill ");
	char tmp2[16];
	sprintf(tmp2, "%d", pid);
	strcat(tmp, tmp2);
	strcat(tmp, "&&clear");
	system(tmp);
}

struct ArrRGB avion_haut;
struct ArrRGB avion_bas;
struct ArrRGB avion_gauche;
struct ArrRGB avion_droite;

void show_plane() {
	char * EXIASAVER3_PBM = malloc(512);
	if (getenv("EXIASAVER3_PBM") == NULL) {
		EXIASAVER3_PBM = "Plane";
	} else {
		EXIASAVER3_PBM = getenv("EXIASAVER3_PBM");
	}
	char * pure = malloc(512);
	char * exiapure = malloc(512);
	strcpy(exiapure, EXIASAVER3_PBM);
	strcat(exiapure, "/avion");
	strcpy(pure, exiapure);
	strcat(pure, "haut.ppm");
	avion_haut = openPBM_PPM(pure);
	strcpy(pure, exiapure);
	strcat(pure, "bas.ppm");
	avion_bas = openPBM_PPM(pure);
	strcpy(pure, exiapure);
	strcat(pure, "gauche.ppm");
	avion_gauche = openPBM_PPM(pure);
	strcpy(pure, exiapure);
	strcat(pure, "droite.ppm");
	avion_droite = openPBM_PPM(pure);
}

void moves() {
	//Déclarer les variables

	char c = 122;
	char r;

	//Récuperer coordonés X et Y
	//sscanf (agrv[0], "%dx%d",&x,&y)
	struct ArrRGB screen;
	struct RGB bg;
	bg.R = 127;
	bg.G = 127;
	bg.B = 127;
	while (c = touche()) { //boucle infini so touche assigné à c
		if (c == 122 || c == 115 || c == 100 || c == 113 || c == 99) { // r devient c si on appuie sur une touche
			r = c;
		}
		if (r == /*z ascii code*/ 122) {
			y--; /*if(y==0){y=23;/*res=1;break;}*/
			y = mod(y, 23); /*printf("Z\n" );*/
			screen = ScreenSizePBM_PPM(avion_haut, 23, 80, y, x, bg);
		} // Incrémenter dans axe y

		//Aller en bas
		if (r == /*s ascii code*/ 115) {
			y++; /*if(y>=18){res=2;break;}*/
			y = mod(y, 23); /*printf("S\n");*/
			screen = ScreenSizePBM_PPM(avion_bas, 23, 80, y, x, bg);
		} //Décrémenter dans axe y

		//Aller à droite
		if (r == /*d ascii code*/ 100) {
			x++;
			x %= 80; /*printf("D\n");*/
			screen = ScreenSizePBM_PPM(avion_droite, 23, 80, y, x, bg);
		} //Incrémenter dans axe x

		//Aller à gauche
		if (r == /*q ascii code*/ 113) {
			x--;
			x %= 80;
			if (x < 0) {
				x = 80 + x;
			} /*printf("Q\n");*/
			screen = ScreenSizePBM_PPM(avion_gauche, 23, 80, y, x, bg);
		} //Décrémenter dans axe x

		if (r == /*c ascii code*/ 99) {
			plane_pos_save();
			kill();
		}
		//printf ("in=%d %dx%d\n", c, x, y);
		printf("\e[1;1H\e[2J");
		printPBM_PPM(screen);
		sleep(1);

	}
}

int main(int argc, char const * argv[]) {
	//Affich
	if (argc == 1) {
		char * arg = malloc(8);
		strcpy(arg, argv[0]);
		int k = sscanf(argv[0], "%dx%d", & x, & y);
		if (k == 2) {
			show_plane();
			moves();
		}

		// Ne fais pas bouger l'avion tant que l'utilisateur n'a pas entré de commandes, s'il en rentre attendre 0.25 (vitesse)

		//return 0;
	}
}
	//-----------------------------------Fonctions secondaires---------------------------------------------------------------------

// Affichage de l'avion--------------------------------------------------------------

// Commandes de l'avion---------------------------------------------------------------
//Décomenter les printf pour débogger

// Module touche----------------------------------------------------------------------
