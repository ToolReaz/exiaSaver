#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Structure qui représente un point avec ses valeurs Red, Green, Blue
struct RGB{
	int R;
	int G;
	int B;
};

// Structure qui représente une image
struct ArrRGB{
	struct RGB **Arr;
	int x;
	int y;
	int type;
};

// Fonction pour calculer le modulo
// Elle n'est utilisé que dans ce fichier uniquement pour corriger celle de base qui bug
int mod(int x, int y) {
	if (x >= 0) {
		return x % y;
	}
	if (x < 0) {
		return y - ((-x) % y);
	}
}

// Fonction: openPBM_PPM
// Description: ouvre les fichier ppm/pbm et renvoie une structure ArrRGB correspondant à l'image contenenue dans le fichier lu
// Paramètre: char path - le chemin vers le fichier pbm/ppm
struct ArrRGB openPBM_PPM(char *path){
	// Ouvre le fichier dont le path est passé en paramètre
	struct ArrRGB ret; // ret(our)
	FILE *file=fopen(path, "r");
	if(file==NULL){ret.type=-1;return ret;}
	char *line=malloc(512*sizeof(char));
	if(line==NULL){ret.type=-2;return ret;}
	int step=0;
	int rgbres=255;

	// Lit le nombre magique du fichier (PPM/PBM) et selectionne le type de l'image (1=monochrome, 2=RGB, 3=code couleur unix)
	while(step==0){
		int k=fscanf(file, "%s\n", line);
		if(k==1){
			if(strcmp(line, "P1")==0){
				ret.type=1;
				step=2;
			}
			if(strcmp(line, "P3")==0){
				ret.type=2;
				step=2;
			}
			if(strcmp(line, "P4")==0){
				ret.type=3;
				step=2;
			}
		}else{
			char ignore[2048];
			fgets(ignore, sizeof(ignore), file);
		}
	}

	// Récupère la largeur de l'image
	while(step==2){
		int y;
		int k=fscanf(file, "%d", &y);
		if(k==1){
			ret.y=y;
			//ret.Arr=malloc(y*(sizeof(struct RGB*)));
			fgetc(file);
			step=3;
		}else{
			char ignore[2048];
			fgets(ignore, sizeof(ignore), file);
		}			
	}

	// Récupère la hauteur de l'image et effectue les malloc pour mettre le tableau "ret" aux dimensions de l'image
	while(step==3){
		int x;
		int k=fscanf(file, "%d", &x);
		if(k==1){
			ret.x=x;
			ret.Arr=malloc(x*sizeof(struct RGB*));
			for(int i=0; i<ret.x; i++){
				ret.Arr[i]=malloc(ret.y*(sizeof(struct RGB)));
			}
			fgetc(file);
			step=5;
		}
	}

	// Si l'image est RGB (type=2), charge la résolution en couleur de l'image, sinon, on passe à l'étape suivante
	while(step==5){
		if(ret.type==2){
			int k=fscanf(file, "%d\n", &rgbres);
			if(k==1){
				step=6;
			}else{
				char ignore[2048];
				fgets(ignore, sizeof(ignore), file);
			}
		}else{
			step=6;
		}	
	}

	// Lecture des valeurs pour l'image (et transformation en RGB des images monochromes)
	while(step==6){
		int value=0;
		int _step=0;
		int indeX=0;
		int indeY=0;
		while(fscanf(file, "%d", &value)>0){
			if(ret.type==1){
				if(value==1){
					ret.Arr[indeX][indeY].R=0;
					ret.Arr[indeX][indeY].G=0;
					ret.Arr[indeX][indeY].B=0;
				}else{
					ret.Arr[indeX][indeY].R=255;
					ret.Arr[indeX][indeY].G=255;
					ret.Arr[indeX][indeY].B=255;
				}
					
				indeY++;
			}
			if(ret.type==2){
				int inG=0;
				if(inG==0&&_step==0){ret.Arr[indeX][indeY].R=value*255/rgbres;inG=1;_step=1;}
				if(inG==0&&_step==1){ret.Arr[indeX][indeY].G=value*255/rgbres;inG=1;_step=2;}
				if(inG==0&&_step==2){ret.Arr[indeX][indeY].B=value*255/rgbres;inG=1;_step=0;indeY++;}
			}
			if(ret.type==3){
				ret.Arr[indeX][indeY].R=value;
				indeY++;
			}
			fgetc(file);
			if(indeY==ret.y){
				indeX++;
				indeY=0;
			}
		}
		char ignore[2048];
		fgets(ignore, sizeof(ignore), file);
		if(indeX==ret.x){
			step=7;
		}
	}

