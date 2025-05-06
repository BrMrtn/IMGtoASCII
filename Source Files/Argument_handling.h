#ifndef Argument_handling_H
#define Argument_handling_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debugmalloc.h"

#ifndef Structs
#define Structs
//Egy pixel struktúrája (red, green, blue értékek 0 és 255 között)
typedef struct Pixel {
	unsigned char red;
	unsigned char green;
	unsigned char blue;
} Pixel;

//BMP kepfájl struktúrája
typedef struct BMP {
	unsigned short signature; //offset: 0
	unsigned int offset; //offset: 10
	unsigned int width; //offset: 18
	unsigned int height; //offset: 22
	unsigned short bits_per_pixel; //offset: 28
	unsigned int size; //offset: 34 lenne, de a program több képet kezelhet, ha a szélesség és magasság adatokból számoljuk ki

	Pixel** bitmap; //offset: offset (54)
	//bitmap[][] - elsõ index a pixel helye (5x5-ös képnél bitmap[5] a 2. sor 1. indexe), második index lehet: 0, 1, 2 (r, g, b)
} BMP;
#endif

int handleArgument(char* arg, char** ascii_p, int* gamma_p, int* compression_p, int* displayOnConsole);
void terminate(char** ascii_p, BMP* BMPimg_p, FILE** imgp_p, FILE** txtp_p);
void setGamma(char** ascii_pointer, int gamma); //mert a handleArgument függvény használja a setGamma-t

#endif