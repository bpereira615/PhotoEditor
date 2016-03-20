/* Ben Gellman and Benjamin Hoertnagl-Pereira
 * bgellma1 and bhoert1
 * 608-228-0671, 631-488-7197
 * Intermediate Programming Assignment 6
 * October 12, 2015
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/ppmIO.h"
#include "../include/imageManip.h"

//defines the maximum string length for name of file
#define MAX_FILENAME_SIZE 100


/*Function to print the menu options nicely formatted*/

void printMenu()
{
  printf("\nMain menu: \n");
  printf("\tr <filename> - read image from <filename>\n");
  printf("\tw <filename> - write image to <filename>\n");
  printf("\tc <x1> <y1> <x2> <y2> - crop image to the box with the given corners\n");
  printf("\ti - invert intensities\n");
  printf("\ts - swap color channels\n");
  printf("\tg - convert to grayscale\n");
  printf("\tbl <sigma> - Gaussian blur with the given radius (sigma)\n");
  printf("\tsh <sigma> <amt> - sharpen by given amount (intensity), with radius (sigma)\n");
  printf("\tbr <amt> - change brightness (up or down) by the given amount\n");
  printf("\tcn <amt> - change contrast (up or down) by the given amount\n");
  printf("\tm - mirror the image horizontally\n");
  printf("\tro - rotate the image 90 degrees to the left\n");
  printf("\tq - quit\n");
  printf("Enter choice: ");
}
/*Function to collect input from the user, assign the input characters to an array.
 * Returns the pointer to that array
 */

char* collectInput()
{
  //init array and pointer
  //char input[2];
  char *input = malloc(3 * sizeof(char));
  //printf("input: %c\n", input);
  //char *input = inputPtr;
  char c;
  //get first character
  scanf("%c", &c);

  //while the first character is a space, keep getting characters (skip whitespace)
  while (isspace(c))
  {
    scanf("%c", &c);
  }

  //assign first non-whitespace character to index 0 of the input array
  input[0] = c;
  //get the next char
  scanf("%c", &c); 
  //if next char is a space, give first index escape character and return pointer to 
  //array
  if (isspace(c))
  {
    input[1] = '\0';
    //inputPtr = input;
    ungetc(c, stdin); 
    return input;
  }
  //if next char is a nonspace char, add that char to index 1 of the array
  else
  {
    input[1] = c;
    //get next char (should be whitespace or input is definitely invalid)
    scanf("%c", &c); 
    //if next char is space, return the pointer to the array
    if (isspace(c))
    {
      input[2] = '\0';
      //inputPtr = input;
      //fseek(stdin, -sizeof(char), SEEK_CUR);
      ungetc(c, stdin);
      return input;
    }
    //input is definitely invalid, start input gathering process over
    else
    {
      printf("Invalid choice. Displaying menu again\n");
      return input;
    }
  }
}

int moveCursor(){
  while(getchar() != '\n');
  return 1;
}
/*Function to collect necessary image boundaries if the crop manipulation is chosen*/
int* collectIntegerInput(){

  int *input = malloc(4 * sizeof(int));
  for(int i = 0; i < 4; i++){
    input[i] = -1;
  }

  scanf("%d %d %d %d", &input[0], &input[1], &input[2], &input[3]);
  return input;
}



/*Function to collect double information for the blur*/
double collectDouble(){
  //double value, either sigma for blur/sharpen or contrast value
  double value;
  scanf("%lf", &value);
  return value;
}





/* After the user has entered r for read or w for write, this function collects the remaining input
 * (allegedly the file to read from or write to) and returns a pointer to the array into which it
 * placed that input to be used to read or write the file.*/

char* collectRemainingInput()
{
  char c;
  int i = 0;
  char* inputArray = malloc(MAX_FILENAME_SIZE * sizeof(char));
  scanf("%c", &c);
  while(isspace(c))
  {
    scanf("%c", &c);
  }
  inputArray[i] = c;
  while (c != '\n' && i < MAX_FILENAME_SIZE - 1)
  {
    scanf("%c", &c);
    i = i + 1;
    inputArray[i] = c;
  }
  if (inputArray[i] == '\n')
  {
Pixel* mirrorImage(Pixel* pixelArray, Image* img);

    inputArray[i] = '\0';
  }
  else
  {
    inputArray[i + 1] = '\0';
  }
  return inputArray;
}

/* Function to interpret user's input after the input is "valid" (has one or two chars). After 
 * interpreting the input, this function calls the function to perform the desired task of reading,
 * writing, or image manipulation. The function then returns the address to the array of Pixel
 * structs that is the result of the read, write, or manipulation.
 * Takes as argument the pointer to the input array, a pointer to the current image struct, and a 
 * pointer to the current Pixel array to operate on (could be NULL if first reading image, eg)
 * Returns 1 if input is one of the options, and zero otherwise.
 */

