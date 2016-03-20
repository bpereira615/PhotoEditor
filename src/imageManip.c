/* Ben Gellman and Benjamin Hoertnagl-Pereira
 * bgellma1 and bhoert1
 * 608-228-0671, 631-488-7197
 * Intermediate Programming Assignment 6
 * October 12, 2015
 */

#include <stdio.h>
#include <stdlib.h>
#include "../include/ppmIO.h"
#include <math.h>

#define PI 3.1415

/* Struct to represent the filter that will be used in the blur function
 * Contains:
 *  pointer to the filter in memory
 *  sigma value to be used in blur
 *  dimension as length/width of filter
 *  bound- the length from center to edge*/
typedef struct filterArray{
  double** filterPtr;
  double sigma;
  int dimension;
  int bound;
}Filter;


/*Function to crop an image
 * Takes paramets for the initial x and y positons, and final x and y positions to crop to
 * Returns a pointer to the cropped pixel array*/
Pixel* crop(Pixel* pixelArray, Image* img, int x1, int y1, int x2, int y2){

  if(x1 == -1 || x2 == -1 || y1 == -1 || y2 == -1){
    printf("Failed to crop image!\n");
    return pixelArray;
  }
  //Sets the new width and height of the image
  int newRows = y2-y1;
  int newCols = x2-x1;

  //Makes sure that only valid crop values are given by the user
  if(newRows < 0 || newCols < 0 || x1 < 0 || y1 < 0 || x2 < 0 || y2 < 0 || x1 > img->cols || x2 > img->cols || y1 > img->rows || y2 > img->rows){
    printf("Invalid crop parameters!\n");
    return pixelArray;
  }

  printf("Cropping region from (%d, %d) to (%d, %d)...\n", x1, y1, x2, y2);

  //Creates the new array of pixels that will store cropped image
  Pixel *newPixelArray; 
  newPixelArray  = malloc(sizeof(Pixel) * newRows * newCols);

  //Iterates through the cropped range of indicies in the original pixel array and sotres each color value to the new pixel at the given location in the cropped image
  int index = 0;
  for(int i = 0; i < newRows; i++){
    for(int j = x1; j < x2; j++){
      newPixelArray[index].r = pixelArray[(i + y1) * img->cols + j].r;
      newPixelArray[index].g = pixelArray[(i + y1) * img->cols + j].g;
      newPixelArray[index].b = pixelArray[(i + y1) * img->cols + j].b;
      index++;
    }
  }

  //Updates the image struct to suit the newly cropped image 
  img->rows = newRows;
  img->cols = newCols;

  //Frees the old pixel array from memory
  free(pixelArray);

  //Returns the cropped image array pointer
  return newPixelArray;
  

}


/*Funcition to invert the color values for each pixel*/ 
Pixel* invert(Pixel* pixelArray, Image* img){

    //Iterates through the array and sets the color values for each pixel to the inverted value
    for(int i = 0; i < (img->rows * img->cols); i++){
    pixelArray[i].r = 255 - pixelArray[i].r; 
    pixelArray[i].b= 255 - pixelArray[i].b;
    pixelArray[i].g = 255 - pixelArray[i].g;

  }
  
  //Returns the inverted image pointer 
  return pixelArray;
}

/*Function to swap the color levels in each pixel*/
Pixel* swap(Pixel* pixelArray, Image* img){


  //Temporary color levels for each pixel
  int red = 0, green = 0, blue = 0;
  
  //Iterates through the array and cycles through switching the values
  for(int i = 0; i < (img->rows * img->cols); i++){
    red = pixelArray[i].r;
    green = pixelArray[i].g;
    blue = pixelArray[i].b;
    pixelArray[i].r = green; 
    pixelArray[i].g= blue;
    pixelArray[i].b = red;

  } 

  //Returns the swapped image pointer
  return pixelArray;
}


