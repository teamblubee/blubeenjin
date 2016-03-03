#include <stdlib.h>
#include "core_engine.h"

float width, height;
lifecycle_ptrs* ptrs;

sprite* q_sprite;
shader_fx* shader;

GLint pos_loc = -1;
GLint col_loc = -1;

mat4 view_mat, proj_mat;

void init(void) {
    // shader --
    // mesh --
    // image
    // texture2d
    //

    q_sprite = sprite_new_ptr(400, 400);

    shader = shader_create_from_file("default.vert", "default.frag");
    pos_loc = shader_get_attribute_location(shader, "a_pos");
    col_loc = shader_get_attribute_location(shader, "a_col");
}

void fixed_update(void) {}

void variable_update(double alpha) {}

void variable_render(double alpha) {
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader_bind_program(shader);
    sprite_render_bind(q_sprite);
}

void resize(int w, int h) {}

void pause(void) {}

void resume(void) {}

void handle_events(key_event* e) {}

void quit(void) {
    sprite_cleanup(q_sprite);
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
