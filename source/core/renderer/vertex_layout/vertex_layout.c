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

