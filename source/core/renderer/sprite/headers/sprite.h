#ifndef SPRITE_H
#define SPRITE_H

#include "glad.h"
#include "mesh.h"
#include "vectormath_aos.h"

typedef struct {
    mesh* sprite_mesh;
    mat4 model_mat;
    vec3 pos;
    vec3 rot;
    vec3 scale;
    GLuint texture_id;
    GLuint vertex_buffer_handle;
    GLuint index_buffer_handle;
    GLuint sprite_mesh_vao_handle;
} sprite;

sprite* sprite_new_ptr(float w, float h, float x, float y);
GLuint sprite_get_vertex_buffer_handle(sprite* in);
GLuint sprite_get_index_buffer_handle(sprite* in);
void sprite_bind_render(sprite* out);
void sprite_cleanup(sprite* in);

#endif /* SPRITE_H */