	// Retourne le tableau "ret" si tout s'est bien passé
	if(step==7){
		return ret;
	}
}



// Fonction: ScreenSizePBM_PPM
// Description: Créer une grande image de la taille indiqué en paramètre et y insère l'image aux coordonnées indiquées en paramètre
// Paramètres:
// ArrRGB image : image à insérer
// int sizeX : taille en X de l'image à renvoyer
// int sizeY : taille en Y de l'image à renvoyer
// int CSGX : coordonnées du coin supérieur gauche de l'image à insérer dans la grande image
// int CSGY : coordonnées du coin supérieur gauche de l'image à insérer dans la grande image
struct ArrRGB ScreenSizePBM_PPM(struct ArrRGB image, int sizeX, int sizeY, int CSGX, int CSGY, struct RGB back){
	 struct ArrRGB screen;
	 screen.x=sizeX;
	 screen.y=sizeY;
	 // Malloc du tableau "screen"
	 screen.Arr=malloc(sizeX*sizeof(struct RGB*));
	 for(int i=0; i<sizeX; i++){
		 screen.Arr[i]=malloc(sizeY*sizeof(struct RGB));
		 for(int j=0; j<sizeY; j++){
			 screen.Arr[i][j]=back;
		 }
	 }
	 // Mise en place d'un type special pour l'image de retour
	 screen.type=10+image.type;
	 for(int i=0; i<image.x; i++){
		 for(int j=0; j<image.y; j++){
		 	// On remplace les struct RGB du tableau "Screen" par celles du tableau "image"
			int realcoordx = mod((CSGX + i), screen.x - 1);
			int realcoordy = mod((CSGY + j), screen.y - 1);
			screen.Arr[realcoordx][realcoordy] = image.Arr[i][j];
		 }
	 }
	 return screen;
 }
 


 // Fonction: printPBM_PPM
 // Description: Elle affiche dans la console la structure ArrRGB (l'image) passé en paramètre
 // Paramètre: struct ArrRGB
int printPBM_PPM(struct ArrRGB screen){
	 for(int i=0; i<screen.x; i++){
		 for(int j=0; j<screen.y; j++){
			 if(screen.type==12||screen.type==2||screen.type==11||screen.type==1){
				 printf("\x1b[38;2;%d;%d;%dm%s\x1b[0m", screen.Arr[i][j].R, screen.Arr[i][j].G, screen.Arr[i][j].B, "█");
// Affiche un caractère bloc ("█") de la couleur définie dans le struct RGB associé à ce caractère
			 }
			 if(screen.type==13||screen.type==3){
				 int color=screen.Arr[i][j].R;
				 int top=color/100;
				 int low=color%100;
				 printf("\x1b[%d;%dm%s\x1b[0m", top, low, "█");
				 // Affiche un caractère bloc ("█") de la couleur définie dans le struct RGB associé à ce caractère dans le code couleur UNIX
			 }
		 }
		 printf("\n");
	 }
	 return 0;
 }



 // Fonction: AppendImagePBM_PPM
 // Description: Elle insère une image aux coordonnées indiquées dans une autre
 // Paramètres:
 // ArrRGB background : image recevant l'autre
 // ArrRGB image : image à insérer
 // int CSGX : coordonnées du coin supérieur gauche de l'image à insérer dans l'autre
 // int CSGY : coordonnées du coin supérieur gauche de l'image à insérer dans l'autre
struct ArrRGB AppendImagePBM_PPM(struct ArrRGB background, struct ArrRGB image, int CSGX, int CSGY){
 for(int i=0; i<image.x; i++){
	 for(int j=0; j<image.y; j++){
		 background.Arr[CSGX+i][CSGY+j]=image.Arr[i][j];
	 }
 }
 return background;
}
