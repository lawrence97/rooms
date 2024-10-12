#include "primitives.h"

static vec3 blueprint_pos[6] = {
	{-1.0f, 1.0f, 0.0f},  {1.0f, 1.0f, 0.0f},  {-1.0f, -1.0f, 0.0f},
	{-1.0f, -1.0f, 0.0f}, {1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 0.0f},
};

static vec2 blueprint_tex[6] = {
	{-1.0f, 1.0f}, {1.0f, 1.0f}, {-1.0f, -1.0}, {-1.0f, -1.0f}, {1.0f, -1.0f}, {1.0f, 1.0f},
};

void new_vertices(vertices_t *verts, tile_t *tile) {

	vec3 o = tile->origin_pos;	 // tile centre
	float dx = tile->dim_pos.x1; // width
	float dy = tile->dim_pos.x2; // height
	float dz = 0;				 // depth (unused)
	for (int m = 0; m < 6; m++) {
		vec3 b = blueprint_pos[m];
		vec3 nth = o; // nth vertex this quad
		nth.x1 += .5f * dx * b.x1;
		nth.x2 += .5f * dy * b.x2;
		nth.x3 += .5f * dz * b.x3;
		verts->positions[m * 3] = nth.x1;
		verts->positions[(m * 3) + 1] = nth.x2;
		verts->positions[(m * 3) + 2] = nth.x3;
	}

	vec2 ot = tile->origin_tex;
	float dxt = tile->dim_tex.x1;
	float dyt = tile->dim_tex.x2;
	for (int m = 0; m < 6; m++) {
		vec2 b = blueprint_tex[m];
		vec2 nth = ot;
		nth.x1 += .5f * dxt * b.x1;
		nth.x2 += .5f * dyt * b.x2;
		verts->texcoords[m * 2] = nth.x1;
		verts->texcoords[(m * 2) + 1] = nth.x2;
	}

	vec4 ic = tile->colours[0];
	vec4 jc = tile->colours[1];
	vec4 kc = tile->colours[2];
	vec4 lc = tile->colours[3];
	vec4 cols[6] = {ic, jc, kc, kc, lc, jc};

	for (int m = 0; m < 6; m++) {
		verts->colours[m * 4] = cols[m].x1;
		verts->colours[(m * 4) + 1] = cols[m].x2;
		verts->colours[(m * 4) + 2] = cols[m].x3;
		verts->colours[(m * 4) + 3] = cols[m].x4;
	}
}
