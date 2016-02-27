#include "shader.h"

shader_fx *shader_create_from_file(char* v_name, char* f_name) {
    char* v_path, *f_path;
    char* v_data, *f_data;

    get_resource(&v_path, v_name);
    get_resource(&f_path, f_name);
    read_file(&v_data, v_path);
    read_file(&f_data, f_path);

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, (const char**)&v_data, NULL);
    glCompileShader(vertexShader);

    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("ERROR VERTEX COMPILATION_FAILED %s\n", infoLog);
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, (const char**)&f_data, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("ERROR FRAGMENT COMPILATION_FAILED %s\n", infoLog);
    }

    GLuint shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("ERROR SHADER PROGRAM COMPILATION_FAILED %s\n", infoLog);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    free(v_path); free(f_path);
    free(v_data); free(f_data);

    shader_fx* out = calloc(1, sizeof(shader_fx));
    out->shader_program_handle = shaderProgram;
    return out;
}

shader_fx *shader_create_from_string(char* v_string, char* f_string) {

    shader_fx* out = calloc(1, sizeof(shader_fx));
    return out;
}

GLint shader_get_uniform_location(shader_fx* shader, char* u) {
    GLint stat = glGetUniformLocation(shader->shader_program_handle, u);
    if (-1 != stat) {
        glGetUniformLocation(shader->shader_program_handle, u);
        return stat;
    }
    return stat;
}

GLint shader_get_attribute_location(shader_fx* shader, char* a) {
    GLint stat = glGetAttribLocation(shader->shader_program_handle, a);
    if (-1 != stat) {
        glGetAttribLocation(shader->shader_program_handle, a);
        return stat;
    }
    return stat;
}

void shader_bind_program(shader_fx* in) {
    glUseProgram(in->shader_program_handle);
}

void shader_cleanup(shader_fx* in) {
    glDeleteBuffers(1, &in->shader_program_handle);
    free(in);
}
