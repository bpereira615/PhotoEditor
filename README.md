***
  CS120 - Homework 6: Photo Editor
  2015-10-12

  Benjamin Gellman: bgellma1@jhu.edu
  Benjamin Hoertnagl-Pereira: bhoertn1@jhu.edu



  Photo Editor:
  This program allows users to manipulate .ppm files by:
  -cropping
  -inverting color
  -swapping channels
  -making grayscale
  -blurring
  -sharpening
  -adjusting contrast
  -mirroring image
  -toggling 90 degree left rotation of image



  Source Files:
  hw6.c - driver file with function call that runs the program
  ppmIO.c - contains functions used to handle input and output of ppm files
  menuUtil.c - contains functions that handle interaction with user and 
    executing commmands on the given image
  imageManip.c - contains functions that manipulate the image in a desired way

  ppmIO.h - header file containing function declarations of ppmIO.c
  menuUtil.h - header file containing function declarations of menuUtil.c
  imageManip.h - header file containing function declarations of imageManip.c
  
  Makefile - used to compile, no unit tests written for the Makefile-testing was
    done manually
  mercurial.log- mercurial log file of commits



  Overall, this assignment was fair. The directions were very clear and easy
  to follow. Handling the image and storing its data in an array of pixel structs 
  was by far the most difficult aspect. Once we had the pixel array, writing the
  manipulating fuctions was straightforward.
***

## Program Description

This program will be an image-manipulation program, in the vein of Photoshop. Yours will have a text-based UI, so there will be no graphical interface, and the range of operations will be limited, but the algorithms you will use are the same ones used in programs like Photoshop or GIMP.

At a basic level, the program will be able to read image files from disk, perform a variety of image manipulation tasks, and then write the result back to disk as a new image file. Since your program will not have a GUI, we will use external programs to view the images. If you are on ugrad (either locally, or remotely with X-tunnelling), you can use the program feh, which is a very simple command-line image viewer; simply run the program with the name of an image file as a command-line argument, and it will display the image on your screen. If you are using a different platform, you are welcome to use an image viewer of your choice; feh is easy to install using most linux package managers, but there are other open source image viewing programs, as well as alternatives for Windows and OSX.

While there are many formats for storing image files, your program will only need to read and write one, the PPM format. This is essentially the simplest and easiest format to read and write, which is why it was chosen; its main drawback is that it does not use any kind of compression, so images stored in this format tend to be on the large side when compared to formats like JPEG or PNG or GIF.

Your program will ultimately be able to perform all of the following operations:

* read an image from a file
* write an image to a file
* crop an image (i.e. extract a rectangular region as a new image)
* invert the colors (i.e. black becomes white, white becomes black, etc.)
* swap color channels (i.e. what was red becomes blue, what was blue becomes green, etc.)
* convert to grayscale (i.e. a full-color image becomes shades of gray)
* increase/decrease brightness
* increase/decrease contrast
* blur (smooth and soften edges)
* sharpen (make edges sharper)

### Files:

* Makefile - if you use the same filenames, you can probably just use the one provided in the scaffolding
* hw6.c - main program; the main() function in my implementation literally calls a single function, then returns 0.
* menuUtil.c - contains implementations of all the functions related to printing the menu, reading user input, etc.
* ppmIO.c - contains implementations of functions for reading, writing, creating, destroying, copying, etc. images (using the PPM format)
* imageManip.c - contains implementations of all image manipulation algorithms
* menuUtil.h - header for menuUtil stuff (prototypes, enums, #defines)
* ppmIO.h - header for ppmIO stuff (prototypes, structs)
* imageManip.h - header for imageManip stuff (prototypes, #defines)
* unitTest_imageManip.c - unit tests for imageManip.c
* unitTest_ppmIO.c - unit tests for ppmIO.c
* unitTest_menuUtil.c - unit tests for menuUtil.c
