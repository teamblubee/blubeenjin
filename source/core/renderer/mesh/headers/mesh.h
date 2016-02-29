#ifndef MESH_H
#define MESH_H

#include "glad.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "vertex_layout.h"

typedef enum {
    STATIC = GL_STATIC_DRAW,
    STREAM = GL_STREAM_DRAW,
    DYNAMIC = GL_DYNAMIC_DRAW
} mesh_draw_mode;

typedef enum {
    TRIANGLES = GL_TRIANGLES,
    POINTS = GL_POINTS,
    LINES = GL_LINES,
    LINE_LOOP = GL_LINE_LOOP,
    TRIANGLE_STRIP = GL_TRIANGLE_STRIP
} mesh_primitive_type;

typedef struct {
    vertex_layout* vertex_layout;
    mesh_draw_mode draw_mode;
    mesh_primitive_type primitive_type;
    GLuint vertex_buffer_handle;
    GLuint index_buffer_handle;
    GLuint mesh_vao_handle;
    size_t vert_attrib_count;
    float* vertex_data;
    short* index_data;
    size_t vertex_count;
    size_t index_count;
    size_t vertex_data_stride;
    size_t elements_to_render;
} mesh;

mesh* mesh_new_ptr(mesh_draw_mode mode, mesh_primitive_type primite_type,
                   vertex_layout* layout);
mesh* mesh_set_vertex_data(mesh* out, float* data, size_t count);
mesh* mesh_bind_vertex_data(mesh* out);

mesh* mesh_update_vertex_data(mesh* out, float* data, size_t start,
                              size_t count);
mesh* mesh_set_index_data(mesh* out, short* data, size_t count, size_t elements);
mesh* mesh_bind_index_data(mesh* out);
size_t mesh_get_index_count(mesh* in);
size_t mesh_get_vertex_sizebytes(mesh* in);
size_t mesh_get_index_sizebytes(mesh* in);
size_t mesh_get_vertex_count(mesh* in);
size_t mesh_get_vertex_stride(mesh* in);
size_t mesh_get_element_count(mesh* in);
short* mesh_get_index_data(mesh* in);
float* mesh_get_vertex_data(mesh* in);
GLuint mesh_get_vertex_buffer_handle(mesh* in);
GLuint mesh_get_index_buffer_handle(mesh* in);

mesh_draw_mode mesh_get_draw_mode(mesh* in);
mesh_primitive_type mesh_get_primitive_type(mesh* in);
void mesh_cleanup(mesh* in);

#endif /* MESH_H */
