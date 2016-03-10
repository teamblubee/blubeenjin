#include <stdlib.h>
#include "core_engine.h"

float width, height, aspect;
lifecycle_ptrs* ptrs;

sprite* q_sprite, *bkg;
shader_fx* shader;

GLint m_mat_loc, v_mat_loc, p_mat_loc, tex_loc;
mat4 v_mat, p_mat;

int l, r, u, d;
int cl, cr, cu, cd;
image* dino;
texture* dtex, *btex;
float x = 30;
float y = 30;

void init(void) {
    dtex = texture_new_ptr("dino.png");
    btex = texture_new_ptr("background.jpg");

    shader = shader_create_from_file("default.vert", "default.frag");

    bkg = sprite_new_ptr(btex, 0, 0);
    q_sprite = sprite_new_ptr(dtex, 0, 0);

    glViewport(0, 0, width, height);
    point3 eye, center;
    vec3 up;
    vmathP3MakeFromElems(&eye, 0, 0, 0);
    vmathP3MakeFromElems(&center, 0, 0, -1);
    vmathV3MakeFromElems(&up, 0, 1, 0);
    vmathM4MakeLookAt(&v_mat, &eye, &center, &up);

    vec3 trans;
    vmathV3MakeFromElems(&trans, -width / 2, -height / 2, 0);
    vmathM4MakeTranslation(&v_mat, &trans);

    // vmathM4MakeOrthographic(&p_mat, -width / 2, width / 2, -height / 2,
    // height / 2, 0, 100);

    if (width >= height) {
        // aspect >= 1, set the height from -1 to 1, with larger width
        // gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
        vmathM4MakeOrthographic(&p_mat, (-width / 2) * aspect,
                                (width / 2) * aspect, -height / 2, height / 2,
                                0, 100);
    } else {
        // aspect < 1, set the width to -1 to 1, with larger height
        // gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
        vmathM4MakeOrthographic(&p_mat, -width / 2, width / 2,
                                (-height / 2) * aspect, (height / 2) * aspect,
                                0, 100);
    }

    m_mat_loc = shader_get_uniform_location(shader, "u_mod_mat");
    v_mat_loc = shader_get_uniform_location(shader, "u_view_mat");
    p_mat_loc = shader_get_uniform_location(shader, "u_proj_mat");
    tex_loc = shader_get_uniform_location(shader, "u_sprite_tex");
}

void fixed_update(void) {
    if (l == 1) {
        sprite_add_position(q_sprite, -20.0f, 0);
    }
    if (r == 1) {
        sprite_add_position(q_sprite, 20.0f, 0);
    }
    if (u == 1) {
        sprite_add_position(q_sprite, 0, 20.0f);
    }
    if (d == 1) {
        sprite_add_position(q_sprite, 0, -20.0f);
    }

    if (q_sprite->pos.x + (q_sprite->scale.x*2) >= width ) {
        x = -x;
    }
    if (q_sprite->pos.x < 0) {
        x = -x;
    }
    if (q_sprite->pos.y  + (q_sprite->scale.y*2) >= height) {
        y = -y;
    }
    if (q_sprite->pos.y < 0) {
        y = -y;
    }

    sprite_add_position(q_sprite, x, y);

    if (cl == 1) {
        float x = vmathM4GetElem(&v_mat, 3, 0);
        x += 10.1;
        vmathM4SetElem(&v_mat, 3, 0, x);
        // printf("x:%f\n",x);
    }
    if (cr == 1) {
        float x = vmathM4GetElem(&v_mat, 3, 0);
        x -= 10.1;
        vmathM4SetElem(&v_mat, 3, 0, x);
        // printf("x:%f\n",x);
    }
    if (cu == 1) {
        float y = vmathM4GetElem(&v_mat, 3, 1);
        y += 10.1;
        vmathM4SetElem(&v_mat, 3, 1, y);
        // printf("x:%f\n",x);
    }
    if (cd == 1) {
        float y = vmathM4GetElem(&v_mat, 3, 1);
        y -= 10.1;
        vmathM4SetElem(&v_mat, 3, 1, y);
        // printf("x:%f\n",x);
    }
}

