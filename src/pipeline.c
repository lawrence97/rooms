#include <glad/glad.h>

#include <stdlib.h>

#include "pipeline.h"

#define CAPACITY_BUFFER 100 + 3;

vec2 template_quad_multipliers[6] = {
	{-1.0f, 1.0f}, {1.0f, 1.0f}, {-1.0f, -1.0f}, {-1.0f, -1.0f}, {1.0f, -1.0f}, {1.0f, 1.0f},
};

int new_pipeline(pipeline_t *p) {

	unsigned int capacity_buffer = CAPACITY_BUFFER;
	p->capacity_tile = capacity_buffer;

	p->positions = (float *)calloc(capacity_buffer, sizeof(float) * 6 * 2);
	p->texcoords = (float *)calloc(capacity_buffer, sizeof(float) * 6 * 2);
	p->colours = (float *)calloc(capacity_buffer, sizeof(float) * 6 * 4);

	if (p->positions == NULL || p->texcoords == NULL || p->colours == NULL) {
		return -1;
	}

	return 0;
}

void send_pipeline(pipeline_t *p, construct_t *c) {

	for (int i = 0; i < c->n_tile; i++) {

		tile_t t = c->tiles[i];
		float dimx = t.dimension.x1 * .5f;
		float dimy = t.dimension.x2 * .5f;
		float dimtx = t.dimension_texture.x1 * .5f;
		float dimty = t.dimension_texture.x2 * .5f;

		vec2 centre = t.offset;
		for (int m = 0; m < 6; m++) {
			p->positions[(i * 12) + (m * 2) + 0] = centre.x1 + (dimx * template_quad_multipliers[m].x1);
			p->positions[(i * 12) + (m * 2) + 1] = centre.x2 + (dimy * template_quad_multipliers[m].x2);
		}

		vec2 centret = t.offset_texture;
		for (int m = 0; m < 6; m++) {
			p->texcoords[(i * 12) + (m * 2) + 0] = centret.x1 + (dimtx * template_quad_multipliers[m].x1);
			p->texcoords[(i * 12) + (m * 2) + 1] = centret.x2 + (dimty * template_quad_multipliers[m].x2);
		}

		for (int m = 0; m < 6; m++) {
			p->colours[(i * 24) + (m * 4) + 0] = t.colour.x1;
			p->colours[(i * 24) + (m * 4) + 1] = t.colour.x2;
			p->colours[(i * 24) + (m * 4) + 2] = t.colour.x3;
			p->colours[(i * 24) + (m * 4) + 3] = t.colour.x4;
		}
	}

	p->active_tile = c->n_tile;
	c->n_tile = 0;
	return;
}

void upload_pipeline(pipeline_t *p, batch_t *b) {

	b->n_tiles = p->active_tile;
	glGenVertexArrays(1, &b->handles.vao);
	glGenBuffers(1, &b->handles.pbo);
	glGenBuffers(1, &b->handles.tbo);
	glGenBuffers(1, &b->handles.cbo);

	glBindVertexArray(b->handles.vao);

	glBindBuffer(GL_ARRAY_BUFFER, b->handles.pbo);
	glBufferData(GL_ARRAY_BUFFER, b->n_tiles * 6 * 2 * sizeof(float), p->positions, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

	glBindBuffer(GL_ARRAY_BUFFER, b->handles.tbo);
	glBufferData(GL_ARRAY_BUFFER, b->n_tiles * 6 * 2 * sizeof(float), p->texcoords, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

	glBindBuffer(GL_ARRAY_BUFFER, b->handles.cbo);
	glBufferData(GL_ARRAY_BUFFER, b->n_tiles * 6 * 4 * sizeof(float), p->colours, GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, (void *)0);

	return;
}

void free_pipeline(pipeline_t *p) {

	if (p->positions != NULL) {
		free(p->positions);
	}
	p->positions = NULL;

	if (p->texcoords != NULL) {
		free(p->texcoords);
	}
	p->texcoords = NULL;

	if (p->colours != NULL) {
		free(p->colours);
	}
	p->colours = NULL;

	p->active_tile = 0;
	p->capacity_tile = 0;

	return;
}
