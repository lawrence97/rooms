#include <stdlib.h>

#include "construct.h"

int new_construct(construct_t *construct) {

	unsigned int n = rand_int(5, 15);
	construct->n = n;
	construct->tiles = (tile_t *)malloc(n * sizeof(tile_t));
	if (construct->tiles == NULL) {
		return -1;
	}

	int rn_x = rand_int(-200, 200);
	int rn_y = rand_int(-200, 200);
	construct->centre = (vec3){rn_x, rn_y, 1.0f};

	float boundary_step = 16.0f;
	int rn_d = 0;
	float dist = 0;

	rn_d = rand_int(4, 12);
	dist = rn_d * boundary_step;
	construct->bounds[0].x1 = construct->centre.x1 - dist;
	construct->bounds[0].x2 = construct->centre.x2 + dist;

	rn_d = rand_int(4, 12);
	dist = rn_d * boundary_step;
	construct->bounds[1].x1 = construct->centre.x1 + dist;
	construct->bounds[1].x2 = construct->centre.x2 + dist;

	rn_d = rand_int(4, 12);
	dist = rn_d * boundary_step;
	construct->bounds[2].x1 = construct->centre.x1 - dist;
	construct->bounds[2].x2 = construct->centre.x2 - dist;

	rn_d = rand_int(4, 12);
	dist = rn_d * boundary_step;
	construct->bounds[3].x1 = construct->centre.x1 + dist;
	construct->bounds[3].x2 = construct->centre.x2 - dist;

	// temp textures centres
	vec2 tex_1 = {.25f, .25f};
	vec2 tex_2 = {.75f, .25f};
	vec2 tex_3 = {.25f, .75f};
	vec2 tex_4 = {.75f, .75f};

	// temp colours
	vec4 white = {1.0f, 1.0f, 1.0f, 1.0f};
	vec4 red = {1.0f, .0f, .0f, 1.0f};
	vec4 green = {.0f, 1.0f, .0f, 1.0f};
	vec4 blue = {.0f, .0f, 1.0f, 1.0f};

	vec4 colour_range[4] = {white, red, green, blue};
	vec2 texture_range[4] = {tex_1, tex_2, tex_3, tex_4};

	for (int i = 0; i < n; i++) {

		tile_t *tsel = (construct->tiles + i);
		vec4 sel_col = colour_range[rand_int(0, 3)];
		vec2 sel_tex = texture_range[rand_int(0, 3)];

		tsel->colours[0] = sel_col;
		tsel->colours[1] = sel_col;
		tsel->colours[2] = sel_col;
		tsel->colours[3] = sel_col;

		tsel->dimension.x1 = 16.0f;
		tsel->dimension.x2 = 16.0f;

		tsel->dimension_tex.x1 = .5f;
		tsel->dimension_tex.x2 = .5f;

		tsel->coord = construct->centre;
		tsel->coord.x1 += rand_int(-2, 2) * 16.0f;
		tsel->coord.x2 += rand_int(-2, 2) * 16.0f;

		tsel->coord_tex = sel_tex;
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
