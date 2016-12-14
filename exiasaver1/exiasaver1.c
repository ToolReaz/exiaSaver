#include "PBM.h"
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
//#include <sys/mman.h>

static struct ArrRGB image;
int main(int argc, char *argv[]){
	if(argc==1){
		image=openPBM_PPM(argv[0]);
		/*int pipeInit[2];
		pipe(pipeInit);*/
		if(fork()==0){
			struct ArrRGB sized = ScreenSizePBM_PPM(image, 80, 24, 40-image.x/2, 12-image.y/2);
			printPBM_PPM(sized);
			//printf("opened : %s\n", argv[0]);
			/*struct ArrRGB*/ 
			//printf("hello");
			/*close(pipeInit[0]);
			write(pipeInit[1], &image, sizeof(struct ArrRGB));
			close(pipeInit[1]);*/
			//printf("opened %d %d\n", image.x, image.y);
		}else{
			//printf("waiting\n");
			wait(0);
			/*close(pipeInit[1]);*/
			//struct ArrRGB image;
			//printf("hello\n");
			/*read(pipeInit[0], &image, sizeof(struct ArrRGB));
			//printf("hello : x=%d, y=%d\n", image.x, image.y);
			close(pipeInit[0]);*/
			//printf("Ended waiting %d %d\n", image.x, image.y);
			//printf("--%d>%d>%d>%d--\n", 80, 23, 40-image.x/2, 23-image.y/2);
			//printf("helo  \n");
			//printf("heu...\n");
			//printf("ScreenSized\n");
			system("bash -c \"read -n1 -r\"&&clear");
		}
		return 0;
	}
	return 1;
}
