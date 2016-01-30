#ifndef CORE_ENGINE_H
#define CORE_ENGINE_H

#include "glad.h"
#include <GLFW/glfw3.h>
#include "vectormath_aos.h"

typedef struct {
    GLFWwindow *k_window;
    int k_key;
    int k_scancode;
    int k_action;
    int k_mods;
    GLFWwindow *m_window;
    int m_button;
    int m_action;
    int m_mods;
} key_event;

typedef struct {
    void (*init_func_ptr)(void);
    void (*fixed_update_func_ptr)(void);
    void (*variable_update_func_ptr)(double alpha);
    void (*variable_render_func_ptr)(double alpha);
    void (*pause_func_ptr)(void);
    void (*resume_func_ptr)(void);
    void (*resize_func_ptr)(int width, int height);
    void (*handle_event_func_ptr)(key_event *e);
    void (*quit_func_ptr)(void);
} lifecycle_ptrs;

int ce_init(int w, int h, const char *title, double fps, lifecycle_ptrs *fncs,
            char *path);
int ce_get_virtual_width(void);
int ce_get_virtual_height(void);
int ce_get_window_width(void);
int ce_get_window_height(void);
void ce_resume(void);
void ce_pause(void);
void ce_quit(void);


#endif /* CORE_ENGINE_H */
