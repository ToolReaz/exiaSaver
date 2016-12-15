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
//Position de l'avion----------------------------------------------------------------
void plane_pos_save(){
    FILE* pos_avion = NULL;
    pos_avion = fopen("plane.pos", "w+");//Supprésion ancienne pos. et écriture de la nouvelle pos.
    if (pos_avion != NULL) {
      //Ecriture fichier
      fprintf(pos_avion, "%d\n%d", x,y );
    }
    if (pos_avion == NULL) {
      printf("plain.pos was not found, he tried to fly between two towers \n");
    }
}
int touche() {
WINDOW *w;
char c;
w = initscr();
noecho();
timeout(100);
c = getch();

//printf("received %c (%d)\n", c, (int) c);
return c;
}
// Module kill------------------------------------------------------------------------
void kill() {
int pid=getpid();
char tmp[32];
strcpy(tmp, "kill ");
char tmp2[16];
sprintf(tmp2, "%d", pid);
strcat(tmp, tmp2);
system(tmp);
}

struct ArrRGB avion_haut;
struct ArrRGB avion_bas;
struct ArrRGB avion_gauche;
struct ArrRGB avion_droite;

void show_plane(){
avion_haut = openPBM_PPM("exiasaver3/avionhaut.ppm");
avion_bas = openPBM_PPM("exiasaver3/avionbas.ppm");
avion_gauche = openPBM_PPM("exiasaver3/aviongauche.ppm");
avion_droite = openPBM_PPM("exiasaver3/aviondroite.ppm");
}

int moves() {
  //Déclarer les variables
  x=40;
  y=11;
  char c =122;
  char r =122;
  //Récuperer coordonés X et Y
  //sscanf (agrv[0], "%dx%d",&x,&y)
	struct ArrRGB screen;
	struct RGB bg;
	bg.R=0;
	bg.G=0;
	bg.B=0;
	int res=0;
  while(c=touche()){ //boucle infini so touche assigné à c
  if(c!=-1){// r devient c si on appuie sur une touche
    r=c;
  }
  if (r == /*z ascii code*/122){ y --; if(y==0){res=1;break;}y%= 23;/*printf("Z\n" );*/screen= ScreenSizePBM_PPM(avion_haut, 23, 80, y, x, bg);} // Incrémenter dans axe y

  //Aller en bas
  if (r == /*s ascii code*/115) {y ++;if(y>=18){res=2;break;} y%= 23;/*printf("S\n");*/screen= ScreenSizePBM_PPM(avion_bas, 23, 80, y, x, bg);}  //Décrémenter dans axe y

  //Aller à droite
  if (r == /*d ascii code*/100) {x ++; x%= 80;/*printf("D\n");*/screen= ScreenSizePBM_PPM(avion_droite, 23, 80, y, x, bg);}  //Incrémenter dans axe x

  //Aller à gauche
  if (r == /*q ascii code*/113) {x --; x%= 80;if(x<0){x=80+x;}/*printf("Q\n");*/screen= ScreenSizePBM_PPM(avion_gauche, 23, 80, y, x, bg);}  //Décrémenter dans axe x

  if (r == /*c ascii code*/99) {plane_pos_save(); kill();}
      //printf ("in=%d %dx%d\n", c, x, y);
      printf("\e[1;1H\e[2J");
      printPBM_PPM(screen);
      sleep (1);
      
    }
    echo();
    endwin();
    printf("\e[1;1H\e[2J");
	if(res==1){printf("\nStalling ...\n"); system("bash -c \"read -n1 -r\"&&clear");}
	if(res==2){printf("\nCrashing ...\n"); system("bash -c \"read -n1 -r\"&&clear");}
    return 0;
  }
  
int main(int argc, char const *argv[]) {
//Affich
show_plane();

moves();


// Ne fais pas bouger l'avion tant que l'utilisateur n'a pas entré de commandes, s'il en rentre attendre 0.25 (vitesse)

  return 0;
}
//-----------------------------------Fonctions secondaires---------------------------------------------------------------------

// Affichage de l'avion--------------------------------------------------------------




// Commandes de l'avion---------------------------------------------------------------
//Décomenter les printf pour débogger




// Module touche----------------------------------------------------------------------


