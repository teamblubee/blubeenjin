#include <stdlib.h>
#include "core_engine.h"

float width, height;
lifecycle_ptrs* ptrs;

float v_data[] = {-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                  -0.5f, 0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                  0.5f,  -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f};

// float v_data[] = {-1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
//-1.0f, 1.0f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
// 1.0f,  1.0f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
// 1.0f,  -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f};

short i_data[] = {0, 1, 2, 0, 2, 3};

shader_fx* shader;
mesh* quad;

GLuint single_quad_vao;
GLint pos_loc = -1;
GLint col_loc = -1;



void init(void) {
    // shader --
    // mesh --
    // image
    // texture2d
    //

    vertex_attribute pos = vertex_attribute_new(POSITION, 3);
    vertex_attribute col = vertex_attribute_new(POSITION, 4);

    vertex_layout* flat_quad = vertex_layout_new_ptr();
    vertex_layout_pushback(flat_quad, &pos);
    vertex_layout_pushback(flat_quad, &col);

    quad = mesh_new_ptr(STATIC, TRIANGLES, flat_quad);
    vertex_layout_cleanup(flat_quad);

    mesh_set_vertex_data(quad, v_data, (sizeof(v_data)/sizeof(v_data[0])));
    mesh_set_index_data(quad, i_data, (sizeof(i_data)/sizeof(i_data[0])));

    printf("total size:%zu bytes\n",mesh_get_vertex_sizebytes(quad));

    shader = shader_create_from_file("default.vert", "default.frag");

    pos_loc = shader_get_attribute_location(shader, "a_pos");
    col_loc = shader_get_attribute_location(shader, "a_col");

    glGenVertexArrays(1, &single_quad_vao);
    glBindVertexArray(single_quad_vao);

    glBindBuffer(GL_ARRAY_BUFFER, mesh_get_vertex_buffer_handle(quad));
    glBufferData(GL_ARRAY_BUFFER, mesh_get_vertex_count(quad),
                 mesh_get_vertex_data(quad), GL_STREAM_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float),
                          (GLvoid*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float),
                          (GLvoid*)(3 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, mesh_get_index_buffer_handle(quad));
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh_get_index_count(quad),
                 mesh_get_index_data(quad), GL_STREAM_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void fixed_update(void) {}

void variable_update(double alpha) {}

void variable_render(double alpha) {
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader_bind_program(shader);
    glBindVertexArray(single_quad_vao);

    glBindBuffer(GL_ARRAY_BUFFER, mesh_get_vertex_buffer_handle(quad));
    glBufferData(GL_ARRAY_BUFFER, mesh_get_vertex_sizebytes(quad),
                 mesh_get_vertex_data(quad), GL_STREAM_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_get_index_buffer_handle(quad));
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh_get_index_sizebytes(quad),
                 mesh_get_index_data(quad), GL_STREAM_DRAW);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
}

void resize(int w, int h) {}

void pause(void) {}

void resume(void) {}

void handle_events(key_event* e) {}

void quit(void) {
    mesh_cleanup(quad);
    shader_cleanup(shader);
    free(ptrs);
}

int main(int argv, char* argc[]) {
    ptrs = calloc(1, sizeof(lifecycle_ptrs));
    ptrs->init_func_ptr = &init;
    ptrs->fixed_update_func_ptr = &fixed_update;
    ptrs->variable_update_func_ptr = &variable_update;
    ptrs->variable_render_func_ptr = &variable_render;
    ptrs->pause_func_ptr = &pause;
    ptrs->resume_func_ptr = &resume;
    ptrs->resize_func_ptr = &resize;
    ptrs->handle_event_func_ptr = &handle_events;
    ptrs->quit_func_ptr = &quit;

    width = 1024;
    height = 720;

    ce_init(width, height, "test title", 60.0, ptrs, argc[0]);
    return 0;
}
