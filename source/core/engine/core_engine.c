#include "core_engine.h"
#include <stdio.h>
#include <stdlib.h>

static int game_paused = -1;
static int game_running = -1;

GLFWwindow *main_window;
static lifecycle_ptrs *logic_pointers;

static double start_time, current_time, new_time;
static double delta_time = 0.01;
static double accum = 0.0;
static double frame_time;

static int window_width;
static int window_height;
static int virtual_width;
static int virtual_height;

key_event in_ev;

static void ce_fixed_update() {
    if (logic_pointers->fixed_update_func_ptr)
        logic_pointers->fixed_update_func_ptr();
}

static void ce_var_update(double alpha) {
    if (logic_pointers->variable_update_func_ptr)
        logic_pointers->variable_update_func_ptr(alpha);
}

static void ce_render(double alpha) {
    if (logic_pointers->variable_render_func_ptr)
        logic_pointers->variable_render_func_ptr(alpha);
}

static void ce_handle_events(key_event *k) {
    if (logic_pointers->handle_event_func_ptr)
        logic_pointers->handle_event_func_ptr(k);
}

static void ce_resize_window(int w, int h) {
    if (logic_pointers->resize_func_ptr) logic_pointers->resize_func_ptr(w, h);
}

static void cleanup(void);
static void init(char *path, const char *title);

// begin glfw functions
static void window_resize_callback(GLFWwindow *window, int width, int height) {
    window_width = width;
    window_height = height;
    ce_resize_window(width, height);
}

static void error_callback(int error, const char *description) {
    fputs(description, stderr);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action,
                         int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) game_running = -1;
    in_ev.k_window = window;
    in_ev.k_key = key;
    in_ev.k_scancode = scancode;
    in_ev.k_action = action;
    in_ev.k_mods = mods;
    ce_handle_events(&in_ev);
}

static void mouse_callback(GLFWwindow *window, int btn, int act, int mods) {
    printf("mouse button: %d\n", btn);
    in_ev.m_window = window;
    in_ev.m_button = btn;
    in_ev.m_action = act;
    in_ev.m_mods = mods;
    ce_handle_events(&in_ev);
}
// end glfw functions

void ce_pause() { game_paused = 1; }

void ce_resume(void) { game_paused = -1; }

void ce_quit(void) { game_running = -1; }

int ce_get_virtual_width(void) { return virtual_width; }
int ce_get_virtual_height(void) { return virtual_height; }
int ce_get_window_width(void) { return window_width; }
int ce_get_window_height(void) { return window_height; }

static int ce_run_game() {
    while (game_running > 0) {
        if (glfwWindowShouldClose(main_window)) game_running = -1;

        new_time = glfwGetTime();
        frame_time = new_time - current_time;

        if (frame_time > 0.025) {
            frame_time = 0.025;
        }

        current_time = new_time;
        accum += frame_time;

        while (accum >= delta_time) {
            ce_fixed_update();
            accum -= delta_time;
        }

        double alpha = accum / delta_time;
        // state = current_state * alpha + prev_state * (1.0 - alpha);
        ce_var_update(alpha);
        ce_render(alpha);

        glfwSwapBuffers(main_window);
        glfwPollEvents();
    }

    return -1;
}

static int init_glfw(int w, int h, const char *title) {
    if (!glfwInit()) return -1;
    main_window = glfwCreateWindow(w, h, title, NULL, NULL);
    if (!main_window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(main_window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwGetWindowSize(main_window, &window_width, &window_height);

    glfwSwapInterval(1);
    return 1;
}

int ce_init(int w, int h, const char *title, double fps, lifecycle_ptrs *fncs,
            char *path) {
    init_resource_path(path);

    logic_pointers = fncs;
    virtual_width = w;
    virtual_height = h;

    delta_time = 1.0 / fps;
    init(path, title);

    while (ce_run_game() >= 0)
        ;

    if (logic_pointers->quit_func_ptr) {
        logic_pointers->quit_func_ptr();
    }

    cleanup();
    return 0;
}

static void init(char *path, const char *title) {

    game_running = init_glfw(virtual_width, virtual_height, title);
    start_time = glfwGetTime();
    current_time = glfwGetTime();

    glfwSetErrorCallback(error_callback);
    glfwSetKeyCallback(main_window, key_callback);
    glfwSetMouseButtonCallback(main_window, mouse_callback);
    glfwSetWindowSizeCallback(main_window, window_resize_callback);

    if (logic_pointers->init_func_ptr) {
        logic_pointers->init_func_ptr();
    }
}

static void cleanup(void) {
    ce_quit();

    glfwDestroyWindow(main_window);
    glfwTerminate();
}
