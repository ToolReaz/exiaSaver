#include "PBM.h"
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

static struct ArrRGB image;
int main(int argc, char *argv[]){
	if(argc==1){
		image=openPBM_PPM(argv[0]);
		if(fork()==0){
			struct ArrRGB sized = ScreenSizePBM_PPM(image, 24, 80, 12-image.x/2, 40-image.y/2);
			printPBM_PPM(sized);
		}else{
			wait(0);
			system("bash -c \"read -n1 -r\"&&clear");
		}
		return 0;
	}
	return 1;
}
