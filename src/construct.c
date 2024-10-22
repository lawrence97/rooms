#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "construct.h"

#define CONSTRUCT_CAPACITY 100
#define DEBUG_POINTS_SIZE 3

#define RADIUS_EXTENSION 1.4
#define THETA_CHANGE_RANGE 0.6

int new_construct(construct_t *c) {

	unsigned int buffer_full_capacity = CONSTRUCT_CAPACITY + DEBUG_POINTS_SIZE;
	c->n_tile = 0;
	c->capacity_tile = buffer_full_capacity;

	c->tiles = (tile_t *)calloc(c->capacity_tile, sizeof(tile_t));
	if (c->tiles == NULL) {
		c->capacity_tile = 0;
		return -1;
	}
	return 0;
}

void update_construct(construct_t *c, construct_opts *prev, construct_opts *next) {

	int n = rand_int(4, 10);
	float dim = 16.0f;
	float dim_texture = .5f;
	double theta = prev->theta + ((rand_int(-4, 4) / 12.0f) * THETA_CHANGE_RANGE * M_PI);
	float radius = sqrt(2.0f * (n * .5f) * dim) + (n * dim * RADIUS_EXTENSION);
	float dx = radius * cos(theta);
	;
	float dy = radius * sin(theta);
	vec2 centre;
	centre.x1 = prev->exit.x1 + dx;
	centre.x2 = prev->exit.x2 + dy;

	c->debug.enabled = 1;

	c->n_tile = 0;

	vec2 subtexture0 = {.25f, .25f};
	vec2 subtexture1 = {.75f, .25f};
	vec2 subtexture2 = {.25f, .75f};
	vec2 subtexture3 = {.75f, .75f};
	vec2 range_textures[4] = {subtexture0, subtexture1, subtexture2, subtexture3};

	vec4 red = {1.0f, 0.0f, 0.0f, 1.0f};
	vec4 green = {0.0f, 1.0f, 0.0f, 1.0f};
	vec4 blue = {0.0f, 0.0f, 1.0f, 1.0f};
	vec4 white = {1.0f, 1.0f, 1.0f, 1.0f};
	vec4 range_colours[4] = {red, green, blue, white};

	vec2 anchor;
	anchor.x1 = centre.x1 - (n * .5f * dim) + (dim * .5f);
	anchor.x2 = centre.x2 + (n * .5f * 16.0f) - (dim * .5f);

	int index = 0;
	for (int i = 0; i < n; i++) {
		vec2 active;
		active.x2 = anchor.x2 - (i * dim);
		for (int j = 0; j < n; j++) {
			active.x1 = anchor.x1 + (j * dim);

			c->tiles[(i * n) + j].offset = active;
			c->tiles[(i * n) + j].dimension = (vec2){dim, dim};
			c->tiles[(i * n) + j].dimension_texture = (vec2){dim_texture, dim_texture};
			c->tiles[(i * n) + j].offset_texture = range_textures[1];
			c->tiles[(i * n) + j].colour = range_colours[3];

			c->n_tile++;
			index++;
		}
	}

	// north face
	int doors_north[n - 2];
	for (int i = 0; i < n - 2; i++) {
		doors_north[i] = (i + 1);
	}

	// south face
	int doors_south[n - 2];
	for (int i = 0; i < n - 2; i++) {
		doors_south[i] = ((n * n) - 2) - i;
	}

	// west face
	int doors_west[n - 2];
	for (int i = 0; i < n - 2; i++) {
		doors_west[i] = (1 + i) * n;
	}

	// east face
	int doors_east[n - 2];
	for (int i = 0; i < n - 2; i++) {
		doors_east[i] = (n * n) - 1 - (n * (i + 1));
	}

	int *east_west[2] = {doors_east, doors_west};
	int *west_east[2] = {doors_west, doors_east};
	int *north_south[2] = {doors_north, doors_south};
	int *south_north[2] = {doors_south, doors_north};

	int **directions[4] = {east_west, west_east, north_south, south_north};

	int chosen_direction = rand_int(0, 3);
	int chosen_face = 0;
	int chosen_door = rand_int(0, n - 2 - 1);

	float adx = fabs(dx);
	float ady = fabs(dy);
	if (ady >= adx) {
		if (dy < 0) {
			chosen_direction = 2;
		} else {
			chosen_direction = 3;
		}
	} else {
		if (dx < 0) {
			chosen_direction = 0;
		} else {
			chosen_direction = 1;
		}
	}

	int entry = directions[chosen_direction][chosen_face][chosen_door];

	chosen_face++;
	chosen_door = rand_int(0, n - 2 - 1);

	int exit = directions[chosen_direction][chosen_face][chosen_door];

	c->tiles[entry].colour = range_colours[2];
	c->tiles[exit].colour = range_colours[0];

	// debug points
	if (c->debug.enabled != 1) {
		return;
	}

	float scale_debug = .4f;
	vec2 texture_debug = range_textures[0];
	vec4 magenta = {1.0f, 0.0f, 1.0f, 1.0f};
	vec4 yellow = {1.0f, 1.0f, 0.0f, 1.0f};

	c->debug.centre = centre;
	c->debug.entry = c->tiles[entry].offset;
	c->debug.exit = c->tiles[exit].offset;

	vec2 debug_points[3] = {c->debug.centre, c->debug.entry, c->debug.exit};

	for (int m = 0; m < 3; m++) {
		int ndb = c->n_tile;
		if (m == 0) {
			c->tiles[ndb].colour = magenta;
		} else {
			c->tiles[ndb].colour = yellow;
		}
		c->tiles[ndb].dimension = (vec2){dim * scale_debug, dim * scale_debug};
		c->tiles[ndb].dimension_texture = (vec2){dim_texture};
		c->tiles[ndb].offset_texture = texture_debug;

		c->tiles[ndb].offset = debug_points[m];
		c->n_tile++;
	}

	next->centre = centre;
	next->dim = (vec2){dim};
	next->dim_texture = (vec2){dim_texture};
	next->entry = c->tiles[entry].offset;
	next->exit = c->tiles[exit].offset;
	next->n = n;
	next->radius = radius;
	next->theta = theta;

	return;
}

void free_construct(construct_t *c) {

	if (c->tiles != NULL) {
		free(c->tiles);
	}
	c->tiles = NULL;
	c->n_tile = 0;
	c->capacity_tile = 0;
	return;
}

int rand_int(int min, int max) {

	if (min == max) {
		return min;
	}

	int rn = rand();
	int rmod = min + (rn % (max + 1 - min));
	return rmod;
}