Pixel* interpretChoice(char* choice, Image * currentImg, Pixel *pixelArray){
  char* input;
  int* intInput;
  if (choice[0] == 'r' && choice[1] == '\0')
  {
    //get remaining input, then try read the specified image and return a pointer to the resulting
    //pixelArray
    input = collectRemainingInput();
    pixelArray = readImage(input, currentImg);
    free(input);
    free(choice);
    return pixelArray;
  }
  else if (choice[0] == 'w' && choice[1] == '\0')
  {
    //get remaining input, try to write the image to memory, return the pixelArray
    input = collectRemainingInput();

    if(!pixelArray){
      printf("No image to be written, re-read image!\n");
      return pixelArray;
    }
    int success = writePPM(pixelArray, input, currentImg);
    if (success == -1)
    {
      printf("Failed to write image!\n");
    }
    free(input);
    free(choice);
    return pixelArray;
  }
  else if (choice[0] == 'c' && choice[1] == '\0'){
    intInput = collectIntegerInput();
    pixelArray = crop(pixelArray, currentImg, intInput[0], intInput[1], intInput[2], intInput[3]);
    free(intInput);
    free(choice);
    return pixelArray;
  }
  else if (choice[0] == 'i' && choice[1] == '\0'){
    printf("Inverting intensity...\n");
    free(choice);
    return invert(pixelArray, currentImg);
  }
  else if (choice[0] == 's' && choice[1] == '\0'){
    printf("Swapping color channels...\n");
    free(choice);
    return swap(pixelArray, currentImg);
  }
  else if (choice[0] == 'g' && choice[1] == '\0'){
    printf("Converting to grayscale...\n");
    free(choice);
    return grayscale(pixelArray, currentImg);
  }
  else if (choice[0] == 'q' && choice[1] == '\0'){
    free(choice);
    //free(input);
    return pixelArray;
  }
  else if (choice[0] == 'b' && choice[1] == 'l' && choice[2] == '\0'){
    double sigma = collectDouble();
    printf("Applying blur filter, sigma %.2lf...\n", sigma);
    free(choice);
    return blur(pixelArray, currentImg, sigma);
  }
  else if (choice[0] == 's' && choice[1] == 'h' && choice[2] == '\0'){
    double sigma = collectDouble();
    double intensity = collectDouble();
    printf("Applying sharpen filter, sigma %.2lf, intensity %.2lf...\n", sigma, intensity);
    free(choice);
    return sharpen(pixelArray, currentImg, sigma, intensity);
  }
  else if (choice[0] == 'b' && choice[1] == 'r' && choice[2] == '\0'){
      double dBright = collectDouble();
      printf("Adjusting brightness by %d...\n", (int) dBright);
      free(choice);
      return brightness(pixelArray, currentImg, (int) dBright);
  }
  else if (choice[0] == 'c' && choice[1] == 'n' && choice[2] == '\0'){
    double factor = collectDouble();
    printf("Adjusting contrast by %.2lf...\n", factor);
    free(choice);
    return contrast(pixelArray, currentImg, factor);
  }
  else if(choice[0] == 'm' && choice[1] == '\0'){
    printf("Making mirror image...\n");
    free(choice);
    return mirrorImage(pixelArray, currentImg);
  }
  else if(choice[0] == 'r' && choice[1] == 'o' && choice[2] == '\0')
  {
    printf("Rotating 90 degrees to left...\n");
    free(choice);
    return rotate(pixelArray, currentImg);
  }
  else{
    printf("Invalid choice!\n");
    free(choice);
    return pixelArray;
  }
}

int editPhotos(){

  //initializes an image struct to store necessary information about the read image
  Image *imagePtr = malloc(sizeof(Image));

  //initializes a pixel array to store the color level image data for each pixel
  Pixel *pixelArray = NULL;

  bool quit = false;
  //initialize pointer
  char *valid = NULL;
  //char *validCopy = NULL;
  //while user is not trying to quit, continue asking for input
  while(!quit){
    //if user is not trying to quit, print the menu, collect their input
    if (!quit)
    {
      printMenu();
      valid = collectInput();
      //validCopy = valid;
      //if the input was valid, first check to see if the input was to quit, and set quit to true if 
      //it was
      if (valid)
      {
        if (valid[0] == 'q' && valid[1] == '\0')
        {
          free(valid);
          quit = true;
          printf("Goodbye!\n");
        }
        //if user not trying to quit and input was valid, try to perform their operation, 
        if (!quit && valid)
        {
          pixelArray = interpretChoice(valid, imagePtr, pixelArray);
          //free(valid);
        }
      //free(valid);
      }
      //free(valid);
    }
    //free(valid);
  }
  //printf("%d\n%d\n%d\n", valid[0], valid[1], valid[2]);
  //printf("Valid pointer: %p\n", valid);
  //free(valid);
  valid = NULL;
  //printf("After: %d\n%d\n%d\n", valid[0], valid[1], valid[2]);
  //frees the unneeded image and pixel array
  free(imagePtr); 
  free(pixelArray);
  return 0;

}

