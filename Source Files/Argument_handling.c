#include "Argument_handling.h"

//egy extra argumentum 4 lehetséges típusa alapján beállítja a szükséges értékeket
int handleArgument(char* arg, char** ascii_p, int* gamma_p, int* compression_p, int* displayOnConsole) {
	switch (arg[0]) {
	case 'i':
		if (strlen(arg) == 1) {
			strrev(*ascii_p);
			return 0;
		}
		else return 1;
		break;
	case 'g':
		if (sscanf(arg, "g%d", gamma_p) == 1) setGamma(ascii_p, *gamma_p);
		else return 1;
		break;
	case 'c':
		sscanf(arg, "c%d", compression_p);
		if (*compression_p > 0) return 0;
		else *compression_p = 1;
		break;
	case 'd':
		*displayOnConsole = 1; return 0;
		break;
	default:
		return 1;
		break;
	}
}

//felszabadítja a memóriát és lezárja a megnyitott file-okat
void terminate(char** ascii_p, BMP* BMPimg_p, FILE** imgp_p, FILE** txtp_p) {
	free(*ascii_p);
	for (int i = 0; i < BMPimg_p->height; i++)
		free(BMPimg_p->bitmap[i]);
	free(BMPimg_p->bitmap);
	fclose(*imgp_p); fclose(*txtp_p);
}