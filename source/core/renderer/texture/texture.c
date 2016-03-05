#include "resource_utils.h"
#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_img.h"

texture* texture_new_ptr(char* filename) {
    char* f_path;
    unsigned char* data;

    get_resource(&f_path, filename);

    texture* out = calloc(1, sizeof(texture));
    out->data = stbi_load(f_path, &out->width, &out->height, &out->bpp, 0);
    if (data == NULL) printf("failed creating image:%s\n", f_path);
    out->data_size_bytes = (out->bpp * out->width * out->height);

    if (out->bpp == 4) {
        out->texture_format = GL_RGBA;
    } else if (out->bpp == 3) {
        out->texture_format = GL_RGB;
    } else if (out->bpp == 2) {
        out->texture_format = GL_RG;
    } else if (out->bpp == 1) {
        out->texture_format = GL_RED;
    } else {
        printf("error opening file\n");
    }

    glGenTextures(1, &out->texture_id);
    glBindTexture(GL_TEXTURE_2D, out->texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glTexImage2D(GL_TEXTURE_2D, 0, out->texture_format, out->width, out->height,
                 0, out->texture_format, GL_UNSIGNED_BYTE, out->data);

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    free(f_path);
    return out;
}

size_t texture_get_width(texture* in) { return in->width; }

size_t texture_get_height(texture* in) { return in->height; }

size_t texture_get_bytes_per_pixel(texture* in) { return in->bpp; }

size_t texture_get_data_size(texture* in) { return in->data_size_bytes; }

void* texture_get_data(texture* in) { return in->data; }

void texture_cleanup(texture* in) {
    free(in->data);
    in->data = 0;
}
