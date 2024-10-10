#pragma once

int new_shader(unsigned int *shader, const char *path, int type);

int new_program(unsigned int *program, const char *vertex_shader, const char *fragment_shader);