Pixel* grayscale(Pixel *pixelArray, Image* img)
{

  double intensity;
  double roundedIntensity;
  //step through each pixel, get its corresponding grayscale value, assign
  //that value to all three color channels for each picture
  for (int i = 0; i < (img->rows * img->cols); i ++)
  {
    intensity = (0.30 * pixelArray[i].r) + (0.59 * pixelArray[i].g) + (0.11 * pixelArray[i].b);
    roundedIntensity = rint(intensity);
    pixelArray[i].r = (int) roundedIntensity;
    pixelArray[i].g = (int) roundedIntensity;
    pixelArray[i].b = (int) roundedIntensity;
  }
  //return the new grayscaled array of pixels
  return pixelArray;

}



/* Function to make the "filter" that is used to blur the array. Takes as input a
 * value for sigma and returns a pointer to a Filter struct containing, among other
 * things, a pointer to the actual filter array */
Filter* makeFilter(double sigma)
{
  //dynamically allocate the filter, init some values
  Filter* filter = malloc(sizeof(Filter));
  int dimension;
  int boundary;
  //get the size of the filter array from sigma
  double temp = sigma * 5;
  double roundedTemp = rint(temp);

  int boundX, boundY;
  if ((int) roundedTemp % 2 == 0)
  {
    roundedTemp = roundedTemp + 1;
  }
  dimension = (int) roundedTemp;
  boundary = (dimension - 1);
  
  boundX = -1 * boundary;
  boundY = -1 * boundary;
  
  //allocate space for the filter array
  double** filterVals = malloc((2 * dimension - 1) * sizeof(double*));
  for (int i = 0; i < (2 * dimension - 1); i ++)
  {
    filterVals[i] = malloc((2 * dimension - 1) * sizeof(double));
  }
  //put values in to the filter array
  for (int i = 0; i < (2 * dimension - 1); i ++)
  {
    boundX = -1 * boundary;
    for (int j = 0; j < (2 * dimension - 1); j ++)
    {
      //uses formula given by prof
       filterVals[i][j] = (1.0 / (2.0 * PI * pow(sigma, 2)) * exp( -(pow(boundX, 2) + pow(boundY, 2)) / (2 * pow(sigma, 2)))); 
       boundX ++;
    }
    boundY++;
  }

  //give filter struct fields the appropriate values
  filter->filterPtr = filterVals;
  filter->sigma = sigma;
  filter->dimension = dimension;
  filter->bound = boundary;
  return filter;
}

/* Function to convert one-dimensional array that was made from read image to a 
 * corresponding two-d array in order to make convolving easier
 */

Pixel** TwoDArray(Pixel* pixelArray, Image* img)
{
  //dynamically allocate the space for 2d array
  Pixel** twoD = malloc(img->rows * sizeof(Pixel*));
  for (int i = 0; i < img->rows; i ++)
  {
    twoD[i] = malloc(img->cols * sizeof(Pixel));
  }

  //fills the 2d array with the values from the original one-d array
  int index = 0;
  for(int i = 0; i < img->rows; i++){
    for(int j = 0; j < img->cols; j++){
      twoD[i][j] = pixelArray[index];
      index++;
    }
  }
  
  //return the pointer to the twoD array
  free(pixelArray);
  return twoD;
}

/* Function to convolve the previously made Gaussian filter with the previously 
 * made twoD array representing our image. Returns a 1D array representing the 
 * blurred image
 */

