/* Ben Gellman and Benjamin Hoertnagl-Pereira
 * bgellma1 and bhoert1
 * 608-228-0671, 631-488-7197
 * Intermediate Programming Assignment 6
 * October 12, 2015
 */


#ifndef _BEN120_MENUUTIL_H
#define _BEN120_MENUUTIL_H


/*Function to print the menu options nicely formatted*/
void printMenu();

/*Function to collect input from the user, assign the input characters to an array.
 * Returns the pointer to that array
 */
char* collectInput();


/* Function to interpret user's input after the input is "valid" (has one or two chars). After 
 * interpreting the input, this function calls the function to perform the desired task of reading,
 * writing, or image manipulation. The function then returns the address to the array of Pixel
 * structs that is the result of the read, write, or manipulation.
 * Takes as argument the pointer to the input array, a pointer to the current image struct, and a 
 * pointer to the current Pixel array to operate on (could be NULL if first reading image, eg)
 * Returns 1 if input is one of the options, and zero otherwise.
 */
Pixel* interpretChoice(char* choice, Image *currentImage, Pixel *pixelArray);


/* After the user has entered r for read or w for write, this function collects the remaining input
 * (allegedly the file to read from or write to) and returns a pointer to the array into which it
 * placed that input to be used to read or write the file.*/

/*Function that gets the file name to be read or written as a string*/
char* collectRemainingInput();

/*Function that gets  the integer parameters needed for cropping the image**/
int* collectIntegerInput();

/*Function that drives the program, run in hw6.c*/
int editPhotos();

/*Function that gets a double from the user, to be used in blur,sharpen, or contrast*/
double collectSigma();
#endif
