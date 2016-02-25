#include <stdlib.h>
#include "core_engine.h"

float width, height;
lifecycle_ptrs* ptrs;

void init(void) {
    vertex_layout *l2 = vertex_layout_new_ptr();
    vertex_layout_pushback(l2, vertex_attribute_new(POSITION, 3));
    vertex_layout_pushback(l2, vertex_attribute_new(TEXCOORD0, 3));

    printf("0: %s\n",vertex_layout_getattributestring(l2, 0));
    printf("1: %s\n",vertex_layout_getattributestring(l2, 1));
    vertex_layout_cleanup(l2);
}

void fixed_update(void) {

}

void variable_update(double alpha) {}

void variable_render(double alpha) {
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void resize(int w, int h) {

}

void pause(void) {

}

void resume(void) { 
}

void handle_events(key_event* e) {

}

void quit(void) {
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
