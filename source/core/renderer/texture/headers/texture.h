#ifndef TEXTURE_H
#define TEXTURE_H

#define STB_IMAGE_IMPLEMENTATION
#include "stdlib.h"
#include "glad.h"

typedef struct {
    GLenum texture_format;
    GLuint texture_id;
    int width;
    int height;
    int bpp;
    int data_size_bytes;
    unsigned char* data;
}texture;

texture* texture_new_ptr(char* filename);
size_t texture_get_width(texture* in);
size_t texture_get_height(texture* in);
size_t texture_get_bytes_per_pixel(texture* in);
size_t texture_get_data_size(texture* in);
void* texture_get_data(texture* in);
void texture_cleanup(texture* in);

#endif /* TEXTURE_H */
