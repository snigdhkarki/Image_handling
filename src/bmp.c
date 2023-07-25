#include "header/bmp.h"
#include "images.c"
#include <stdio.h>
#include <stdlib.h>

uint32 load_bmp_file(char *filename, image *img)
{
    releaseImage(img);

    bmpid bmp_id;
    bmpheader bmp_Header;
    infoheader info_Header;

    FILE *stream = fopen(filename, "rb");
    if (stream == NULL)
        return _CANT_OPEN;

    if (fread(bmp_id.id, sizeof(uint16), 1, stream) != 1)
    {
        fclose(stream);
        return _CANT_READ;
    }

    if (*((uint16 *)bmp_id.id) != 0x4D42)
    {
        fclose(stream);
        return _INVALID_ID;
    }

    if (fread(&bmp_Header, sizeof(bmpheader), 1, stream) != 1)
    {
        fclose(stream);
        return _CANT_READ;
    }

    if (fread(&info_Header, sizeof(infoheader), 1, stream) != 1)
    {
        fclose(stream);
        return _CANT_READ;
    }

    if (info_Header.compression_type != 0)
        fprintf(stderr, "Warning!!! Compression type is not supported");

    if (fseek(stream, bmp_Header.offset, SEEK_SET))
    {
        fclose(stream);
        return _CANT_READ;
    }
    // printf("id: %c%c\n", bmp_id.id[0], bmp_id.id[1]);
    // printf("size: %d    offset:%d   reserved:%d\n", bmp_Header.size, bmp_Header.offset, bmp_Header.reserved);
    // printf("infoheadersize:%d width:%d    height:%d   nplanes:%d  bpp:%d\n", info_Header.info_header_size, info_Header.width, info_Header.height, info_Header.nplanes, info_Header.bits_per_pixel);
    // printf("Type Compression:%d No of color used:%d  byte size:%d\n", info_Header.compression_type, info_Header.nused_colors, info_Header.bmp_size);
    // printf("bmp file header:%ld, bmp info header:%ld,file size sz:%ld\n", sizeof(bmp_Header), sizeof(info_Header), sizeof(bmp_Header.reserved));

    pixelFormat pf = ((info_Header.bits_per_pixel == 24) ? pf_rgb : (info_Header.bits_per_pixel == 32) ? pf_argb
                                                                                                       : pf_grey);
    *img = createImage(info_Header.width, info_Header.height, pf);
    fread(img->img_bmpdata, 1, info_Header.bmp_size, stream);
    fclose(stream);
    return _NO_ERROR;
}

void save_4bytes(uint32 val, FILE *stream)
{
    fprintf(stream, "%c", (val & 0x000000FF) >> 0 * 8);
    fprintf(stream, "%c", (val & 0x0000FF00) >> 1 * 8);
    fprintf(stream, "%c", (val & 0x00FF0000) >> 2 * 8);
    fprintf(stream, "%c", (val & 0xFF000000) >> 3 * 8);
}

uint32 save_img_to_file(const char *filename, image *img)
{
    int r, g, b;
    int padding = 4 - ((img->img_width * 3) % 4);
    if (padding == 4)
        padding = 0;
    int padded_size = ((img->img_width * 3) + padding) * img->img_height;

    FILE *stream = fopen(filename, "wb");
    fprintf(stream, "BM");
    save_4bytes(padded_size + 54, stream);
    save_4bytes(0, stream);
    save_4bytes(54, stream);
    save_4bytes(40, stream);
    save_4bytes(img->img_width, stream);
    save_4bytes(img->img_height, stream);

    fprintf(stream, "%c", 1);
    fprintf(stream, "%c", 0);

    fprintf(stream, "%c", 24);
    fprintf(stream, "%c", 0);

    save_4bytes(0, stream);
    save_4bytes(padded_size, stream);
    save_4bytes(0, stream);
    save_4bytes(0, stream);
    save_4bytes(0, stream);
    save_4bytes(0, stream);

    // headers completed
    int j = img->img_width * img->img_height;
    for (int i = 0; i < j; i++)
    {
        if (img->img_format == 1)
        {
            b = img->img_bmpdata[i];
            fputc(b, stream);
            continue;
        }
        else
        {
            int bpp = 3;
            b = img->img_bmpdata[i * bpp];
            g = img->img_bmpdata[i * bpp + 1];
            r = img->img_bmpdata[i * bpp + 2];
        }
        fputc(b, stream);
        fputc(g, stream);
        fputc(r, stream);
        if ((i + 1) % img->img_width == 0)
        {
            for (int k = 0; k < padding; k++)
            {
                fputc(0, stream);
            }
        }
    }
    fclose(stream);
    return _NO_ERROR;
}

int main()
{
    image file;
    image *img = &file;
    load_bmp_file("img/sample1.bmp", img);
    save_img_to_file("img/output.bmp", img);
    return 0;
}
