#ifndef Argument_handling_H
#define Argument_handling_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debugmalloc.h"

#ifndef Structs
#define Structs
//Egy pixel strukt�r�ja (red, green, blue �rt�kek 0 �s 255 k�z�tt)
typedef struct Pixel {
	unsigned char red;
	unsigned char green;
	unsigned char blue;
} Pixel;

//BMP kepf�jl strukt�r�ja
typedef struct BMP {
	unsigned short signature; //offset: 0
	unsigned int offset; //offset: 10
	unsigned int width; //offset: 18
	unsigned int height; //offset: 22
	unsigned short bits_per_pixel; //offset: 28
	unsigned int size; //offset: 34 lenne, de a program t�bb k�pet kezelhet, ha a sz�less�g �s magass�g adatokb�l sz�moljuk ki

	Pixel** bitmap; //offset: offset (54)
	//bitmap[][] - els� index a pixel helye (5x5-�s k�pn�l bitmap[5] a 2. sor 1. indexe), m�sodik index lehet: 0, 1, 2 (r, g, b)
} BMP;
#endif

int handleArgument(char* arg, char** ascii_p, int* gamma_p, int* compression_p, int* displayOnConsole);
void terminate(char** ascii_p, BMP* BMPimg_p, FILE** imgp_p, FILE** txtp_p);
void setGamma(char** ascii_pointer, int gamma); //mert a handleArgument f�ggv�ny haszn�lja a setGamma-t

#endif