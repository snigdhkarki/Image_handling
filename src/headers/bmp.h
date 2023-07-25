#ifndef _BMP
#define _BMP

#include "types.h"
#include "images.h"
#include <stdio.h>

typedef struct
{
    uint8 id[2]; // this is 'BM' for bmp file
} bmpid;

typedef struct
{
    uint32 size;     // size of the file
    uint32 reserved; // unused(=0)
    uint32 offset;   // offset from beginning of file to beginning of the bitmap data
} bmpheader;

typedef struct
{
    uint32 info_header_size; // size of this header(=40 bytes)
    uint32 width;            // width of image in pixel
    uint32 height;           // height of image in pixel
    uint16 nplanes;          // no of planes used(=1)
    uint16 bits_per_pixel;   // no of bits used to represent a color in a pixel
    uint32 compression_type; // type of compression if any (=0 if no compression)
    uint32 bmp_size;         // size of the img
    uint32 hres;             // horizontal resolution (pixels/meter)
    uint32 vres;             // vertical resolution (pixels/meter)
    uint32 nused_colors;     // no of used colors. For a 8-bit / pixel bitmap this will be 256
    uint32 nimp_colors;      // no of imp colors(0= all)  (generally ignored)
} infoheader;

uint32 load_bmp_file(char *, image *);
uint32 save_img_to_file(const char *, image *);
void save_4bytes(uint32, FILE *);

typedef enum
{
    _NO_ERROR = 0,
    _CANT_OPEN = 2,
    _CANT_READ = 3,
    _INVALID_ID = 4
} file_error;

#endif