#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct RGB{
	int R;
	int G;
	int B;
};
struct ArrRGB{
	struct RGB **Arr;
	int x;
	int y;
	int type;
};

struct ArrRGB openPBM_PPM(char *path){
	struct ArrRGB ret;
	FILE *file=fopen(path, "r");
	if(file==NULL){ret.type=-1;return ret;}
	char *line=malloc(512*sizeof(char));
	if(line==NULL){ret.type=-2;return ret;}
	int step=-1;
	int rgbres=255;
	while(step==-1){
		int k=fscanf(file, "#%s\n", line);
		if(k==0){step=0;}
	}
	while(step==0){
		int k=fscanf(file, "%s\n", line);
		if(k==1){
			if(strcmp(line, "P1")==0){
				ret.type=1;
				step=1;
			}
			if(strcmp(line, "P3")==0){
				ret.type=2;
				step=1;
			}
			if(strcmp(line, "P4")==0){
				ret.type=3;
				step=1;
			}
		}
	}
	while(step==1){
		int k=fscanf(file, "#%s\n", line);
		if(k==0){step=2;}
	}
	while(step==2){
		//printf("getting coords\n");
		int x;
		int k=fscanf(file, "%d", &x);
		//printf("%d\n", k);
		if(k==1){
			//printf("size==%d\n", x);
			ret.x=x;
			ret.Arr=malloc(x*(sizeof(struct RGB*)));
			fgetc(file);
			//for(int i=0;i<x;i++){
			//	ret.Arr[i]=malloc(y*(sizeof(struct RGB)+1));
			//}
			//ret.y=y;
			//printf("got coords\n");
			step=3;
		}
	}
	while(step==3){
		int y;
		int k=fscanf(file, "%d", &y);
		//printf("%d\n", k);
		if(k==1){
			//printf("other size==%d\n", y);
			ret.y=y;
			for(int i=0; i<ret.x; i++){
				ret.Arr[i]=malloc(y*(sizeof(struct RGB)));
			}
			fgetc(file);
			step=4;
		}
	}
	while(step==4){
		int k=fscanf(file, "#%s\n", line);
		if(k==0){step=5;}
	}
	while(step==5){
		if(ret.type==2){
			int k=fscanf(file, "%d\n", &rgbres);
			if(k==1){
				step=6;
			}
		}else{
			step=6;
		}	
	}
	while(step==6){
		int value=0;
		int _step=0;
		int indeX=0;
		int indeY=0;
		while(fscanf(file, "#%s\n", line)>0){}
		while(fscanf(file, "%d", &value)>0){
			if(ret.type==1){
				if(value==1){
					ret.Arr[indeY][indeX].R=0;
					ret.Arr[indeY][indeX].G=0;
					ret.Arr[indeY][indeX].B=0;
				}else{
					ret.Arr[indeY][indeX].R=255;
					ret.Arr[indeY][indeX].G=255;
					ret.Arr[indeY][indeX].B=255;
				}
					
				indeX++;
			}
			if(ret.type==2){
				int inG=0;
				if(inG==0&&_step==0){ret.Arr[indeX][indeY].R=value*255/rgbres;inG=1;_step=1;}
				if(inG==0&&_step==1){ret.Arr[indeX][indeY].G=value*255/rgbres;inG=1;_step=2;}
				if(inG==0&&_step==2){ret.Arr[indeX][indeY].B=value*255/rgbres;inG=1;_step=0;indeX++;}
			}
			if(ret.type==3){
				ret.Arr[indeY][indeX].R=value;
				indeX++;
			}
			fgetc(file);
			if(indeX==ret.x){
				indeY++;
				indeX=0;
			}
		}
		step=7;
	}
	if(step==7){
		return ret;
	}
}

struct ArrRGB ScreenSizePBM_PPM(struct ArrRGB image, int sizeX, int sizeY, int CSGX, int CSGY){
	 struct ArrRGB screen;
	 screen.x=sizeY;
	 screen.y=sizeX;
	 screen.Arr=malloc(sizeX*sizeof(struct RGB*));
	 for(int i=0; i<sizeX; i++){
		 screen.Arr[i]=malloc(sizeY*sizeof(struct RGB));
		 for(int j=0; j<sizeY; j++){
			 screen.Arr[i][j].R=127;
			 screen.Arr[i][j].G=127;
			 screen.Arr[i][j].B=127;
		 }
	 }
	 screen.type=10+image.type;
	 //printf("haya\n");
	 for(int i=0; i<image.x; i++){
		 for(int j=0; j<image.y; j++){
			 //printf("screen{%d,%d}(%d,%d)=image{%d,%d}(%d,%d)\n", screen.x, screen.y, CSGX+i, CSGY+j, image.x, image.y, i, j);
			 //printf("image[%d,%d,%d]\n", image.Arr[i][j].R, image.Arr[i][j].G, image.Arr[i][j].B);
			 if(image.type==2){screen.Arr[CSGX+i][CSGY+j]=image.Arr[i][j];}
			 if(image.type==1){screen.Arr[CSGX+i][CSGY+j]=image.Arr[j][i];}
			 //printf("halala %d \n", j);
		 }
		 //printf("hololo %d\n", i);
	 }
	 return screen;
 }
 
int printPBM_PPM(struct ArrRGB screen){
	 //system("clear");
	 for(int i=0; i<screen.x; i++){
		 for(int j=0; j<screen.y; j++){
			 /*if(screen.type==11||screen.type==1){
				 //printf("%d", screen.Arr
				 if(screen.Arr[i][j].R==0){printf(/*"\x1b[38;2;255;255;255m%s\x1b[0m*\/"█"/*\x1b[0m"*\/);}else{printf(" ");}
			 }*/
			 if(screen.type==12||screen.type==2||screen.type==11||screen.type==1){
				 printf("\x1b[38;2;%d;%d;%dm%s\x1b[0m", screen.Arr[j][i].R, screen.Arr[j][i].G, screen.Arr[j][i].B, "█");
			 }
			 if(screen.type==13||screen.type==3){
				 int color=screen.Arr[i][j].R;
				 int top=color/100;
				 int low=color%100;
				 printf("\x1b[%d;%dm%s\x1b[0m", top, low, "█");
			 }
		 }
		 printf("\n");
	 }
	 return 0;
 }
struct ArrRGB AppendImagePBM_PPM(struct ArrRGB background, struct ArrRGB image, int CSGX, int CSGY){
 for(int i=0; i<image.x; i++){
	 for(int j=0; j<image.y; j++){
		 if(image.type==2){background.Arr[CSGX+i][CSGY+j]=image.Arr[i][j];}
		 if(image.type==1){background.Arr[CSGX+i][CSGY+j]=image.Arr[j][i];}
		 if(image.type==3){background.Arr[CSGX+i][CSGY+j]=image.Arr[i][j];}
	 }
 }
 return background;
}
