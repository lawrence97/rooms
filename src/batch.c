#include <glad/glad.h>

#include <stdlib.h>

#include "batch.h"
#include "construct.h"

vec3 templates[6] = {
	{-1.0f, 1.0f, 0.0f},  {1.0f, 1.0f, 0.0f},  {-1.0f, -1.0f, 0.0f},
	{-1.0f, -1.0f, 0.0f}, {1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 0.0f},
};

int new_batch(batch_t *batch, construct_t *construct) {

	unsigned int num = construct->n;
	tile_t *tiles = construct->tiles;

	batch->n = construct->n;
	unsigned int component_multiplier = num * 6 * sizeof(float);
	unsigned int size_p = component_multiplier * 3;
	unsigned int size_t = component_multiplier * 2;
	unsigned int size_c = component_multiplier * 4;

	float *p_buffer = (float *)malloc(size_p);
	if (p_buffer == NULL) {
		return -1;
	}
	float *t_buffer = (float *)malloc(size_t);
	if (t_buffer == NULL) {
		free(p_buffer);
		return -1;
	}
	float *c_buffer = (float *)malloc(size_c);
	if (c_buffer == NULL) {
		free(p_buffer);
		free(t_buffer);
		return -1;
	}

	tile_t t = {0};
	vec3 template = {0};
	vec3 nth = {0};
	vec2 ntht = {0};

	vec3 o = {0};
	float dx, dy, dz = 0;
	vec2 ot = {0};
	float dxt, dyt = 0;
	vec4 colours[6] = {0};

	int p_offset = 0;
	int t_offset = 0;
	int c_offset = 0;

	for (int n = 0; n < num; n++) {
		t = tiles[n];

		o = t.coord;
		dx = t.dimension.x1;
		dy = t.dimension.x2;

		ot = t.coord_tex;
		dxt = t.dimension_tex.x1;
		dyt = t.dimension_tex.x2;

		p_offset = n * 6 * 3;
		t_offset = n * 6 * 2;
		c_offset = n * 6 * 4;

		colours[0] = t.colours[0];
		colours[1] = t.colours[1];
		colours[2] = t.colours[2];
		colours[3] = t.colours[2];
		colours[4] = t.colours[3];
		colours[5] = t.colours[1];

		for (int m = 0; m < 6; m++) {
			template = templates[m];

			nth = o;
			nth.x1 += .5f * dx * template.x1;
			nth.x2 += .5f * dy * template.x2;
			nth.x3 += .5f * dz * template.x3;

			p_buffer[p_offset + (m * 3) + 0] = nth.x1;
			p_buffer[p_offset + (m * 3) + 1] = nth.x2;
			p_buffer[p_offset + (m * 3) + 2] = nth.x3;

			ntht = ot;
			ntht.x1 += .5f * dxt * template.x1;
			ntht.x2 += .5f * dyt * template.x2;

			t_buffer[t_offset + (m * 2) + 0] = ntht.x1;
			t_buffer[t_offset + (m * 2) + 1] = ntht.x2;

			c_buffer[c_offset + (m * 4) + 0] = colours[m].x1;
			c_buffer[c_offset + (m * 4) + 1] = colours[m].x2;
			c_buffer[c_offset + (m * 4) + 2] = colours[m].x3;
			c_buffer[c_offset + (m * 4) + 3] = colours[m].x4;
		}
	}

	batch->positions = p_buffer;
	batch->texcoords = t_buffer;
	batch->colours = c_buffer;

	glGenVertexArrays(1, &batch->handles.vao);
	GLuint *array_handles[3] = {(GLuint *)&batch->handles.pbo, (GLuint *)&batch->handles.tbo,
								(GLuint *)&batch->handles.cbo};
	glGenBuffers(3, *array_handles);

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

	return 0;
}