/* Ben Gellman and Benjamin Hoertnagl-Pereira
 * bgellma1 and bhoert1
 * 608-228-0671, 631-488-7197
 * Intermediate Programming Assignment 6
 * October 12, 2015
 */


#ifndef _BEN120_PPMIO_H
#define _BEN120_PPMIO_H

#include <stdbool.h>

/*Struct to represent each pixel of the image, with its 3 distict color levels*/
typedef struct pixel{
  unsigned char r;
  unsigned char g;
  unsigned char b;
} Pixel;


/*Struct to represent image
 * Contains:
 * dimenions as rows and columns
 * ppm- boolean reperesenting if image is a .ppm file
 * startBinary- starting position to read binary information
 * color value, usually 255*/
typedef struct image{
  int rows;
  int cols;
  bool ppm;
  long startBinary;
  int colors;
  //FILE* address;
} Image;

/*Function to read image from a given file, specified by the character array filename[]
 * Loads information into the passed Image struct pointer to be used for future functions*/
Pixel* readImage(const char filename[], Image *img);

/*Function to check if the given image file is .ppm, returns false if not*/
int isPPM(FILE * fp, Image * currentImg);

/*Function to write manipulated file from pixelArray format to a new file, specified by filename
 * Uses data fields of Image struct to write dimension and color value info*/
int writePPM(const Pixel *pixelArray, const char fileName[], const Image * image);


#endif
