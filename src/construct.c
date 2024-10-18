#include <stdlib.h>

#include "construct.h"

#define CONSTRUCT_CAPACITY 10
#define CONSTRUCT_MIN 4
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

	unsigned int n = rand_int(CONSTRUCT_MIN, CONSTRUCT_CAPACITY);

	float dim = 16.0f;
	float dim_texture = .5f;

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

	float x = (float)rand_int(-280, 280);
	float y = (float)rand_int(-280, 280);

	vec2 centre = {x, y};

	for (int i = 0; i < n; i++) {

		float ox = (float)rand_int(-1, 1) * 16.0f;
		float oy = (float)rand_int(-1, 1) * 16.0f;
		vec2 offset = {ox, oy};

		c->tiles[i].dimension = (vec2){dim, dim};

		c->tiles[i].dimension_texture = (vec2){dim_texture, dim_texture};

		c->tiles[i].offset = (vec2){centre.x1 + offset.x1, centre.x2 + offset.x2};

		int texture_picker = rand_int(0, 3);
		c->tiles[i].offset_texture = range_textures[texture_picker];

		int colour_picker = rand_int(0, 3);
		c->tiles[i].colour = range_colours[colour_picker];

		c->n_tile++;
	}

	if (c->debug.enabled != 1) {
		return;
	}

	float scale_debug = .5f;
	vec2 texture_debug = range_textures[1];
	vec4 yellow = {1.0f, 1.0f, 0.0f, 1.0f};

	c->debug.centre = centre;
	c->debug.open = c->tiles[0].offset;
	c->debug.close = c->tiles[c->n_tile - 1].offset;

	vec2 debug_points[3] = {c->debug.centre, c->debug.open, c->debug.close};

	for (int m = 0; m < 3; m++) {
		int ndb = c->n_tile;
		c->tiles[ndb].colour = yellow;
		c->tiles[ndb].dimension.x1 = dim * scale_debug;
		c->tiles[ndb].dimension.x2 = dim * scale_debug;
		c->tiles[ndb].dimension_texture.x1 = dim_texture;
		c->tiles[ndb].dimension_texture.x2 = dim_texture;
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
