#pragma once

#include "vector.h"

typedef struct tile_t {
	vec3 origin_pos;
	vec2 dim_pos;
	vec2 origin_tex;
	vec2 dim_tex;
	vec4 colours[4];
} tile_t;

typedef struct blueprint_t {
	unsigned int n;
	tile_t *tiles;
} blueprint_t;

int new_blueprint(blueprint_t *blueprint, unsigned int n);

int rand_int(int min, int max);
