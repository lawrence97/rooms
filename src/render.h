#pragma once

#include "primitives.h"

typedef struct handles_t {
	unsigned int vao;
	unsigned int pbo;
	unsigned int tbo;
	unsigned int cbo;
} handles_t;

void new_buffers(handles_t *handles, vertices_t *verts);
