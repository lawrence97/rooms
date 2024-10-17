#pragma once

#include "vector.h"

typedef struct debug_t {
	unsigned int vao;
	unsigned int vbo;
	float colour[4];
	unsigned int capacity;
	unsigned int n;
	float *points;
} debug_t;

int new_debug(debug_t *db);

int append_debug(debug_t *db, vec2 new);
