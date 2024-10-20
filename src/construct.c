#include <stdlib.h>

#include "construct.h"

#define CONSTRUCT_CAPACITY 100
#define DEBUG_POINTS_SIZE 3

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

void update_construct(construct_t *c) {

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

	float dim = 16.0f;
	float dim_texture = .5f;

	float n = rand_int(5, 10);

	int open_index = rand_int(1, n - 2);
	int close_index = (n * n) - rand_int(2, n - 1);

	float x = (float)rand_int(-280, 280);
	float y = (float)rand_int(-280, 280);
	vec2 centre = {x, y};

	vec2 anchor;
	anchor.x1 = x - (n * .5f * dim) + (dim * .5f);
	anchor.x2 = y + (n * .5f * 16.0f) - (dim * .5f);

	int index = 0;
	for (int i = 0; i < n; i++) {
		vec2 active;
		active.x2 = anchor.x2 - (i * dim);
		for (int j = 0; j < n; j++) {
			active.x1 = anchor.x1 + (j * dim);

			c->tiles[(i * (int)n) + j].offset = active;
			c->tiles[(i * (int)n) + j].dimension = (vec2){dim, dim};
			c->tiles[(i * (int)n) + j].dimension_texture = (vec2){dim_texture, dim_texture};

			int texture_picker;
			if (index == open_index || index == close_index) {
				texture_picker = 0;
			} else {
				texture_picker = 1;
			}
			c->tiles[(i * (int)n) + j].offset_texture = range_textures[texture_picker];

			int colour_picker;
			if (index == open_index || index == close_index) {
				colour_picker = 1;
			} else {
				colour_picker = 3;
			}
			c->tiles[(i * (int)n) + j].colour = range_colours[colour_picker];

			c->n_tile++;
			index++;
		}
	}

	if (c->debug.enabled != 1) {
		return;
	}

	float scale_debug = .5f;
	vec2 texture_debug = range_textures[0];
	vec4 magenta = {1.0f, 0.0f, 1.0f, 1.0f};

	c->debug.centre = centre;
	c->debug.open = c->tiles[open_index].offset;
	c->debug.close = c->tiles[close_index].offset;

	vec2 debug_points[3] = {c->debug.centre, c->debug.open, c->debug.close};

	for (int m = 0; m < 3; m++) {
		int ndb = c->n_tile;
		c->tiles[ndb].colour = magenta;
		c->tiles[ndb].dimension = (vec2){dim * scale_debug, dim * scale_debug};
		c->tiles[ndb].dimension_texture = (vec2){dim_texture};
		c->tiles[ndb].offset_texture = texture_debug;

		c->tiles[ndb].offset = debug_points[m];
		c->n_tile++;
	}

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