Pixel* convolve(Pixel** twoD, Image* img, Filter* filter)
{
 
  //init some values, allocate space for the array representing the blurred photo
  Pixel blurredPixel;
  double sumRed, sumGreen, sumBlue;
  Pixel* blurArray = malloc(img->rows * img->cols * sizeof(Pixel));
  

  //printf("%d\n", (pointerArray[3]));

  //populate blurArray with empty Pixel structs in preparation for populating
  //those strutcs with rgb values
  for (int i = 0; i < img->rows * img->cols; i ++)
  {
    blurArray[i] = blurredPixel;
  }
  //first two for loops step through the 2D image array to convolve with every pixel
  for (int i = 0; i < img->rows; i ++)
  {
    for (int j = 0; j < img->cols; j ++)
    {
      sumRed = 0;
      sumGreen = 0;
      sumBlue = 0;
      //second two for loops step through filter array to get the average values
      //for each pixel in the image
      for (int k = -1 * filter->bound; k <= filter->bound ; k ++)
      {
        for (int l = -1 * filter->bound; l <= filter->bound; l ++)
        {
          //this conditional statement deals with boundary cases -- ensures only
          //pixels actually in the image are included in convolution
          if((i + k) >= 0 && (i + k) < img->rows && (j + l) >= 0 && (j + l) < img->cols){
            //get weighted average of each color channel via convolution
            sumRed = sumRed + ((twoD[i + k][j + l]).r * filter->filterPtr[k + filter->bound][l + filter->bound]);
            sumGreen = sumGreen + ((twoD[i + k][j + l]).g * filter->filterPtr[k + filter->bound][l + filter->bound]);
            sumBlue = sumBlue + ((twoD[i + k][j + l]).b * filter->filterPtr[k + filter->bound][l + filter->bound]);
          }
        }
      }
      //assign each pixel in blurArray the weighted average pixel values made above
      blurArray[i * img->cols + j].r = sumRed;
      blurArray[i * img->cols + j].g = sumGreen;
      blurArray[i * img->cols + j].b = sumBlue;
    }

  }
  

  //frees the filter array and the filter struct from memory 
  for (int i = 0; i < (2 * filter->dimension - 1); i ++)
  {
    free(filter->filterPtr[i]);
  }
  free(filter->filterPtr);
  free(filter);


  //frees the two dimensional pixel array
  for (int i = 0; i < img->rows; i ++)
  {
    free(twoD[i]);
  }
  free(twoD);


  //return the blurred image in the form of a 1D array
  return blurArray;
}

/* Function to blur an image; sigma is input specified by the user*/

Pixel* blur(Pixel* pixelArray, Image * img, double sigma)
{
  //prevents bad sigma value from passing
  if(sigma <= 0){
    printf("Invalid value for sigma!\n");
    return pixelArray;
  }

  //initialize filter struct
  Filter* filter;
  //get 2D version of image
  Pixel** pointerArray = TwoDArray(pixelArray, img);
  //get the filter
  filter = makeFilter(sigma);
  //convolve filter with image, get blurred image
  Pixel* blurArray = convolve(pointerArray, img, filter);
  //return blurred image
  return blurArray;
  

}

/* Function to sharpen image by reversing the blur operation */

Pixel* sharpen(Pixel* pixelArray, Image* img, double sigma, double intensity)
{
  //prevents bad sigma or intensity values from passing
  if(sigma <= 0 || intensity <= 0){
    printf("Invalid value for sigma AND/OR intensity!\n");
    return pixelArray;
  }

  //blur image using the blur function
  Pixel* blurredImg = blur(pixelArray, img, sigma);
  //allocate room for sharpened image
  Pixel* sharpenedImg = malloc(img->rows * img->cols * sizeof(Pixel));

  //init some terms for math to sharpen
  Pixel tempPix;
  int redDiff;
  int greenDiff;
  int blueDiff;
  double redInt;
  double greenInt;
  double blueInt;
  double newRed;
  double newGreen;
  double newBlue;
  //step through each pixel in the original, blurred, and sharpened array and do...
  for (int i = 0; i < (img->rows * img->cols); i ++)
  {
    //populate sharpened image array with empty pixel structs to be filled with
    //color values later
    sharpenedImg[i] = tempPix;
    //get diff between orig and blurred for each color
    redDiff = pixelArray[i].r - blurredImg[i].r;
    greenDiff = pixelArray[i].g - blurredImg[i].g;
    blueDiff = pixelArray[i].b - blurredImg[i].b;
    //multiply that difference by user-supplied intensity for each color
    redInt = redDiff * intensity;
    greenInt = greenDiff * intensity;
    blueInt = blueDiff * intensity;
    //round those products to be integer values (technically doubles without numbers
    //past the decimal place)
    redInt = rint(redInt);
    greenInt = rint(greenInt);
    blueInt = rint(blueInt);
    //get new pixel values by adding the diff * intensity val to orig pixel val for 
    //each color
    newRed = redInt + pixelArray[i].r;
    newGreen = greenInt + pixelArray[i].g;
    newBlue = blueInt + pixelArray[i].b;
    //if colors are overflowed (above 255) or underflowed (below 0), saturate (set at
    //255) or desaturate (set at 0) respectively so pixels can get numbers that are
    //in the range of unsigned char for all colors
    if (newRed > 255)
      newRed = 255;
    if (newRed < 0)
      newRed = 0;
    if (newGreen > 255)
      newGreen = 255;
    if (newGreen < 0)
      newGreen = 0;
    if (newBlue > 255)
      newBlue = 255;
    if (newBlue < 0)
      newBlue = 0;
    //put new pixel in sharpened image array at corresponding index
    sharpenedImg[i].r = (unsigned char) newRed;
    sharpenedImg[i].g = (unsigned char) newGreen;
    sharpenedImg[i].b = (unsigned char) newBlue;
  }
  free(blurredImg);
  //return the sharpened image
  return sharpenedImg;
}

