#include <stdio.h>
#include <stdlib.h>
#include <time.h>														//Pour le random
#include <dirent.h>														//Pour lister les fichiers dans les répertoires
#include <unistd.h>														//Pour les instructions unix (exec ...)
#include <string.h>														//Pour les actions sur les strings
#include "log.h"

int main(int argc, char *argv[]){
	system("clear");
	FILE *file=fopen("log.txt","r");
	if(file!=NULL){
		fclose(file);
	}else{
		file=fopen("log.txt","w");
		if(file==NULL){printf("ERROR : un processus a bloqué l'accès à log.txt\n");return -9;}
		fprintf(file, "#Format de chaque ligne avec séparateur ;\n#Date format jj/mm/yyyy HH:MM:ss;\n#Niveau : 1=statique, 2=dynamique, 3=interactif;\n#Si 1, nom fichier ; Si 2, taille d'affichage ; Si 3, position initiale avion sur la console\n");
		fclose(file);
	}
	if(argc>1){
		if(strcmp(argv[1], "-stat")==0){
			int md=0;
			while(md==0){
				system("clear");
				printf("Choisissez :\n 1>Afficher l'historique de lancement de tous les termsavers\n 2>Afficher l'historique de lancement du termsaver1\n 3>Afficher l'historique de lancement du termsaver2\n 4>Afficher l'historique de lancement du termsaver3\n 5>Afficher les statistiques des termsavers triées\n 6>Afficher le ratio des termsavers\n\n >>Choix (1,2,3,4,5,6) : ");
				int ans;
				scanf("%d", &ans);
				if(ans>0 && ans<=6 ){
					md=ans;
				}
			}
			file=fopen("log.txt","r");
			char line[256];
			if(file==NULL){printf("ERROR : un processus a bloqué l'accès à log.txt\n");return -13;}
			char**content=malloc(sizeof(char*));
			if(content==NULL){printf("ERROR : content==NULL\n");return -14;}
			int lines=1;
			float countA=0;
			float countB=0;
			float countC=0;
			int *linesA=malloc(sizeof(int));
			if(linesA==NULL){printf("ERROR : linesA==NULL\n");return -17;}
			while(fgets(line, sizeof(line), file)!=NULL){
				char *checker=NULL;
				checker=strstr(line, "#");
				if(checker!=line){
					if(md==1){printf("%s", line);}
					if(md>=2&&md<5){
						char *token;
						token=strtok(line, ";");
						token=strtok(NULL, ";");
						if(atoi(token)==(md-1)){
							printf("%s\n", line);
						}
					}
					if(md==6){
						char *token=strtok(line, ";");
						token=strtok(NULL, ";");
						if(atoi(token)==1){countA++;}
						if(atoi(token)==2){countB++;}
						if(atoi(token)==3){countC++;}
					}
					if(md==5){
						content[lines-1]=malloc(strlen(line));
						if(content[lines-1]==NULL){printf("ERROR : content[%d]==NULL\n", lines-1);return -15;}
						memcpy(content[lines-1],line, strlen(line));
						content=realloc(content, (lines+1)*sizeof(char*)+2);
						if(content==NULL){printf("ERROR : content==NULL\n");return -16;}
						lines++;
						linesA[lines-1]=strlen(line);
						linesA=realloc(linesA, lines*sizeof(int)+2);
						if(linesA==NULL){printf("ERROR : linesA==NULL\n");return -18;}
					}
				}
			}
			if(md==6){
				printf("Statistiques d'utilisation :\n\n TermSaver 1 (statique) : %f%%\n TermSaver 2 (dynamique) : %f%%\n TermSaver 3 (interactif) : %f%%\n", 100*countA/(countA+countB+countC), 100*countB/(countA+countB+countC), 100*countC/(countA+countB+countC));
			}
			if(md==5){
				for(int i=1; i<4;i++){
					for(int j=0;j<lines-1;j++){
						int l=strcspn(content[j], ";");
						int k=48+i;
						if((char)content[j][l+1]==(char)k){
							char*text=strtok(content[j],"\n");
							printf("%s\n", text/*content[j]*/);
						}
					}
					printf("\n");
				}
			}
			fclose(file);
		}else{
			printf("ERROR : Paramètre %s inconnu.\n",argv[1]);
		}
	}else{
		srand(time(NULL));
		int nb=1;//rand()%3+1;
		char *ESL;
		if(getenv("EXIASAVER_HOME")==NULL){									//Si la variable d'environnement n'est pas définie
			ESL=malloc(512*sizeof(char));
			if(ESL==NULL){printf("ERROR : ESL==NULL\n");return -1;}			//Si l'allocation s'est bien passée
			ESL=".";
		}else{
			ESL=malloc(512*sizeof(char));
			if(ESL==NULL){printf("ERROR : ESL==NULL\n");return -2;}			//Si l'allocation s'est bien passée
			ESL=getenv("EXIASAVER_HOME");
		}
		char realPSS[PATH_MAX];
		realpath(ESL, realPSS);
		if(nb==1){															//ScreenSaver #1
			DIR *d;
			struct dirent *dir;
			char *ESP;
			if(getenv("EXIASAVER1_PBM")==NULL){								//Si la variable d'environnement n'est pas définie
				ESP=malloc(sizeof(2*sizeof(char)));
				if(ESP==NULL){printf("ERROR : ESP==NULL\n");return -3;}		//Si l'allocation s'est bien passée
				ESP=".";
			}else{
				ESP=malloc(512*sizeof(char));
				if(ESP==NULL){printf("ERROR : ESP==NULL\n");return -4;}		//Si l'allocation s'est bien passée
				ESP=getenv("EXIASAVER1_PBM");
				printf("EXIASAVER1_PBM==%s\n",ESP);
			}
			char realP[PATH_MAX];
			realpath(ESP, realP);
			d=opendir(realP);
			if(!d){printf("ERROR : Répertoire '%s' n'existe pas!\n",realP);return -8;}
			char **Fichiers;
			int nbFichiers=0;
			Fichiers=malloc(sizeof(char*));
			if(Fichiers==NULL){printf("ERROR : Fichiers==NULL\n");return -5;}	//Si l'allocation s'est bien passée
			while((dir=readdir(d))!=NULL){
				char *dot=strrchr(dir->d_name,'.');
				if(dir->d_type==DT_REG && ((dot && !strcmp(dot, ".pbm"))||(dot && !strcmp(dot, ".ppm")))){
					nbFichiers++;
					Fichiers[nbFichiers-1]=malloc(sizeof(dir->d_name));
					if(Fichiers[nbFichiers-1]==NULL){printf("ERROR : Fichiers[nb]==NULL\n");return -6;}	//Si l'allocation s'est bien passée
					Fichiers[nbFichiers-1]=dir->d_name;
				}
			}
			closedir(d);
			if(nbFichiers>0){
				srand(time(NULL));
				int nbFich=rand()%nbFichiers;
				char *FichCur=malloc(sizeof(Fichiers[nbFich]));
				if(FichCur==NULL){printf("ERROR : FichCur==NULL\n");return -12;}	//Si l'allocation s'est bien passée
				strcpy(FichCur, Fichiers[nbFich]);
				char *commandLine=NULL;
				int len=0;
				len=strlen(realPSS)+strlen("/exiasaver1");
				commandLine=malloc(len/sizeof(char));
				if(commandLine==NULL){printf("ERROR : commandLine==NULL\n"); return -7;}
				strcpy(commandLine, realPSS);
				strcat(commandLine, "/exiasaver1");
				time_t ti=time(NULL);
				struct tm tm = *localtime(&ti);
				char *timeT=malloc(256*sizeof(char));
				if(timeT==NULL){printf("ERROR : timeT==NULL\n");return -11;}
				sprintf(timeT, "%d/%d/%d %d:%d:%d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
				strcat(timeT, ";1;");
				strcat(timeT, FichCur);
				logEvent(timeT);
				execl(commandLine, FichCur, (char*)NULL);
			}else{
				printf("ERROR : nbFichiers==%d (<1)\n", nbFichiers);
			}
		}
		if(nb==2){
			int len=strlen(realPSS)+strlen("/exiasaver2");
			char *command=malloc(len/sizeof(char));
			if(command==NULL){printf("ERROR : command==NULL\n"); return -19;}
			strcpy(command, realPSS);
			strcat(command, "/exiasaver2");
			time_t ti=time(NULL);
			struct tm tm = *localtime(&ti);
			char *timeT=malloc(256*sizeof(char));
			if(timeT==NULL){printf("ERROR : timeT==NULL\n");return -11;}
			sprintf(timeT, "%d/%d/%d %d:%d:%d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
			strcat(timeT, ";2;");
			
			char *EST;
			if(getenv("EXIASAVER2_TAILLE")==NULL){									//Si la variable d'environnement n'est pas définie
				EST=malloc(512*sizeof(char));
				if(EST==NULL){printf("ERROR : EST==NULL\n");return -20;}			//Si l'allocation s'est bien passée
				EST="5x3";
			}else{
				EST=malloc(512*sizeof(char));
				if(EST==NULL){printf("ERROR : EST==NULL\n");return -21;}			//Si l'allocation s'est bien passée
				EST=getenv("EXIASAVER2_TAILLE");
			}
			strcat(timeT, EST);
			logEvent(timeT);
			char *args[]={NULL};
			execv(command, args);
		}
		if(nb==3){
			file=fopen("plane.pos","r");
			if(file==NULL){
				file=fopen("plane.pos", "w");
				if(file==NULL){printf("ERROR : un processus a bloqué l'accès à plane.pos\n");return -22;}
				fprintf(file,"40x12");
				fclose(file);
			}
			char *pos=malloc(strlen("00x00")/sizeof(char));
			if(pos==NULL){printf("ERROR : pos==NULL\n");return -23;}
			file=fopen("plane.pos", "r");
			char line[6];
			fgets(line, sizeof(line), file);
			strcat(pos, line);
			fclose(file);
			int len=strlen(realPSS)+strlen("/exiasaver3");
			char *command=malloc(len/sizeof(char));
			if(command==NULL){printf("ERROR : command==NULL\n"); return -19;}
			strcpy(command, realPSS);
			strcat(command, "/exiasaver3");
			time_t ti=time(NULL);
			struct tm tm = *localtime(&ti);
			char *timeT=malloc(256*sizeof(char));
			if(timeT==NULL){printf("ERROR : timeT==NULL\n");return -11;}
			sprintf(timeT, "%d/%d/%d %d:%d:%d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
			strcat(timeT, ";3;");
			strcat(timeT, pos);
			logEvent(timeT);
			execl(command, pos, (char*)NULL);
		}
	}
}
