#include <stdlib.h>

#include "construct.h"

int new_construct(construct_t *c) {
	c->n_tile = 0;
	c->capacity_tile = 10;

	c->tiles = (tile_t *)calloc(c->capacity_tile, sizeof(tile_t));
	if (c->tiles == NULL) {
		c->capacity_tile = 0;
		return -1;
	}
	return 0;
}

int update_construct(construct_t *c) {

	c->n_tile = 0;

	unsigned int n = rand_int(6, 10);
	float dim = 16.0f;
	float dim_texture = .5f;

	vec4 red = {1.0f, 0.0f, 0.0f, 1.0f};
	vec4 green = {0.0f, 1.0f, 0.0f, 1.0f};
	vec4 blue = {0.0f, 0.0f, 1.0f, 1.0f};
	vec4 range[3] = {red, green, blue};

	float x = (float)rand_int(-280, 280);
	float y = (float)rand_int(-280, 280);

	vec2 centre = {x, y};

	for (int i = 0; i < n; i++) {

		float ox = (float)rand_int(-1, 1) * 16.0f;
		float oy = (float)rand_int(-1, 1) * 16.0f;

		vec2 offset = {ox, oy};
		vec2 centre_texture = {.75f, .25f};

		int picker = rand_int(0, 2);
		c->tiles[i].colour = range[picker];

		c->tiles[i].dimension.x1 = dim;
		c->tiles[i].dimension.x2 = dim;

		c->tiles[i].dimension_texture.x1 = dim_texture;
		c->tiles[i].dimension_texture.x2 = dim_texture;

		c->tiles[i].offset.x1 = centre.x1 + offset.x1;
		c->tiles[i].offset.x2 = centre.x2 + offset.x2;

		c->tiles[i].offset_texture.x1 = centre_texture.x1;
		c->tiles[i].offset_texture.x2 = centre_texture.x2;

		c->n_tile++;
	}

	return 0;
}

int free_construct(construct_t *c) {

	if (c->tiles != NULL) {
		free(c->tiles);
	}
	c->tiles = NULL;
	c->n_tile = 0;
	c->capacity_tile = 0;
	return 0;
}

int rand_int(int min, int max) {

	if (min == max) {
		return min;
	}

	int rn = rand();
	int rmod = min + (rn % (max + 1 - min));
	return rmod;
}