/* Function to increase or decrease brightness by factor, a term that is specified
 * by the user
 */
Pixel* brightness(Pixel* pixelArray, Image* img, int factor)
{

  //init new pixel values
  int newRed;
  int newGreen;
  int newBlue;
  //step through each pixel in pixelArray and...  Pixel** pointerArray = TwoDArray(pixelArray, img);
  for (int i = 0; i < (img->rows * img->cols); i ++)
  {
    //get new pixel value by adding current pixel value to factor for each color
    newRed = pixelArray[i].r + factor;
    newGreen = pixelArray[i].g + factor;
    newBlue = pixelArray[i].b + factor;
    //if colors are overflowed (above 255) or underflowed (below 0), saturate (set at
    //255) or desaturate (set at 0) respectively so pixels can get numbers that are
    //in the range of unsigned char for all colors
    if (newRed > 255)
      newRed = 255;
    if (newRed < 0)
      newRed = 0;
    if (newGreen > 255)
      newGreen = 255;
    if (newGreen < 0)
      newGreen = 0;
    if (newBlue > 255)
      newBlue = 255;
    if (newBlue < 0)
      newBlue = 0;
    //assign pixel values to new brightened (or darkened) value for each color
    pixelArray[i].r = newRed;
    pixelArray[i].g = newGreen;
    pixelArray[i].b = newBlue;
  }
  //return pixelArray
  return pixelArray;
}


Pixel* contrast(Pixel* pixelArray, Image* img, double factor){

  //prevents bad factor amount from passing
  if(factor <= 0){
    printf("Invalid contrast amount!\n");
    return pixelArray;
  }

  //init new pixel values
  int newRed, newGreen, newBlue; 
  double conRed, conGreen, conBlue;

  //step through each pixel in pixelArray and...
  for (int i = 0; i < (img->rows * img->cols); i ++)
  {
    //store the current pixel color levels from original image
    newRed = pixelArray[i].r;
    newGreen = pixelArray[i].g;
    newBlue = pixelArray[i].b;

    //convert the color levels into even range [-0.5, 0.5] and multiply,
    //by the contrast factor
    conRed = ((newRed/255.0) - 0.5) * factor;
    conGreen = ((newGreen/255.0) - 0.5) * factor;
    conBlue = ((newBlue/255.0) - 0.5) * factor;

    //convert the newly contrasted color levels back to [0-255] range
    newRed = (int) ((conRed + 0.5) * 255);
    newGreen = (int) ((conGreen + 0.5) * 255);
    newBlue = (int) ((conBlue + 0.5) * 255);


    
    //if colors are overflowed (above 255) or underflowed (below 0), saturate (set at
    //255) or desaturate (set at 0) respectively so pixels can get numbers that are
    //in the range of unsigned char for all colors
    if (newRed > 255)
      newRed = 255;
    if (newRed < 0)
      newRed = 0;
    if (newGreen > 255)
      newGreen = 255;
    if (newGreen < 0)
      newGreen = 0;
    if (newBlue > 255)
      newBlue = 255;
    if (newBlue < 0)
      newBlue = 0;

    //assign pixel values to new contrasted value for each color
    pixelArray[i].r = newRed;
    pixelArray[i].g = newGreen;
    pixelArray[i].b = newBlue;
  }
  
  
  return pixelArray;
}

