#include <glad/glad.h>

#include <stdlib.h>

#include "render.h"

vec3 blueprint_pos[6] = {
	{-1.0f, 1.0f, 0.0f},  {1.0f, 1.0f, 0.0f},  {-1.0f, -1.0f, 0.0f},
	{-1.0f, -1.0f, 0.0f}, {1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 0.0f},
};

vec2 blueprint_tex[6] = {
	{-1.0f, 1.0f}, {1.0f, 1.0f}, {-1.0f, -1.0}, {-1.0f, -1.0f}, {1.0f, -1.0f}, {1.0f, 1.0f},
};

void new_batch(batch_t *batch, tile_t *tiles, unsigned int ntiles) {
	batch->n = ntiles;
	unsigned int size_p = ntiles * 6 * 3 * sizeof(float);
	unsigned int size_t = ntiles * 6 * 2 * sizeof(float);
	unsigned int size_c = ntiles * 6 * 4 * sizeof(float);

	float *p_buffer = (float *)malloc(size_p);
	float *t_buffer = (float *)malloc(size_t);
	float *c_buffer = (float *)malloc(size_c);

	tile_t t = tiles[0];

	// positions
	vec3 o = t.origin_pos;
	float dx = t.dim_pos.x1;
	float dy = t.dim_pos.x2;
	float dz = 0;
	int p_offset = 0;
	int t_offset = 0;
	int c_offset = 0;

	for (int m = 0; m < 6; m++) {
		vec3 b = blueprint_pos[m];
		vec3 nth = o;
		nth.x1 += .5f * dx * b.x1;
		nth.x2 += .5f * dy * b.x2;
		nth.x3 += .5f * dz * b.x3;
		p_buffer[p_offset + (m * 3) + 0] = nth.x1;
		p_buffer[p_offset + (m * 3) + 1] = nth.x2;
		p_buffer[p_offset + (m * 3) + 2] = nth.x3;
	}

	// texcoords
	vec2 ot = t.origin_tex;
	float dxt = t.dim_tex.x1;
	float dyt = t.dim_tex.x2;
	for (int m = 0; m < 6; m++) {
		vec2 b = blueprint_tex[m];
		vec2 nth = ot;
		nth.x1 += .5f * dxt * b.x1;
		nth.x2 += .5f * dyt * b.x2;
		t_buffer[t_offset + (m * 2) + 0] = nth.x1;
		t_buffer[t_offset + (m * 2) + 1] = nth.x2;
	}

	// colours
	vec4 ic = t.colours[0];
	vec4 jc = t.colours[1];
	vec4 kc = t.colours[2];
	vec4 lc = t.colours[3];
	vec4 cols[6] = {ic, jc, kc, kc, lc, jc};

	for (int m = 0; m < 6; m++) {
		c_buffer[c_offset + (m * 4) + 0] = cols[m].x1;
		c_buffer[c_offset + (m * 4) + 1] = cols[m].x2;
		c_buffer[c_offset + (m * 4) + 2] = cols[m].x3;
		c_buffer[c_offset + (m * 4) + 3] = cols[m].x4;
	}

	// now 2nd tile

	t = tiles[1];
	p_offset += 6 * 3;
	t_offset += 6 * 2;
	c_offset += 6 * 4;

	o = t.origin_pos;
	dx = t.dim_pos.x1;
	dy = t.dim_pos.x2;

	for (int m = 0; m < 6; m++) {
		vec3 b = blueprint_pos[m];
		vec3 nth = o;
		nth.x1 += .5f * dx * b.x1;
		nth.x2 += .5f * dy * b.x2;
		nth.x3 += .5f * dz * b.x3;
		p_buffer[p_offset + (m * 3) + 0] = nth.x1;
		p_buffer[p_offset + (m * 3) + 1] = nth.x2;
		p_buffer[p_offset + (m * 3) + 2] = nth.x3;
	}

	ot = t.origin_tex;
	dxt = t.dim_tex.x1;
	dyt = t.dim_tex.x2;

	for (int m = 0; m < 6; m++) {
		vec2 b = blueprint_tex[m];
		vec2 nth = ot;
		nth.x1 += .5f * dxt * b.x1;
		nth.x2 += .5f * dyt * b.x2;
		t_buffer[t_offset + (m * 2) + 0] = nth.x1;
		t_buffer[t_offset + (m * 2) + 1] = nth.x2;
	}

	ic = t.colours[0];
	jc = t.colours[1];
	kc = t.colours[2];
	lc = t.colours[3];
	cols[0] = ic;
	cols[1] = jc;
	cols[2] = kc;
	cols[3] = kc;
	cols[4] = lc;
	cols[5] = jc;

	for (int m = 0; m < 6; m++) {
		c_buffer[c_offset + (m * 4) + 0] = cols[m].x1;
		c_buffer[c_offset + (m * 4) + 1] = cols[m].x2;
		c_buffer[c_offset + (m * 4) + 2] = cols[m].x3;
		c_buffer[c_offset + (m * 4) + 3] = cols[m].x4;
	}

	batch->positions = p_buffer;
	batch->texcoords = t_buffer;
	batch->colours = c_buffer;

	glGenVertexArrays(1, &batch->handles.vao);
	glGenBuffers(1, &batch->handles.pbo);
	glGenBuffers(1, &batch->handles.tbo);
	glGenBuffers(1, &batch->handles.cbo);

	glBindVertexArray(batch->handles.vao);

	glBindBuffer(GL_ARRAY_BUFFER, batch->handles.pbo);
	glBufferData(GL_ARRAY_BUFFER, size_p, batch->positions, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

	glBindBuffer(GL_ARRAY_BUFFER, batch->handles.tbo);
	glBufferData(GL_ARRAY_BUFFER, size_t, batch->texcoords, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

	glBindBuffer(GL_ARRAY_BUFFER, batch->handles.cbo);
	glBufferData(GL_ARRAY_BUFFER, size_c, batch->colours, GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, (void *)0);
}
