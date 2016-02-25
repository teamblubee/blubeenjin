#include "vertex_layout.h"

static char* va_getstring(vertex_attribute_type* t) {
    switch (*t) {
        case POSITION:
            return "Position";
            break;
        case COLOR:
            return "Color";
            break;
        case NORMAL:
            return "Normal";
            break;
        case TEXCOORD0:
            return "TexCoord0";
            break;
        case TEXCOORD1:
            return "TexCoord1";
            break;
        case TEXCOORD2:
            return "TexCoord2";
            break;
        case TEXCOORD3:
            return "TexCoord3";
            break;
    }
}

static vertex_attribute_type va_gettype(vertex_attribute_type* t) {
    switch (*t) {
        case POSITION:
            return POSITION;
            break;
        case COLOR:
            return COLOR;
            break;
        case NORMAL:
            return NORMAL;
            break;
        case TEXCOORD0:
            return TEXCOORD0;
        case TEXCOORD1:
            return TEXCOORD1;
        case TEXCOORD2:
            return TEXCOORD2;
        case TEXCOORD3:
            return TEXCOORD3;
            break;
    }
}

static void copy_vertex_layout(vertex_layout** to, vertex_attribute* from, size_t index) {
    (*to)->vertex_layout_attributes[0]->vertex_attribute_size =
        from->vertex_attribute_size;

    (*to)->vertex_layout_attributes[index]->vertex_attribute_count =
        from->vertex_attribute_count;
    (*to)->vertex_layout_attributes[index]->vertex_attribute =
        from->vertex_attribute;
    (*to)->vertex_layout_attribute_count++;
}


char* vertex_attribute_type_getstring(vertex_attribute_type* a) {
    return va_getstring(a);
}

vertex_attribute_type vertex_attribute_type_gettype(vertex_attribute_type* a) {
    return va_gettype(a);
}

vertex_attribute* vertex_attribute_new_ptr(vertex_attribute_type type,
                                           size_t count) {
    vertex_attribute* out = calloc(1, sizeof(vertex_attribute));
    out->vertex_attribute = type;
    out->vertex_attribute_count = count;
    out->vertex_attribute_size = count * sizeof(float);
    return out;
}

vertex_attribute vertex_attribute_new(vertex_attribute_type type,
                                      size_t count) {
    vertex_attribute out;
    out.vertex_attribute = type;
    out.vertex_attribute_count = count;
    out.vertex_attribute_size = count * sizeof(float);
    return out;
}

char* vertex_attribute_getstring(vertex_attribute* a) {
    return va_getstring(&a->vertex_attribute);
}

vertex_attribute_type vertex_attribute_gettype(vertex_attribute* a) {
    return va_gettype(&a->vertex_attribute);
}

size_t vertex_attribute_getcount(vertex_attribute* a) {
    return a->vertex_attribute_count;
}

size_t vertex_attribute_sizebytes(vertex_attribute* a) {
    return a->vertex_attribute_size * a->vertex_attribute_count;
}

void vertex_attribute_cleanup(vertex_attribute* a) { free(a); }

vertex_layout* vertex_layout_new_packed_ptr(vertex_attribute** a,
                                            size_t count) {
    vertex_layout* out = calloc(1, sizeof(vertex_layout));
    out->vertex_layout_attributes = calloc(count, sizeof(vertex_attribute));
    out->vertex_layout_attribute_count = count;

    for (size_t i = 0; i < count; i++) {
        out->vertex_layout_attributes[i] = calloc(1, sizeof(vertex_attribute));
        copy_vertex_layout(&out, *a, i);
    }
    return out;
}

vertex_layout* vertex_layout_new_ptr(void) {
    vertex_layout* out = calloc(1, sizeof(vertex_layout));
    return out;
}

vertex_layout vertex_layout_new(void) {
    vertex_layout out;
    return out;
}

vertex_layout* vertex_layout_pushback_ptr(vertex_layout* l,
                                          vertex_attribute* a) {
    size_t index = l->vertex_layout_attribute_count;
    if (index == 0) {
        l->vertex_layout_attributes = calloc(1, sizeof(vertex_attribute));
    }
    copy_vertex_layout(&l, a, index);
    return l;
}

vertex_layout* vertex_layout_pushback(vertex_layout* l, vertex_attribute a) {
    size_t index = l->vertex_layout_attribute_count;
    if (index == 0) {
        l->vertex_layout_attributes = calloc(1, sizeof(vertex_attribute));
    }
    l->vertex_layout_attributes[index] = calloc(1, sizeof(vertex_attribute));
    copy_vertex_layout(&l, &a, index);
    return l;
}

vertex_attribute* vertex_layout_getattribute(vertex_layout* in, size_t index) {
    return in->vertex_layout_attributes[index];
}

char* vertex_layout_getattributestring(vertex_layout* in, size_t index) {
    return vertex_attribute_getstring(in->vertex_layout_attributes[index]);
}

vertex_attribute_type vertex_layout_getattributetype(vertex_layout* in,
                                                     size_t index) {
    return vertex_attribute_gettype(in->vertex_layout_attributes[index]);
}

void vertex_layout_cleanup(vertex_layout* l) {
    size_t count = l->vertex_layout_attribute_count;

    for (int i = 0; i < count; i++) {
        free(l->vertex_layout_attributes[i]);
    };
    free(l->vertex_layout_attributes);
    free(l);
}
