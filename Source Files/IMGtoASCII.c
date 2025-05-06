#include "Image_processing.h"
#include "Argument_handling.h"

//Kompatibilit�s: bmp (esetleges k�s�bbi fejleszt�sn�l: raw, png, jpg/jpeg)

int main(int argc, char* argv[]) {
	if (argc < 3) { printf("The program needs at last 3 arguments:\n'IMGtoASCII.exe' <path of image file> <name of text file>"); return 1; }
	int max_size = 10000000;
	debugmalloc_max_block_size(max_size); //max: 10MB

	//f�jlok megnyit�sa
	FILE* imgp, * txtp;
	BMP BMPimg; //ebbe a f�jlba lesznek elt�rolva a .bmp k�pf�jl adatai
	imgp = fopen(argv[1], "rb");
	if (imgp == NULL) { printf("There's no such file!"); return 2; }
	txtp = fopen(argv[2], "wb");
	if (imgp == NULL) { printf("File can not be created!"); fclose(imgp); return 2; }

	//header adatok kinyer�se a k�pb�l
	int return_code = readBMP(imgp, &BMPimg);
	
	//visszat�r�si k�dok kezel�se
	if (return_code == 1) {
		printf("Wrong format! The program only supports .bmp images.");
		fclose(imgp); fclose(txtp);
		return 3;
	}
	if (return_code == 2) {
		printf("Wrong format! The program does not support pictures with a palette.");
		fclose(imgp); fclose(txtp);
		return 3;
	}
	if (return_code == 3) {
		printf("Wrong format! The program only supports 24-bit images.");
		fclose(imgp); fclose(txtp);
		return 3;
	}
	if (BMPimg.size > max_size) {
		printf("Image too big!");
		fclose(imgp); fclose(txtp);
		return 4;
	}

	//.bmp bitmap-j�nek beolvas�sa
	unsigned char* prebitmap; //egy v�ltoz�, amibe a program egyt�l egyig beh�zza a bitmap r�szeit
	prebitmap = (unsigned char*)malloc(BMPimg.size);
	fseek(imgp, BMPimg.offset, SEEK_SET);
	fread(prebitmap, 1, BMPimg.size, imgp);

	//bitmap feltl�t�se: bej�r�sn�l a k�ls� ciklus ford�tva, mert bin�risan �gy helyezkednek el az adatok
	BMPimg.bitmap = (Pixel**)malloc(BMPimg.height * sizeof(Pixel*));
	if (BMPimg.bitmap == NULL) {
		printf("Image too big!");
		free(prebitmap);
		fclose(imgp); fclose(txtp);
		return 4;
	}
	int k = 0; //a prebitmap adataira ez az index hivatkozik
	for (int i = BMPimg.height - 1; i >= 0; i--) {
		BMPimg.bitmap[i] = (Pixel*)malloc(BMPimg.width * sizeof(Pixel));
		if (BMPimg.bitmap[i] == NULL) {
			printf("Image too big!");
			free(BMPimg.bitmap);
			free(prebitmap);
			fclose(imgp); fclose(txtp);
			return 4;
		}
		for (int j = 0; j < BMPimg.width; j++) {
			BMPimg.bitmap[i][j].blue = prebitmap[k];
			BMPimg.bitmap[i][j].green = prebitmap[k+1];
			BMPimg.bitmap[i][j].red = prebitmap[k+2];
			k += 3;
		}
		k+=(BMPimg.width%4); //annyi paddinget kell �tugranunk, amennyi a width-et 4-gyel oszthat�v� teszi (pl. 127 eset�n 1-et)
	}
	free(prebitmap);

	//argumentumok kezel�se
	char* ascii = NULL; //a setGamma f�ggv�ny mem�ri�t szabad�t fel, ez�rt el�tte vagy �rt�ket kell �ll�tani, vagy le kell foglalni valamennyi helyet
	int gamma = 0; //nincs t�lindexel�s, hiszen && ut�n nincs ki�rt�kelve a m�sodik, ha az els� hamis
	setGamma(&ascii, gamma);
	int compression = 1;
	int displayOnConsole = 0;
	if (argc > 3)
		if (handleArgument(argv[3], &ascii, &gamma, &compression, &displayOnConsole) == 1) {
			printf("Incorrect argument 4!"); terminate(&ascii, &BMPimg, &imgp, &txtp); return 1;
		}
	if (argc > 4)
		if (handleArgument(argv[4], &ascii, &gamma, &compression, &displayOnConsole) == 1) {
			printf("Incorrect argument 5!"); terminate(&ascii, &BMPimg, &imgp, &txtp); return 1;
		}
	if (argc > 5)
		if (handleArgument(argv[5], &ascii, &gamma, &compression, &displayOnConsole) == 1) {
			printf("Incorrect argument 6!"); terminate(&ascii, &BMPimg, &imgp, &txtp); return 1;
		}
	if (argc > 6)
		if (handleArgument(argv[6], &ascii, &gamma, &compression, &displayOnConsole) == 1) {
			printf("Incorrect argument 7!"); terminate(&ascii, &BMPimg, &imgp, &txtp); return 1;
		}
	if (argc > 7) {
		printf("Too many arguments!"); terminate(&ascii, &BMPimg, &imgp, &txtp); return 1;
	}
	if (BMPimg.height % compression != 0 || BMPimg.width % compression != 0) {
		printf("This compression can not be appied to this picture!");
		terminate(&ascii, &BMPimg, &imgp, &txtp);
		return 3;
	}

	//bitmap pixeleinek console-ra �s txt-be �r�sa
	int ascii_length = strlen(ascii);
	int index;
	for (int i = 0; i < BMPimg.height; i += compression) {
		for (int j = 0; j < BMPimg.width; j += compression) {
			index = compress(BMPimg.bitmap, i, j, compression, ascii_length);
			fwrite(&ascii[index], sizeof(char), 1, txtp);
			fwrite(&ascii[index], sizeof(char), 1, txtp); //minden karakterb�l 2 db, mert egy karakter 2x olyan magas, mint sz�les
			if (displayOnConsole == 1)
				printf("%c%c", ascii[index], ascii[index]);
		}
		fwrite("\n", sizeof(char), 1, txtp);
		if (displayOnConsole == 1)
			printf("\n");
	}

	printf("ASCII art generated successfully!");
	terminate(&ascii, &BMPimg, &imgp, &txtp);//mem�ria felszabad�t�sa
	return 0;
}