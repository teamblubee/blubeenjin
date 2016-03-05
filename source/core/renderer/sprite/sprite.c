#include "sprite.h"

float v_data[] = {-1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		  -1.0f, 1.0f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		  1.0f,  1.0f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		  1.0f,  -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f};

short i_data[] = {0, 1, 2, 0, 2, 3};

static void bind_vertex_data(sprite* out) {
    glBindVertexArray(out->sprite_mesh_vao_handle);

    glBindBuffer(GL_ARRAY_BUFFER, out->vertex_buffer_handle);
    glBufferData(GL_ARRAY_BUFFER, mesh_get_vertex_count(out->sprite_mesh),
		 mesh_get_vertex_data(out->sprite_mesh),
		 out->sprite_mesh->draw_mode);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
			  mesh_get_vertex_stride(out->sprite_mesh), (GLvoid*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
			  mesh_get_vertex_stride(out->sprite_mesh),
			  (GLvoid*)(3 * sizeof(float)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
			  mesh_get_vertex_stride(out->sprite_mesh),
			  (GLvoid*)(7 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

static void bind_index_data(sprite* out) {
    glBindVertexArray(out->sprite_mesh_vao_handle);

    glBindBuffer(GL_ARRAY_BUFFER, out->vertex_buffer_handle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, out->index_buffer_handle,
		 mesh_get_index_data(out->sprite_mesh),
		 out->sprite_mesh->draw_mode);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

sprite* sprite_new_ptr(texture* t, float x, float y) {
    sprite* out = calloc(1, sizeof(sprite));
    out->texture_id = t->texture_id;
    out->z_index = 0;

    glGenBuffers(1, &out->sprite_mesh_vao_handle);
    glGenBuffers(1, &out->vertex_buffer_handle);
    glGenBuffers(1, &out->index_buffer_handle);

    vertex_attribute pos = vertex_attribute_new(POSITION, 3);
    vertex_attribute col = vertex_attribute_new(POSITION, 4);
    vertex_attribute tex = vertex_attribute_new(TEXCOORD0, 2);

    vertex_layout* sprite_vl = vertex_layout_new_ptr();
    vertex_layout_pushback(sprite_vl, &pos);
    vertex_layout_pushback(sprite_vl, &col);
    vertex_layout_pushback(sprite_vl, &tex);

    out->sprite_mesh = mesh_new_ptr(STATIC, TRIANGLES, sprite_vl);

    mesh_set_vertex_data(out->sprite_mesh, v_data,
			 (sizeof(v_data) / sizeof(v_data[0])));
    mesh_set_index_data(out->sprite_mesh, i_data,
			(sizeof(i_data) / sizeof(i_data[0])), 6);

    vertex_layout_cleanup(sprite_vl);

    vmathV3MakeFromElems(&out->scale, t->width * 0.5f, t->height * 0.5f, 1);
    vmathM4SetElem(&out->model_mat, 0, 0, out->scale.x);
    vmathM4SetElem(&out->model_mat, 1, 1, out->scale.y);
    vmathM4SetElem(&out->model_mat, 2, 2, 1);
    vmathM4SetElem(&out->model_mat, 3, 3, 1);
    // vmathM4MakeScale(&out->model_mat, &out->scale);
    vmathM4SetElem(&out->model_mat, 3, 0, (-out->scale.x) - (x));
    vmathM4SetElem(&out->model_mat, 3, 1, (out->scale.y) + (y));
    // vmathM4Prints(&out->model_mat, "model");

    bind_vertex_data(out);
    bind_index_data(out);

    return out;
}

GLuint sprite_get_vertex_buffer_handle(sprite* in) {
    return in->vertex_buffer_handle;
}

GLuint sprite_get_index_buffer_handle(sprite* in) {
    return in->index_buffer_handle;
}

sprite* sprite_set_z_index(sprite* out, int index) {
   out->z_index = index;
   return out;
}

sprite* sprite_set_position(sprite* out, float x, float y) {
    vmathV3Copy(&out->p_pos, &out->pos);
    vmathV3MakeFromElems(&out->pos, x, y, out->z_index);
    return out;
}

void sprite_bind_render(sprite* out, shader_fx* s, GLuint tex_loc) {
    shader_bind_program(s);
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(tex_loc, 0);

    glBindVertexArray(out->sprite_mesh_vao_handle);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, out->texture_id);

    glBindBuffer(GL_ARRAY_BUFFER, out->vertex_buffer_handle);
    glBufferData(GL_ARRAY_BUFFER, mesh_get_vertex_sizebytes(out->sprite_mesh),
		 mesh_get_vertex_data(out->sprite_mesh),
		 out->sprite_mesh->draw_mode);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, out->index_buffer_handle);
    glBufferData(
	GL_ELEMENT_ARRAY_BUFFER, mesh_get_index_sizebytes(out->sprite_mesh),
	mesh_get_index_data(out->sprite_mesh), out->sprite_mesh->draw_mode);

    glDrawElements(GL_TRIANGLES, mesh_get_element_count(out->sprite_mesh),
		   GL_UNSIGNED_SHORT, 0);
}

void sprite_cleanup(sprite* in) {
    mesh_cleanup(in->sprite_mesh);
    glDeleteBuffers(1, &in->vertex_buffer_handle);
    in->vertex_buffer_handle = -1;
    glDeleteBuffers(1, &in->index_buffer_handle);
    in->index_buffer_handle = -1;
    glDeleteBuffers(1, &in->sprite_mesh_vao_handle);
    in->sprite_mesh_vao_handle = -1;
    glDeleteBuffers(1, &in->texture_id);
    in->texture_id = -1;
    free(in);
}
