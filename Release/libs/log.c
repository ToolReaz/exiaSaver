#include <stdio.h>
int logEvent(const char *event){
	FILE *k=fopen("log.txt", "a");
	if(k==NULL){return -1;}
	fprintf(k, "%s\n", event);
	fclose(k);
}
