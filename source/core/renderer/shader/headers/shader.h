#ifndef SHADER_H
#define SHADER_H

#include "glad.h"
#include "stdlib.h"
#include "stdio.h"
#include "resource_utils.h"

typedef struct {
    GLuint shader_program_handle;
}shader_fx;

shader_fx *shader_create_from_file(char* v_path, char* f_path);
shader_fx *shader_create_from_string(char* v_string, char* f_string);
GLint shader_get_uniform_location(shader_fx* shader, char* u);
GLint shader_get_attribute_location(shader_fx* shader, char* a);
void shader_bind_program(shader_fx* in);
void shader_cleanup(shader_fx* in);

#endif /* SHADER_H */
