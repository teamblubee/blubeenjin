#ifndef VERTEX_LAYOUT_H
#define VERTEX_LAYOUT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "glad.h"

typedef enum {
    POSITION,
    COLOR,
    NORMAL,
    TEXCOORD0,
    TEXCOORD1,
    TEXCOORD2,
    TEXCOORD3
} vertex_attribute_type;

typedef struct {
    vertex_attribute_type vertex_attribute;
    size_t vertex_attribute_count;
    size_t vertex_attribute_sizebytes;
} vertex_attribute;

typedef struct {
    vertex_attribute** vertex_layout_attributes;
    size_t vertex_layout_attribute_count;
    size_t vertex_layout_attribute_sizebytes;
} vertex_layout;

char* vertex_attribute_type_getstring(vertex_attribute_type* a);
vertex_attribute_type vertex_attribute_type_gettype(vertex_attribute_type* a);
void vertex_attribute_type_print(vertex_attribute_type* a);
void vertex_attribute_type_print_s(vertex_attribute_type* a, char* tag);


vertex_attribute vertex_attribute_new(vertex_attribute_type type, size_t count);
char* vertex_attribute_get_string(vertex_attribute* a);
size_t vertex_attribute_get_count(vertex_attribute* a);
size_t vertex_attribute_get_sizebytes(vertex_attribute* a);
vertex_attribute_type vertex_attribute_get_type(vertex_attribute* a);
void vertex_attribute_print(vertex_attribute* a);
void vertex_attribute_print_s(vertex_attribute* a, char* tag);


vertex_layout* vertex_layout_new_ptr(void);
vertex_layout* vertex_layout_pushback(vertex_layout* l, vertex_attribute* a);
vertex_layout** vertex_layout_duplicate(vertex_layout** to, vertex_layout** from);

vertex_attribute* vertex_layout_getattribute(vertex_layout* in, size_t index);
char* vertex_layout_getattributestring(vertex_layout* in, size_t index);
vertex_attribute_type vertex_layout_getattributetype(vertex_layout* in,
                                                     size_t index);

void vertex_layout_print(vertex_layout* in);
void vertex_layout_print_s(vertex_layout* in, char* tag);
void vertex_layout_cleanup(vertex_layout* l);

#endif /* VERTEX_LAYOUT_H */
