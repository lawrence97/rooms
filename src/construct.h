#pragma once

#include "vector.h"

typedef struct tile_t {
	vec2 offset;
	vec2 dimension;
	vec2 offset_texture;
	vec2 dimension_texture;
	vec4 colour;
} tile_t;

typedef struct debug_t {
	int enabled;
	vec2 centre;
	vec2 entry;
	vec2 exit;
} debug_t;

typedef struct construct_opts {
	vec2 centre;
	vec2 entry;
	vec2 exit;
	vec2 dim;
	vec2 dim_texture;
	unsigned int n;
	float radius;
	double theta;
} construct_opts;

typedef struct construct_t {
	unsigned int capacity_tile;
	unsigned int n_tile;
	tile_t *tiles;
	debug_t debug;
} construct_t;

int new_construct(construct_t *c);

void update_construct(construct_t *c, construct_opts *prev, construct_opts *next);

void free_construct(construct_t *c);

int rand_int(int min, int max);
