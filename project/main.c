#include <stdlib.h>
#include "core_engine.h"

float width, height;
lifecycle_ptrs* ptrs;

void init(void) {

}

void fixed_update(void) {}

void variable_update(double alpha) {}

void variable_render(double alpha) {
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void resize(int w, int h) {}

void pause(void) {}

void resume(void) { free(ptrs); }

void handle_events(key_event* e) {}

void quit(void) {}

int main(int argv, char* argc[]) {
    ptrs = (lifecycle_ptrs*)calloc(1, sizeof(lifecycle_ptrs));
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
