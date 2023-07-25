#ifndef _IMAGES
#define _IMAGES

#include "types.h"

typedef enum
{
    pf_unknown = 0,
    pf_grey = 1,
    pf_rgb = 3,
    pf_argb = 4
} pixelFormat;

typedef struct
{
    uint32 img_width;
    uint32 img_height;
    pixelFormat img_format;
    uint8 *img_bmpdata;
} image;

image createImage(uint32, uint32, pixelFormat);
void releaseImage(image *);
void resetImage(image *);
#endif