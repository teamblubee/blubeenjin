#include <stdlib.h>
#include "core_engine.h"

float width, height, aspect;
lifecycle_ptrs* ptrs;

sprite* q_sprite;
shader_fx* shader;

GLint m_mat_loc, v_mat_loc, p_mat_loc, tex_loc;
mat4 v_mat, p_mat;

int l, r, u, d;
image* dino;
texture* dtex;

void init(void) {
    // shader --
    // mesh --
    // image
    // texture2d
    //

    dino = image_new_ptr("dino.png");
    image_cleanup(dino);

    dtex = texture_new_ptr("dino.png");
    //dtex = texture_new_ptr("dino2.jpg");
    // texture_cleanup(dtex);

    q_sprite = sprite_new_ptr(dtex, width * 0.5, height * 0.5);

    shader = shader_create_from_file("default.vert", "default.frag");

    point3 eye, center;
    vec3 up;
    vmathP3MakeFromElems(&eye, 0, 0, -20);
    vmathP3MakeFromElems(&center, 0, 0, -1);
    vmathV3MakeFromElems(&up, 0, 1, 0);
    vmathM4MakeLookAt(&v_mat, &eye, &center, &up);
    // vmathM4SetElem(&v_mat, 3, 0, width * 0.5f);
    // vmathM4SetElem(&v_mat, 3, 1, height * 0.5f);
    // vmathM4SetElem(&v_mat, 3, 2, -20);
    // vmathM4Prints(&v_mat, "view");
    vmathM4MakeOrthographic(&p_mat, 0, width, 0, height, 1, 100);
    glViewport(0, width, 0, -height);

    m_mat_loc = shader_get_uniform_location(shader, "u_mod_mat");
    v_mat_loc = shader_get_uniform_location(shader, "u_view_mat");
    p_mat_loc = shader_get_uniform_location(shader, "u_proj_mat");
    tex_loc = shader_get_uniform_location(shader, "u_sprite_tex");
}

void fixed_update(void) {
    if (l == 1) {
	// float x = vmathM4GetElem(&v_mat, 3, 0);
	float x = vmathM4GetElem(&q_sprite->model_mat, 3, 0);
	x += 10.1;
	// vmathM4SetElem(&v_mat, 3, 0, x);
	vmathM4SetElem(&q_sprite->model_mat, 3, 0, x);
	// printf("x:%f\n",x);
    }
    if (r == 1) {
	// float x = vmathM4GetElem(&v_mat, 3, 0);
	float x = vmathM4GetElem(&q_sprite->model_mat, 3, 0);
	x -= 10.1;
	// vmathM4SetElem(&v_mat, 3, 0, x);
	vmathM4SetElem(&q_sprite->model_mat, 3, 0, x);
	// printf("x:%f\n",x);
    }
    if (u == 1) {
	// float y = vmathM4GetElem(&v_mat, 3, 1);
	float y = vmathM4GetElem(&q_sprite->model_mat, 3, 1);
	y += 10.1;
	// vmathM4SetElem(&v_mat, 3, 1, y);
	vmathM4SetElem(&q_sprite->model_mat, 3, 1, y);
	// printf("x:%f\n",x);
    }
    if (d == 1) {
	// float y = vmathM4GetElem(&v_mat, 3, 1);
	float y = vmathM4GetElem(&q_sprite->model_mat, 3, 1);
	y -= 10.1;
	// vmathM4SetElem(&v_mat, 3, 1, y);
	vmathM4SetElem(&q_sprite->model_mat, 3, 1, y);
	// printf("x:%f\n",x);
    }
}

void variable_update(double alpha) {}

void variable_render(double alpha) {
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader_bind_program(shader);

    vmathM4GetData(&q_sprite->model_mat);
    vmathM4GetData(&v_mat);
    vmathM4GetData(&p_mat);

    glUniformMatrix4fv(m_mat_loc, 1, GL_FALSE,
		       vmathM4GetData(&q_sprite->model_mat));
    glUniformMatrix4fv(p_mat_loc, 1, GL_FALSE, vmathM4GetData(&p_mat));
    glUniformMatrix4fv(v_mat_loc, 1, GL_FALSE, vmathM4GetData(&v_mat));

    glActiveTexture(GL_TEXTURE0);
    glUniform1i(tex_loc, 0);
    glBindTexture(GL_TEXTURE_2D, q_sprite->texture_id);
    //printf("tex:%d\n", tex_loc);

    // vmathM4Prints(&p_mat, "proj");
    // vmathM4Prints(&v_mat, "view");
    // vmathM4Prints(&q_sprite->model_mat, "model");

    sprite_bind_render(q_sprite);
}

void resize(int w, int h) {}

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
	//l = r = u = d = -1;
	// printf("released\n");
    }
}

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
    aspect = width / height;

    ce_init(width, height, "test title", 60.0, ptrs, argc[0]);
    return 0;
}
