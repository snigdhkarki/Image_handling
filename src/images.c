#include "header/images.h"
#include <stdlib.h> // for using malloc()
#include <string.h> // for using memset()

image createImage(uint32 width, uint32 height, pixelFormat format)
{
    image img;                             // creating an obj img of struct image
    resetImage(&img);                      // reseting the img
    uint32 size = width * height * format; // calculating the total size needed for storing pixel values of the image
    if (size > 0 && format != pf_unknown)
    {
        img.img_bmpdata = (uint8 *)malloc(size);
        img.img_width = width;
        img.img_height = height;
        img.img_format = format;
        memset(img.img_bmpdata, 0, size); // makes sure that the pixel value of img is 0 initially
    }
    return img;
}

void resetImage(image *img)
{
    img->img_height = 0;
    img->img_width = 0;
    img->img_format = pf_unknown;
    img->img_bmpdata = 0x0;
}

void releaseImage(image *img)
{
    if (img->img_bmpdata != 0x0)
    {
        free(img->img_bmpdata);
    }
    resetImage(img);
}