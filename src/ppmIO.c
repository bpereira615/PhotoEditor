/* Ben Gellman and Benjamin Hoertnagl-Pereira
 * bgellma1 and bhoert1
 * 608-228-0671, 631-488-7197
 * Intermediate Programming Assignment 6
 * October 12, 2015
 */


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>
#include "../include/ppmIO.h"


int isPPM(FILE * fp, Image * currentImg);

Pixel* readImage(const char filename[], Image * img)
{
  printf("Reading from %s ...\n", filename);
  //Creates file pointer to read input file
  FILE *fp = fopen(filename, "rb");

  //Exits safely if the file does not exist
  if(!fp){
    printf("The image could not be loaded, invalid path!\n");
    return NULL;
  }
  
  //Checks if the given file is a .ppm file
  int success = isPPM(fp, img);
  if (success == 0)
  {
    printf("Not a ppm file");
  }


  //Creates an array to hold each pixel of the image
  Pixel *pixelArray = malloc(img->rows * img->cols * sizeof(Pixel));

  
  //Moves the cursor to the beginning of the binary
  fseek(fp, img->startBinary, SEEK_SET);

  //Reads in each pixel to the array of pixels representing the image
  fread(pixelArray, sizeof(Pixel), (img->rows * img->cols), fp);
   
  //Closes the poiter to the read file 
  fclose(fp); 
  
  if (img->ppm)
  {
    //Returns the pointer to the array of pixels
    return pixelArray;
  }
  else
  {
    //Returns null, the image is not in .ppm format
    return NULL;
  }

}

int  writePPM(const Pixel *pixelArray, const char fileName[], const Image * image)
{
  //Creates a file pointer for the resulting output file
  printf("Writing to %s ...\n", fileName);
  FILE *rp = fopen(fileName, "wb");

  //Exits with error if the file pointer is unable to be created
  if (!rp)
  {
    return -1;
  }

  //Writes the PPM header information to the output file, using the rows and columns from the image struct
  fprintf(rp, "P6\n");
  fprintf(rp, "%d %d\n",image->cols, image->rows);
  fprintf(rp,"%d\n", image->colors);

  //Writes the binary data from the pixel array to the output file
  fwrite(pixelArray, sizeof(Pixel), (image->rows * image->cols), rp);
  fclose(rp);

  //Sucessful exit
  return 1;
}



int isPPM(FILE * fp, Image * currentImg)
{

//  currentImg->address = fp;

  //Store the first two characters of the PPM header
  char first;
  char second;

  fscanf(fp, "%c%c", &first, &second);
  if (first != 'P' || second != '6')
  {  
    //This file is not a PPM file
    return 0;
  }

  //If the first two characters are P6 then this is a PPM file
  currentImg->ppm = true;
 
  
  
  //scan over newline character after the opening P6
  fscanf(fp, "%c", &first);
  while (isspace(first))
  {
    fscanf(fp, "%c", &first);
  }
  //check if there is a comment on the next line, if so scan over whole line
  while(first == '#')
  {
    if (first == '#')
    {
     while (first != '\n')
     {
       fscanf(fp, "%c", &first);
      }
    }
    fscanf(fp, "%c", &first);
  }

  //moves file reader cursor back one character
  fseek(fp, -sizeof(char), SEEK_CUR);

  //declares rows and columns, get values from file and saves to struct
  int cols;
  int rows;
  fscanf(fp, "%d %d", &cols, &rows);
  currentImg->cols = cols;
  currentImg->rows = rows;

  //printf("Rows- %d, Cols- %d\n", rows, cols);
  //scan over newline character after the number of rows and columns
  fscanf(fp, "%c", &first);

  //check if there is a comment on the next line, if so scan over whole line
  fscanf(fp, "%c", &first);
  while (isspace(first))
  {
    fscanf(fp, "%c", &first);
  }
  while(first == '#')
  {
    if (first == '#')
    {
     while(first != '\n')
      {
        fscanf(fp, "%c", &first);
      }
    }
    //Checks if there is a new comment starting on the new line
    fscanf(fp, "%c", &first);
  }


  //Moves the cursor back one character for proper reading
  fseek(fp, -sizeof(char), SEEK_CUR);
  
  //Reads and sets the color value for the PPM image
  int colors;
  fscanf(fp, "%d", &colors);
  currentImg->colors = colors;

  //get last newline after number of colors before binary
  fscanf(fp, "%c", &first);

  //Sets the poisition from which the binary data of file begins
  currentImg->startBinary = ftell(fp);


  return 1;
  

  
}
