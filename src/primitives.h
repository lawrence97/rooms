#pragma once

#include "maths.h"

typedef struct tile_t {
	vec3 origin_pos;
	vec2 dim_pos;
	vec2 origin_tex;
	vec2 dim_tex;
	vec4 colours[4];
} tile_t;

typedef struct vertices_t {
	float positions[6 * 3];
	float texcoords[6 * 2];
	float colours[6 * 4];
} vertices_t;

void new_vertices(vertices_t *verts, tile_t *tile);
