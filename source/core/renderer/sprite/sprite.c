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

    vmathV3MakeFromElems(&out->piv, t->width * 0.5f, t->height * 0.5f, 1.0);
    vmathV3MakeFromElems(&out->scale, out->piv.x, out->piv.y, out->piv.z);

    vmathM4MakeScale(&out->model_mat, &out->scale);

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

// state = current_state * alpha + prev_state * (1.0 - alpha);

sprite* sprite_add_position(sprite* out, float x, float y) {
    out->p_pos.x = out->pos.x;
    out->p_pos.y = out->pos.y;
    out->p_pos.z = out->pos.z;

    out->pos.x = out->pos.x + x;
    out->pos.y = out->pos.y + y;
    out->pos.z = out->z_index;
    return out;
}

void sprite_bind_render(sprite* out, shader_fx* s, GLuint tex_loc, GLuint m_mat_loc, float al) {
    // state = current_state * alpha + prev_state * (1.0 - alpha);
    float tx, ty;

    tx = (out->pos.x * al) + (out->p_pos.x * (1.0f - al));
    ty = (out->pos.y * al) + (out->p_pos.y * (1.0f - al));
    //printf("interpolated x:%f interpolated y:%f\n",tx, ty);
    out->p_pos.x = tx;
    out->p_pos.y = ty;

    vmathM4SetElem(&out->model_mat, 3, 0, (out->scale.x) + (tx));
    vmathM4SetElem(&out->model_mat, 3, 1, (out->scale.y) + (ty));

    //vmathV3Prints(&out->p_pos, "prev");
    //vmathV3Prints(&out->pos, "pos");

    shader_bind_program(s);
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(tex_loc, 0);

    glBindVertexArray(out->sprite_mesh_vao_handle);

    glUniformMatrix4fv(m_mat_loc, 1, GL_FALSE, vmathM4GetData(&out->model_mat));
    //vmathM4Prints(&out->model_mat, "model");

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
