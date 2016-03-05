#include "resource_utils.h"
#include "image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_img.h"

image* image_new_ptr(char* filename) {
    char* f_path;
    unsigned char* data;

    get_resource(&f_path, filename);

    image* out = calloc(1, sizeof(image));
    out->data = stbi_load(f_path, &out->width, &out->height, &out->bpp, 0);
    if (data == NULL) printf("failed creating image:%s\n", f_path);
    out->data_size_bytes = (out->bpp * out->width * out->height);

    free(f_path);
    return out;
}

size_t image_get_width(image* in) { return in->width; }

size_t image_get_height(image* in) { return in->height; }

size_t image_get_bytes_per_pixel(image* in) { return in->bpp; }

size_t image_get_data_size(image* in) { return in->data_size_bytes; }

void* image_get_data(image* in) { return in->data; }

void image_cleanup(image* in) {
    free(in->data);
    in->data = 0;
}
