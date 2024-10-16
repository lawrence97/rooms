#pragma once

#include "vector.h"

typedef struct tile_t {
	vec3 coord;
	vec2 dimension;
	vec2 coord_tex;
	vec2 dimension_tex;
	vec4 colours[4];
} tile_t;

typedef struct construct_t {
	vec3 centre;
	vec2 bounds[4];
	unsigned int n;
	tile_t *tiles;
} construct_t;

int new_construct(construct_t *construct);

int rand_int(int min, int max);
