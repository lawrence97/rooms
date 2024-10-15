#pragma once

#include "vector.h"

typedef struct handles_t {
	unsigned int vao;
	unsigned int pbo;
	unsigned int tbo;
	unsigned int cbo;
} handles_t;

typedef struct tile_t {
	vec3 origin_pos;
	vec2 dim_pos;
	vec2 origin_tex;
	vec2 dim_tex;
	vec4 colours[4];
} tile_t;

typedef struct batch_t {
	handles_t handles;
	unsigned int n;
	float *positions;
	float *texcoords;
	float *colours;
} batch_t;

int new_batch(batch_t *batch, tile_t *tiles, unsigned int ntiles);
