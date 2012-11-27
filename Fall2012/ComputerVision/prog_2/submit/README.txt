Computer Vision Prog 2
Brian Fehrman

COMPILE
------------
From within the prog_2 directory, the program should be able to be compiled with a simple:

make clean
make all

You may need to edit the directories in the make file. This command will simply use
prog_2.cpp as the compile target which is actually a "unity" compile file.

RUN
---------------
From the prog 2 directory, call:

./bin/prog_2 left_image_name right_image_name (output_file_name)

Right now it seems like it needs the left and right images passed in accordingly but this is probably an easy fix.

Options
----------------
Can enter:

'a' : Auto mosaic stitching (self explanatory). Runs slow because of ransac, high resolution images, and adaptive non-maximum supression which are all things that can
be optimized

'f' : Manual frame stitching in which you select 4 arbitrary points from the left image to make a box from which pixels will be taken. Then you select 4 arbitrary points in the right image to make a box around where you want your pixels to be warped. Hit the "enter" key after selecting all 8 points. THE RIGHT MOUSE BUTTON CAN
BE USED TO DESELECT POINTS.

'm' : Manual mosaic stitching in which you select 4 points in the left image and 4 
coressponding points in the right image. Hit "enter" key after selecting all points. Again, the right mouse button can be used to deselect points.

's' : Semi-auto frame stitching in which you select just 4 points in the right image 
to make a box in to which you would like to transfer ALL pixels from the left image.

Notes
------------
Program seems to randomly crash sometimes. I am not sure what it is exactly but I will
look in to it more. If it crashes then just run it a few more times and it should work.

If the output image doesn't seem right then try swapping the order of the command line
argument file names.
