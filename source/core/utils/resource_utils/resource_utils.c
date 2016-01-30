#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "resource_utils.h"

char* resource_dir;
char* savedata_dir;

size_t resource_dir_len;
size_t savedata_dir_len;

void init_resource_path(char* path) {
    size_t end_slash_count;
    size_t exe_dir_len = 10;
    size_t res_dir_len = 11;
    size_t sub_path;
    char* end_slash;
    char* actual_path;
    char res_dir[11] = "/resources/";
    char data_dir[6] = "/data/";

    size_t full_path_size = strlen(path) + 1;

    actual_path = calloc(full_path_size, sizeof(char*));
    realpath(path, actual_path);

    end_slash = strrchr(actual_path, '/');
    end_slash_count =
        (end_slash - actual_path + 1);  //+1 for the null terminating char

    resource_dir_len = end_slash_count + res_dir_len;
    resource_dir = calloc(resource_dir_len, sizeof(char));
    sub_path = end_slash - actual_path;

    strncat(resource_dir, actual_path, (sub_path));
    strncat(resource_dir, res_dir, res_dir_len);

    savedata_dir_len = end_slash_count + res_dir_len;
    savedata_dir = calloc(resource_dir_len, sizeof(char));

    strncat(savedata_dir, actual_path, (sub_path));
    strncat(savedata_dir, data_dir, savedata_dir_len);
    free(actual_path);
}

char* get_resource(char** res, const char* filename) {
    size_t count = ((strlen(resource_dir) + strlen(filename) + 1));
    *res = calloc(count, sizeof(char));
    strncpy(*res, resource_dir, resource_dir_len);
    strncat(*res, filename, strlen(filename));
    return *res;
}

char* read_file(char** data, const char* file_path) {
    FILE* fp;
    size_t buffer = 4096;
    size_t index = 0;
    int ch;

    fp = fopen(file_path, "r");
    if (fp == NULL) {
        printf("failed to open file: %s\n", file_path);
        return "-1\0";
    }

    (*data) = calloc(buffer, sizeof(char));
    while (EOF != (ch = fgetc(fp))) {
        (*data)[index] = (char)ch;
        ++index;
        if (index == buffer - 1) {
            buffer = buffer * 2;
            data = realloc(data, buffer);
            if (data != NULL) {
                printf(
                    "buffer not large enough, reallocating %zu bytes to "
                    "load %s\n",
                    buffer, file_path);
            } else {
                printf("failed to realloc %zu bytes to load %s\n", buffer,
                       file_path);
            }
        }
    }
    (*data) = realloc((*data), (sizeof(char) * (index + 1)));
    (*data)[index] = '\0';

    fclose(fp);
    return *data;
}

char* get_resource_path(void) { return resource_dir; }
char* get_savedata_path(void) { return savedata_dir; }

void save_file(char* filename, char* data) {
    //if filename doesn't exist, create it, write data
    printf("implement this later, filename;%s\n",filename);
}

void cleanup_resource_path(void) {
    free(savedata_dir);
    free(resource_dir);
}
