#ifndef Image_processing_H
#define Image_processing_H

#include <string.h>
#include <stdlib.h>
#include <math.h>
//esetleges késõbbi fejlesztésnél: <windows.h>
#include "debugmalloc.h"

//#pragma warning ( disable: 4996 ) //így a Visual Studio nem tekinti problémának a scanf visszatérési értékének nem kezelését - más IDE-hez nem kell
//#pragma pack(1) //- a compiler nem rak space-eket az atribútumok közé - jelenleg szükségtelen

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

//függvény deklarációk
int readBMP(FILE* imgp, BMP* BMPimg);
int calculate_index(Pixel p, int ascii_length);
void setGamma(char** ascii_pointer, int gamma);
int compress(Pixel** bitmap, int cur_i, int cur_j, int compression, int ascii_length);

#endif