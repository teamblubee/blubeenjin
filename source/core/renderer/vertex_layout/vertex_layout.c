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
            break;
        case TEXCOORD1:
            return TEXCOORD1;
            break;
        case TEXCOORD2:
            return TEXCOORD2;
            break;
        case TEXCOORD3:
            return TEXCOORD3;
            break;
    }
}

static void copy_vertex_attribute(vertex_attribute* to,
                                  vertex_attribute* from) {
    memcpy(&to->vertex_attribute, &from->vertex_attribute,
           sizeof(vertex_attribute));
    memcpy(&to->vertex_attribute_sizebytes, &from->vertex_attribute_sizebytes,
           sizeof(size_t));
    memcpy(&to->vertex_attribute_count, &from->vertex_attribute_count,
           sizeof(size_t));
}

char* vertex_attribute_type_getstring(vertex_attribute_type* a) {
    return va_getstring(a);
}
vertex_attribute_type vertex_attribute_type_gettype(vertex_attribute_type* a) {
    return va_gettype(a);
}

void vertex_attribute_type_print(vertex_attribute_type* a) {
    printf("%s\n", va_getstring(a));
}

void vertex_attribute_type_print_s(vertex_attribute_type* a, char* tag) {
    printf("%s %s\n", tag, va_getstring(a));
}

vertex_attribute vertex_attribute_new(vertex_attribute_type type,
                                      size_t count) {
    vertex_attribute out;
    out.vertex_attribute = type;
    out.vertex_attribute_count = count;
    out.vertex_attribute_sizebytes = (sizeof(float) * count);

    return out;
}

char* vertex_attribute_get_string(vertex_attribute* a) {
    return va_getstring(&a->vertex_attribute);
}

size_t vertex_attribute_get_count(vertex_attribute* a) {
    return a->vertex_attribute_count;
}

size_t vertex_attribute_get_sizebytes(vertex_attribute* a) {
    return (a->vertex_attribute_sizebytes);
}

vertex_attribute_type vertex_attribute_get_type(vertex_attribute* a);

void vertex_attribute_print(vertex_attribute* a) {
    printf("TYPE: %s COUNT: %zu SIZE BYTES: %zu\n",
           va_getstring(&a->vertex_attribute), a->vertex_attribute_count,
           a->vertex_attribute_sizebytes);
}
void vertex_attribute_print_s(vertex_attribute* a, char* tag) {
    printf("%s TYPE:%s COUNT:%zu SIZE BYTES:%zu\n", tag,
           va_getstring(&a->vertex_attribute), a->vertex_attribute_count,
           a->vertex_attribute_sizebytes);
}

vertex_layout* vertex_layout_new_ptr(void) {
    vertex_layout* out = calloc(1, sizeof(vertex_layout));
    out->vertex_layout_attributes = calloc(1, sizeof(vertex_attribute*));
    out->vertex_layout_attribute_sizebytes = 0;
    out->vertex_layout_attribute_combined_count = 0;
    out->vertex_layout_attribute_combined_count = 0;
    out->vertex_layout_attribute_count = 0;
    return out;
}

vertex_layout* vertex_layout_pushback(vertex_layout* l, vertex_attribute* a) {
    size_t count = l->vertex_layout_attribute_count;

    l->vertex_layout_attributes = realloc(
        l->vertex_layout_attributes, (count + 1) * (sizeof(vertex_attribute*)));
    l->vertex_layout_attributes[count] = calloc(1, sizeof(vertex_attribute*));
    copy_vertex_attribute(l->vertex_layout_attributes[count], a);
    l->vertex_layout_attribute_count++;
    l->vertex_layout_attribute_combined_count += a->vertex_attribute_count;
    l->vertex_layout_attribute_sizebytes += a->vertex_attribute_sizebytes;
    return l;
}

