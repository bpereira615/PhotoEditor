/* Ben Gellman and Benjamin Hoertnagl-Pereira
 * bgellma1 and bhoert1
 * 608-228-0671, 631-488-7197
 * Intermediate Programming Assignment 6
 * October 12, 2015
 */

#ifndef _BEN120_IMAGEMANIP_H
#define _BEN120_IMAGEMANIP_H

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
Pixel* crop(Pixel* pixelArray, Image* img, int x1, int y1, int x2, int y2);

/*Funcition to invert the color values for each pixel*/
Pixel* invert(Pixel* pixelArray, Image* img);

/*Function to swap the color levels in each pixel*/
Pixel* swap(Pixel* pixelArray, Image* img);

/* Function that converts a photo to grayscale using the NTSC standard
 * conversion formula. Takes as input a Pixel array pointer and an Image
 * struct pointer. Returns the pixelArray that is the updated, grayscaled
 * photo.
 */
Pixel* grayscale(Pixel *pixelArray, Image* img);

/* Function to make the "filter" that is used to blur the array. Takes as input a
 *  * value for sigma and returns a pointer to a Filter struct containing, among other
 *   * things, a pointer to the actual filter array */
Filter* makeFilter(double sigma);

/* Function to convert one-dimensional array that was made from read image to a 
 * corresponding two-d array in order to make convolving easier
 */
Pixel** TwoDArray(Pixel* pixelArray, Image* img);

/* Function to convolve the previously made Gaussian filter with the previously 
 * made twoD array representing our image. Returns a 1D array representing the 
 * blurred image
 */
Pixel* convolve(Pixel** twoD, Image* img, Filter* filter);


/* Function to blur an image; sigma is input specified by the user*/
Pixel* blur(Pixel* pixelArray, Image* img, double sigma);

/*Function to sharpen image by reversing the blur operation*/
Pixel* sharpen(Pixel* pixelArray, Image* img, double sigma, double intensity);

/* Function to increase or decrease brightness by factor, a term that is 
 * specified by the user
 */
Pixel* brightness(Pixel* pixelArray, Image* img, int factor);

/* Function to incread or decrease the contrast of the image by a factor, a 
 * term that is specified by the user
 */
Pixel* contrast(Pixel* pixelArray, Image* img, double factor);

//Function to produce the mirror image (over the line running vertically
//down the center of the image) of an image
Pixel* mirrorImage(Pixel* pixelArray, Image* img);

//This function toggles rotating the image. calling the function once
//rotates the image 90 degrees to the left, and calling the function again
//retunrs the image to an upright position
Pixel* rotate(Pixel* pixelArray, Image* img);
#endif
