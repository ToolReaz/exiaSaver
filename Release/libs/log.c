#include <stdio.h>
// Ecrit "event" dans le fichier "log.txt" avec un retour à la ligne à la fin
int logEvent(const char *event){
	FILE *k=fopen("log.txt", "a");
	if(k==NULL){return -1;}
	fprintf(k, "%s\n", event);
	fclose(k);
}
