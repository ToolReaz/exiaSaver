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
extern struct ArrRGB openPBM_PPM(char *path);
extern struct ArrRGB ScreenSizePBM_PPM(struct ArrRGB image, int sizeX, int sizeY, int CSGX, int CSGY);
extern struct ArrRGB AppendImagePBM_PPM(struct ArrRGB background, struct ArrRGB image, int CSGX, int CSGY);
extern int printPBM_PPM(struct ArrRGB screen);
