#ifndef RESOURCE_UTILS_H
#define RESOURCE_UTILS_H

#include "stb_img.h"

void init_resource_path(char* path);
void cleanup_resource_path(void);

char* get_resource(char** res, const char* filename);
const char* read_file(char** data, const char* file_path);

char* get_resource_path(void);
char* get_savedata_path(void);

void save_file(char* filename, char* data); //not finished implementing yet

#endif /* RESOURCE_UTILS_H */