vertex_layout** vertex_layout_duplicate(vertex_layout** to,
                                        vertex_layout** from) {
    size_t vl_attrib_count = (*from)->vertex_layout_attribute_count;
    size_t vertex_layout_sizebytes = (*from)->vertex_layout_attribute_sizebytes;
    size_t vertex_attribute_count;
    size_t vertex_attribute_sizebytes;
    vertex_attribute_type vertex_attribute_type_c;
    (*to) = calloc(1, sizeof(vertex_layout));
    (*to)->vertex_layout_attributes =
        calloc(vl_attrib_count, sizeof(vertex_attribute*));
    for (size_t i = 0; i < vl_attrib_count; i++) {
        (*to)->vertex_layout_attributes[i] =
            calloc(1, sizeof(vertex_attribute));
        memcpy(&(*to)->vertex_layout_attributes[i]->vertex_attribute,
               &(*from)->vertex_layout_attributes[i]->vertex_attribute,
               sizeof(vertex_attribute));

        memcpy(
            &(*to)->vertex_layout_attributes[i]->vertex_attribute_sizebytes,
            &(*from)->vertex_layout_attributes[i]->vertex_attribute_sizebytes,
            sizeof(size_t));

        memcpy(&(*to)->vertex_layout_attributes[i]->vertex_attribute_count,
               &(*from)->vertex_layout_attributes[i]->vertex_attribute_count,
               sizeof(size_t));
    }
    memcpy(&(*to)->vertex_layout_attribute_sizebytes,
           &(*from)->vertex_layout_attribute_sizebytes, sizeof(size_t));
    ;
    memcpy(&(*to)->vertex_layout_attribute_count,
           &(*from)->vertex_layout_attribute_count, sizeof(size_t));
    memcpy(&(*to)->vertex_layout_attribute_combined_count,
           &(*from)->vertex_layout_attribute_combined_count, sizeof(size_t));
    return to;
}

vertex_attribute* vertex_layout_getattribute(vertex_layout* in, size_t index) {
    return in->vertex_layout_attributes[index];
}

char* vertex_layout_getattributestring(vertex_layout* in, size_t index) {
    return va_getstring(&in->vertex_layout_attributes[index]->vertex_attribute);
}

vertex_attribute_type vertex_layout_getattributetype(vertex_layout* in,
                                                     size_t index) {
    return va_gettype(&in->vertex_layout_attributes[index]->vertex_attribute);
}

void vertex_layout_print(vertex_layout* in) {
    size_t count = in->vertex_layout_attribute_count;
    printf("---------------------------------------------------------\n");
    printf(
        "vertex layout "
        "\n---------------------------------------------------------\n");
    for (size_t i = 0; i < count; i++) {
        vertex_attribute_print(in->vertex_layout_attributes[i]);
    }
    printf("SIZE BYTES PER VERTEX: %zu VERTEX ATTRIBUTE COUNT: %zu",
           in->vertex_layout_attribute_sizebytes,
           in->vertex_layout_attribute_count);
    printf("\n=========================================================\n\n");
}

void vertex_layout_print_s(vertex_layout* in, char* tag) {
    size_t count = in->vertex_layout_attribute_count;
    printf("---------------------------------------------------------\n");
    printf(
        "vertex layout ( %s "
        ")\n---------------------------------------------------------\n",
        tag);
    for (size_t i = 0; i < count; i++) {
        vertex_attribute_print(in->vertex_layout_attributes[i]);
    }
    printf("SIZE BYTES PER VERTEX: %zu VERTEX ATTRIBUTE COUNT: %zu",
           in->vertex_layout_attribute_sizebytes,
           in->vertex_layout_attribute_count);
    printf("\n=========================================================\n\n");
}

void vertex_layout_cleanup(vertex_layout* l) {
    size_t count = l->vertex_layout_attribute_count;
    for (size_t i = 0; i < count; i++) {
        free(l->vertex_layout_attributes[i]);
        l->vertex_layout_attributes[i] = 0;
    }
    free(l->vertex_layout_attributes);
    l->vertex_layout_attributes = 0;
    free(l);
    l = 0;
}
