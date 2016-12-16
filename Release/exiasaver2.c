#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <memory.h>
#include <unistd.h>
#include "libs/PBM.h"

int main (int argc, char *argv[]){
    // VARIABLES
    char *EXIASAVER2_PBM;
    char *EXIASAVER2_TAILLE;
    char *EXIASAVER2_SLEEP;

    time_t timeSec;
    struct tm timeHms;

    // Récupère les variables d'environement
    // EXIASAVER2_PBM - Répertoire ou se trouvent tous les fichiers PBM @Défaut=.
    // EXIASAVER2_TAILLE - Taille affichage chiffres - @défaut=5x3
    // EXIASAVER2_SLEEP - Temps en seconde entre chaques rafraichissements
    if(getenv("EXIASAVER2_PBM") == NULL){
        EXIASAVER2_PBM = "nbrs";
    }else{
        EXIASAVER2_PBM = getenv("EXIASAVER2_PBM");
    }
    if(getenv("EXIASAVER2_TAILLE") == NULL){
        EXIASAVER2_TAILLE = "3x5";
    }else{
        EXIASAVER2_TAILLE=getenv("EXIASAVER2_TAILLE");
    }
    if(getenv("EXIASAVER2_SLEEP") == NULL){
        EXIASAVER2_SLEEP = "10";
    }else{
        EXIASAVER2_SLEEP = getenv("EXIASAVER2_SLEEP");
    }

    struct ArrRGB *tableau=malloc(11*sizeof(struct ArrRGB));        // INCEPTION
    if(tableau==NULL){printf("Error null\n");}
    char *exiapure=malloc(512);
    char *ext=malloc(16);
    ext="ppm";
    strcpy(exiapure, EXIASAVER2_PBM);
    strcat(exiapure, "/0.");
    strcat(exiapure, ext);
    tableau[0] = openPBM_PPM(exiapure);
    strcpy(exiapure, EXIASAVER2_PBM);
    strcat(exiapure, "/1.");
    strcat(exiapure, ext);
    tableau[1] = openPBM_PPM(exiapure);

    strcpy(exiapure, EXIASAVER2_PBM);
    strcat(exiapure, "/2.");
    strcat(exiapure, ext);
    tableau[2] = openPBM_PPM(exiapure);

    strcpy(exiapure, EXIASAVER2_PBM);
    strcat(exiapure, "/3.");
    strcat(exiapure, ext);
    tableau[3] = openPBM_PPM(exiapure);

    strcpy(exiapure, EXIASAVER2_PBM);
    strcat(exiapure, "/4.");
    strcat(exiapure, ext);
    tableau[4] = openPBM_PPM(exiapure);

    strcpy(exiapure, EXIASAVER2_PBM);
    strcat(exiapure, "/5.");
    strcat(exiapure, ext);
    tableau[5] = openPBM_PPM(exiapure);

    strcpy(exiapure, EXIASAVER2_PBM);
    strcat(exiapure, "/6.");
    strcat(exiapure, ext);
    tableau[6] = openPBM_PPM(exiapure);

    strcpy(exiapure, EXIASAVER2_PBM);
    strcat(exiapure, "/7.");
    strcat(exiapure, ext);
    tableau[7] = openPBM_PPM(exiapure);

    strcpy(exiapure, EXIASAVER2_PBM);
    strcat(exiapure, "/8.");
    strcat(exiapure, ext);
    tableau[8] = openPBM_PPM(exiapure);

    strcpy(exiapure, EXIASAVER2_PBM);
    strcat(exiapure, "/9.");
    strcat(exiapure, ext);
    tableau[9] = openPBM_PPM(exiapure);

    strcpy(exiapure, EXIASAVER2_PBM);
    strcat(exiapure, "/dd.");
    strcat(exiapure, ext);
    tableau[10] = openPBM_PPM(exiapure);
    while (1) {
        struct ArrRGB bg;
        bg.x = 5;
        bg.y = 40;
        bg.type = 2;
        bg.Arr=malloc(bg.x*sizeof(struct RGB*));
        for (int i = 0; i < bg.x; i++) {
            bg.Arr[i]=malloc(bg.y*sizeof(struct RGB));
            for (int j = 0; j < bg.y; ++j) {
                bg.Arr[i][j].R = 0;
                bg.Arr[i][j].G = 0;
                bg.Arr[i][j].B = 0;
            }
        }
        struct ArrRGB im1;
        im1.x=bg.x;
        im1.y=bg.y;
        im1.type=bg.type;
        im1.Arr=malloc(im1.x*sizeof(struct RGB*));
        for (int i = 0; i < im1.x; i++) {
            im1.Arr[i]=malloc(im1.y*sizeof(struct RGB));
            for (int j = 0; j < im1.y; ++j) {
                im1.Arr[i][j].R = 0;
                im1.Arr[i][j].G = 0;
                im1.Arr[i][j].B = 0;
            }
        }
        // Récupérer l'heure
        time(&timeSec);
        // Formater l'heure avec une structure de la librairie time.h
        timeHms = *localtime(&timeSec);

        // Sépare les dixaines des unités
        int H = timeHms.tm_hour/10;
        int h = timeHms.tm_hour%10;
        int M = timeHms.tm_min/10;
        int m = timeHms.tm_min%10;
        int S = timeHms.tm_sec/10;
        int s = timeHms.tm_sec%10;

        // Fusionne chaques chiffres pour former une seul image finale (en utilisant la librairie PBM)
        im1 = AppendImagePBM_PPM(bg, tableau[H], 0, 0);
        bg = AppendImagePBM_PPM(im1, tableau[h], 0, 5);
        im1 = AppendImagePBM_PPM(bg, tableau[10], 0, 10);
        bg = AppendImagePBM_PPM(im1, tableau[M], 0, 15);
        im1 = AppendImagePBM_PPM(bg, tableau[m], 0, 20);
        bg = AppendImagePBM_PPM(im1, tableau[10], 0, 25);
        im1 = AppendImagePBM_PPM(bg, tableau[S], 0, 30);
        bg = AppendImagePBM_PPM(im1, tableau[s], 0, 35);

        // Paramètre la couleur de fond
        struct RGB background;
        background.R=0;
        background.G=0;
        background.B=0;

        struct ArrRGB fs=ScreenSizePBM_PPM(bg, 23, 80, 11-bg.x/2, 40-bg.y/2, background);
        system("clear");
        
        printPBM_PPM(fs);

        printf("Cet écran sera actualisé dans quelques secondes ");
        fflush(stdout);

        //sleep(1);

        // Ajoute un '.' toutes les secondes pendant EXIASAVER2_SLEEP secondes0
        for (int k = 0; k < atoi(EXIASAVER2_SLEEP); k++) {
            printf(".");
            fflush(stdout);
            sleep(1);
        }

    }
}
