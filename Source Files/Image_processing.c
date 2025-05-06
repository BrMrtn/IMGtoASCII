#include "Image_processing.h"

//header adatok kinyer�se a k�pb�l, kiv�telek kezel�se
int readBMP(FILE* imgp, BMP* BMPimg) {
	fseek(imgp, 0, SEEK_SET);
	fread(&BMPimg->signature, 2, 1, imgp);
	if (BMPimg->signature != 19778)
		return 1;
	fseek(imgp, 10, SEEK_SET);
	fread(&BMPimg->offset, 4, 1, imgp);
	if (BMPimg->offset != 54)
		return 2;
	fseek(imgp, 18, SEEK_SET);
	fread(&BMPimg->width, 4, 1, imgp);
	fseek(imgp, 22, SEEK_SET);
	fread(&BMPimg->height, 4, 1, imgp);
	fseek(imgp, 28, SEEK_SET);
	fread(&BMPimg->bits_per_pixel, 2, 1, imgp);
	if (BMPimg->bits_per_pixel != 24)
		return 3;
	fseek(imgp, 34, SEEK_SET);
	BMPimg->size = (BMPimg->width * BMPimg->height * 3 + BMPimg->height * 2); //minden pixel 3 byte, �s minden sor v�g�n van k�t byte-nyi padding
}

// egy pixel r, g �s b �rt�keib�l egy sz�rke�rnyalatos �rt�ket sz�mol, ami alapj�n visszaadja az ascii t�mbben l�v�, a pixelt legjobban reprezent�l� ascii karakter index�t
int calculate_index(Pixel p, int ascii_length) {
	double grayscale_value = p.red * 0.2126 + p.green * 0.7152 + p.blue * 0.0722;
	int index = floor(grayscale_value / 255 * ascii_length);
	return index;
}

//a gamma �rt�k alapj�n �t�rja az ascii t�mb�t, hogy a program nagyobb val�sz�n�s�ggel v�lasszon vil�gosabb/s�t�tebb pixeleket
void setGamma(char** ascii_pointer, int gamma) {
	char* ascii = *ascii_pointer;
	char* ascii_sample = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'.  "; //ascii karakterlista s�t�t -> vil�gos, alapeset: 71 db, a v�g�n 2 space, mert a floor miatt az utols� karakter sosem v�laszt�dna ki

	int length = strlen(ascii_sample);
	int gamma_size = abs(gamma);
	char* result = (char*)malloc((length + gamma_size + 1) * sizeof(char));
	char* add = (char*)malloc((gamma_size + 1) * sizeof(char));

	if (gamma > 0) { //ha gamma pozit�v, a program "vil�gosabb� teszi a k�pet" (spaceket f�z az ascii_sample t�mb v�g�re)
		for (int i = 0; i < gamma_size; i++)
			add[i] = ascii[length - 1];
		add[gamma_size] = '\0';
		strcpy(result, ascii);
		strcat(result, add);
	}
	else if (gamma < 0) { //ha gamma negat�v, a program "s�t�tebb� teszi a k�pet" ($ jeleket f�z az ascii_sample t�mb elej�re)
		for (int i = 0; i < gamma_size; i++)
			add[i] = ascii[0];
		add[gamma_size] = '\0';
		strcpy(result, add);
		strcat(result, ascii);

	}
	else strcpy(result, ascii_sample);

	free(add); free(ascii);
	*ascii_pointer = result;
}

//egy pixel feldolgoz�sa helyett compression^2 db pixel index�t �tlagolja, �gy kisebb� t�ve a k�pet
int compress(Pixel** bitmap, int cur_i, int cur_j, int compression, int ascii_length) {
	int sum = 0;
	for (int i = 0; i < compression; i++) {
		for (int j = 0; j < compression; j++) {
			Pixel a = bitmap[cur_i + i][cur_j + j];
			sum += calculate_index(bitmap[cur_i + i][cur_j + j], ascii_length);
		}
	}
	return sum / (compression * compression);
}