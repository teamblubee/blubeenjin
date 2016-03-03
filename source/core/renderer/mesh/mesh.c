#include "mesh.h"

mesh* mesh_new_ptr(mesh_draw_mode mode, mesh_primitive_type primite_type,
                   vertex_layout* layout) {
    mesh* out = calloc(1, sizeof(mesh));
    vertex_layout_duplicate(&out->vertex_layout, &layout);
    out->draw_mode = mode;
    out->primitive_type = primite_type;
    out->vertex_count = -1;
    out->index_count = -1;
    return out;
}

mesh* mesh_set_vertex_data(mesh* out, float* data, size_t count) {
    out->vertex_data = calloc(count, sizeof(data[0]));
    memcpy(out->vertex_data, data, sizeof(data[0]) * count);
    out->vertex_count = count;

    return out;
}

mesh* mesh_update_vertex_data(mesh* out, float* data, size_t start,
                              size_t count) {
    if (out->draw_mode == STATIC) {
        printf(
            "you choose static geometry, probably shouldn't be calling this\n");
    }
    memcpy(out->vertex_data + start, data, sizeof(data[0]) * count);
    out->vertex_data = realloc(out->vertex_data, sizeof(data[0]) * count);

    printf(
        "------------------------------------------------------------\n\
            you've got to test this implement mesh_update_vertex_data\
         ------------------------------------------------------------\n");
    return out;
}

mesh* mesh_set_index_data(mesh* out, short* data, size_t count,
                          size_t elements) {
    out->index_data = calloc(count, sizeof(data[0]));
    memcpy(out->index_data, data, sizeof(data[0]) * count);
    out->index_count = count;
    out->elements_to_render = elements;

    return out;
}


size_t mesh_get_index_count(mesh* in) { return in->index_count; }

size_t mesh_get_vertex_stride(mesh* in) {
    return (in->vertex_layout->vertex_layout_attribute_combined_count *
            sizeof(float));
}

size_t mesh_get_element_count(mesh* in) { return in->elements_to_render; }

size_t mesh_get_vertex_sizebytes(mesh* in) {
    return (sizeof(float) * in->vertex_count);
}

size_t mesh_get_index_sizebytes(mesh* in) {
    return (sizeof(short) * in->index_count);
}

size_t mesh_get_vertex_count(mesh* in) { return in->vertex_count; }

short* mesh_get_index_data(mesh* in) { return in->index_data; }

float* mesh_get_vertex_data(mesh* in) { return in->vertex_data; }

mesh_draw_mode mesh_get_draw_mode(mesh* in) { return in->draw_mode; }

mesh_primitive_type mesh_get_primitive_type(mesh* in) {
    return in->primitive_type;
}

void mesh_cleanup(mesh* in) {
    free(in->vertex_data);
    in->vertex_data = 0;
    in->vertex_count = 0;
    free(in->index_data);
    in->index_data = 0;
    in->index_count = 0;
    vertex_layout_cleanup(in->vertex_layout);
    in->elements_to_render = 0;
    free(in);
}