void variable_update(double alpha) {}

void variable_render(double alpha) {
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUniformMatrix4fv(p_mat_loc, 1, GL_FALSE, vmathM4GetData(&p_mat));
    glUniformMatrix4fv(v_mat_loc, 1, GL_FALSE, vmathM4GetData(&v_mat));

    // render bg
    sprite_bind_render(bkg, shader, tex_loc, m_mat_loc, alpha);

    // render dino
    sprite_bind_render(q_sprite, shader, tex_loc, m_mat_loc, alpha);

    // vmathM4Prints(&p_mat, "proj");
    // vmathM4Prints(&v_mat, "view");
    // vmathM4Prints(&q_sprite->model_mat, "model");
    // vmathV3Prints(&q_sprite->pos, "position");
}

void resize(int w, int h) {
    if (h == 0) h = 1;  // To prevent divide by 0
    GLfloat a = (GLfloat)w / (GLfloat)h;

    // Set the viewport to cover the new window
    glViewport(0, 0, w, h);

    // Set the aspect ratio of the clipping area to match the viewport
    if (w >= h) {
        // aspect >= 1, set the height from -1 to 1, with larger width
        // gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
        vmathM4MakeOrthographic(&p_mat, (-w / 2), (w / 2), -h / 2, h / 2, 0,
                                100);
    } else {
        // aspect < 1, set the width to -1 to 1, with larger height
        // gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
        vmathM4MakeOrthographic(&p_mat, -w / 2, w / 2, (-h / 2) * a,
                                (h / 2) * a, 0, 100);
    }
}

void pause(void) {}

void resume(void) {}

void handle_events(key_event* e) {
    if (e->k_action == GLFW_PRESS) {
        // printf("pressed\n");
        if (e->k_key == GLFW_KEY_LEFT) {
            l = 1;
        }
        if (e->k_key == GLFW_KEY_RIGHT) {
            r = 1;
        }
        if (e->k_key == GLFW_KEY_UP) {
            u = 1;
        }
        if (e->k_key == GLFW_KEY_DOWN) {
            d = 1;
        }
    }

    if (e->k_action == GLFW_RELEASE) {
        if (e->k_key == GLFW_KEY_LEFT) {
            l = -1;
        }
        if (e->k_key == GLFW_KEY_RIGHT) {
            r = -1;
        }
        if (e->k_key == GLFW_KEY_UP) {
            u = -1;
        }
        if (e->k_key == GLFW_KEY_DOWN) {
            d = -1;
        }
    }

    if (e->k_action == GLFW_PRESS) {
        // printf("pressed\n");
        if (e->k_key == GLFW_KEY_D) {
            cl = 1;
        }
        if (e->k_key == GLFW_KEY_A) {
            cr = 1;
        }
        if (e->k_key == GLFW_KEY_W) {
            cu = 1;
        }
        if (e->k_key == GLFW_KEY_S) {
            cd = 1;
        }
    }

    if (e->k_action == GLFW_RELEASE) {
        // printf("pressed\n");
        if (e->k_key == GLFW_KEY_D) {
            cl = -1;
        }
        if (e->k_key == GLFW_KEY_A) {
            cr = -1;
        }
        if (e->k_key == GLFW_KEY_W) {
            cu = -1;
        }
        if (e->k_key == GLFW_KEY_S) {
            cd = -1;
        }
    }
}

void quit(void) {
    sprite_cleanup(q_sprite);
    shader_cleanup(shader);
    texture_cleanup(dtex);
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

    // width = 2280;
    // height = 1440;
    width = 1024;
    height = 720;

    // width = 1920;
    // height = 1280;
    aspect = width / height;

    ce_init(width, height, "test title", 10.0, ptrs, argc[0]);
    return 0;
}
