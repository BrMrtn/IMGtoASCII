#ifndef Image_processing_H
#define Image_processing_H

#include <string.h>
#include <stdlib.h>
#include <math.h>
//esetleges k�s�bbi fejleszt�sn�l: <windows.h>
#include "debugmalloc.h"

//#pragma warning ( disable: 4996 ) //�gy a Visual Studio nem tekinti probl�m�nak a scanf visszat�r�si �rt�k�nek nem kezel�s�t - m�s IDE-hez nem kell
//#pragma pack(1) //- a compiler nem rak space-eket az atrib�tumok k�z� - jelenleg sz�ks�gtelen

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

//f�ggv�ny deklar�ci�k
int readBMP(FILE* imgp, BMP* BMPimg);
int calculate_index(Pixel p, int ascii_length);
void setGamma(char** ascii_pointer, int gamma);
int compress(Pixel** bitmap, int cur_i, int cur_j, int compression, int ascii_length);

#endif