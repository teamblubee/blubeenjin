#ifndef IMAGE_H
#define IMAGE_H

#include "stdlib.h"

typedef struct {
    int width;
    int height;
    int bpp;
    int data_size_bytes;
    unsigned char* data;
}image;

image* image_new_ptr(char* filename);
size_t image_get_width(image* in);
size_t image_get_height(image* in);
size_t image_get_bytes_per_pixel(image* in);
size_t image_get_data_size(image* in);
void* image_get_data(image* in);
void image_cleanup(image* in);

#endif /* IMAGE_H */
