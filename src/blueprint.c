#include <stdlib.h>

#include "blueprint.h"

int new_blueprint(blueprint_t *blueprint, unsigned int n) {

	blueprint->tiles = (tile_t *)malloc(n * sizeof(tile_t));
	if (blueprint->tiles == NULL) {
		return -1;
	}
	blueprint->n = n;

	// some defaults for now.
	vec2 texture_centre = {.75f, .25f};
	vec3 centre = {.0f, .0f, 1.0f};
	vec4 white = {1.0f, 1.0f, 1.0f, 1.0f};
	vec4 red = {1.0f, .0f, .0f, 1.0f};
	vec4 green = {.0f, 1.0f, .0f, 1.0f};
	vec4 blue = {.0f, .0f, 1.0f, 1.0f};
	vec4 colour_range[4] = {white, red, green, blue};

	tile_t *ts = blueprint->tiles;

	for (int i = 0; i < n; i++) {
		vec4 sel_col = colour_range[rand_int(0, 3)];
		ts[i].colours[0] = sel_col;
		ts[i].colours[1] = sel_col;
		ts[i].colours[2] = sel_col;
		ts[i].colours[3] = sel_col;

		ts[i].dim_pos.x1 = 16.0f;
		ts[i].dim_pos.x2 = 16.0f;

		ts[i].dim_tex.x1 = .5f;
		ts[i].dim_tex.x2 = .5f;

		ts[i].origin_pos = centre;
		ts[i].origin_pos.x1 = rand_int(-i, i) * 16.0f;
		ts[i].origin_pos.x2 = rand_int(-i, i) * 16.0f;

		ts[i].origin_tex = texture_centre;
	}

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