//Function to produce the mirror image (over the line running vertically down the 
//center of the image) of an image

Pixel* mirrorImage(Pixel* pixelArray, Image* img)
{
  //allocate space for the new mirror image
  Pixel* mirror = malloc(img->rows * img->cols * sizeof(Pixel));

 /* Pixel** twoD = TwoDArray(pixelArray, img);

  int index = 0;
  for(int i = 0; i < img->rows; i++){
    for(int j = 0; j < img->cols; j++){
      mirror[index] = twoD[img->rows - i][img->cols - j];
      index++;
    }
  }

  //frees the two dimensional pixel array
  for (int i = 0; i < img->rows; i ++)
  {
    free(twoD[i]);
  }
  free(twoD);*/
  //step through each row in pixelArray
  for (int i = 0; i < img->rows; i ++)
  {
    //reflect the pixels over the center of the row
    for (int j = 0; j < img->cols; j ++)
    {
      mirror[(i * img->cols) + j] = pixelArray[((i + 1) * (img->cols) - 1) - j];
    }
  }
  //free pixelArray
  free(pixelArray);
  //return the array representing the mirrored image
  return mirror;

}

//This function toggles rotating the image. Calling the function once rotates the 
//image 90 degrees to the left, and calling the function again returns the image
//to an upright position

Pixel* rotate(Pixel* pixelArray, Image* img)
{
  //get new Image struct for rotated image, populate its fields with appropriate 
  //vals including switched row and column vals from the original image
  Pixel rotPixel;
  Image* rotImgPtr = malloc(sizeof(Image));
  rotImgPtr->rows = img->cols;
  rotImgPtr->cols = img->rows;
  rotImgPtr->ppm = img->ppm;
  rotImgPtr->startBinary = img->startBinary;
  rotImgPtr->colors = img->colors;
  //get a two dimensional array representation of the original image
  Pixel** orig = TwoDArray(pixelArray, img);

  //dynamically allocate the space for new 2d array for rotated image
  Pixel** rot = malloc(rotImgPtr->rows * sizeof(Pixel*));
  for (int i = 0; i < rotImgPtr->rows; i ++)
  {
    rot[i] = malloc(rotImgPtr->cols * sizeof(Pixel));
  }
  
  //populate the rotated 2d array with the corresponding pixels from the orig 2d array
  for (int i = 0; i < img->rows; i ++)
  {
    for (int j = 0; j < img->cols; j ++)
    {
      rot[j][i] = orig[i][j];
    }
  }
  
  //allocate space for a 1d array representation of rotated 2d array image
  Pixel* rotArray = malloc(rotImgPtr->rows * rotImgPtr->cols * sizeof(Pixel));
  for (int i = 0; i < (rotImgPtr->rows * rotImgPtr->cols); i ++)
  {
    rotArray[i] = rotPixel;
  }
  //populate that 1d array with the rotated image
  for (int i = 0; i < rotImgPtr->rows; i ++)
  {
    for (int j = 0; j < rotImgPtr->cols; j ++)
    {
      rotArray[(i * rotImgPtr->cols) + j] = rot[i][j];
    }
  }
  //free the original 2d image
  for (int i = 0; i < img->rows; i ++)
  {
    free(orig[i]);
  }
  free(orig);
  //change the Image struct representing this image's rows and columns to the new
  //values that they should have considering the image has been rotated
  img->rows = rotImgPtr->rows;
  img->cols = rotImgPtr->cols;

  //free the 2d array representing the rotated image
  for (int i = 0; i < rotImgPtr->rows; i ++)
  {
    free(rot[i]);
  }
  free(rot);
  
  //free the memory used by the temp rotated image struct

  free(rotImgPtr);

  //return the rotated image as a 1d array
  return rotArray;
}

