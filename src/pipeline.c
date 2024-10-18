#include <glad/glad.h>

#include <stdlib.h>

#include "pipeline.h"

#define CAPACITY_BUFFER 10;

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

int send_pipeline(pipeline_t *p, construct_t *c) {

	for (int i = 0; i < c->n_tile; i++) {

		tile_t t = c->tiles[i];
		float dimx = t.dimension.x1 * .5f;
		float dimy = t.dimension.x2 * .5f;
		float dimtx = t.dimension_texture.x1 * .5f;
		float dimty = t.dimension_texture.x2 * .5f;

		vec2 centre = t.offset;
		vec2 iv = {centre.x1 - dimx, centre.x2 + dimy};
		vec2 jv = {centre.x1 + dimx, centre.x2 + dimy};
		vec2 kv = {centre.x1 - dimx, centre.x2 - dimy};
		vec2 lv = {centre.x1 + dimx, centre.x2 - dimy};

		p->positions[(i * 12) + 0] = iv.x1;
		p->positions[(i * 12) + 1] = iv.x2;

		p->positions[(i * 12) + 2] = jv.x1;
		p->positions[(i * 12) + 3] = jv.x2;

		p->positions[(i * 12) + 4] = kv.x1;
		p->positions[(i * 12) + 5] = kv.x2;

		p->positions[(i * 12) + 6] = kv.x1;
		p->positions[(i * 12) + 7] = kv.x2;

		p->positions[(i * 12) + 8] = lv.x1;
		p->positions[(i * 12) + 9] = lv.x2;

		p->positions[(i * 12) + 10] = jv.x1;
		p->positions[(i * 12) + 11] = jv.x2;

		vec2 centret = t.offset_texture;
		vec2 itv = {centret.x1 - dimtx, centret.x2 + dimty};
		vec2 jtv = {centret.x1 + dimtx, centret.x2 + dimty};
		vec2 ktv = {centret.x1 - dimtx, centret.x2 - dimty};
		vec2 ltv = {centret.x1 + dimtx, centret.x2 - dimty};

		p->texcoords[(i * 12) + 0] = itv.x1;
		p->texcoords[(i * 12) + 1] = itv.x2;

		p->texcoords[(i * 12) + 2] = jtv.x1;
		p->texcoords[(i * 12) + 3] = jtv.x2;

		p->texcoords[(i * 12) + 4] = ktv.x1;
		p->texcoords[(i * 12) + 5] = ktv.x2;

		p->texcoords[(i * 12) + 6] = ktv.x1;
		p->texcoords[(i * 12) + 7] = ktv.x2;

		p->texcoords[(i * 12) + 8] = ltv.x1;
		p->texcoords[(i * 12) + 9] = ltv.x2;

		p->texcoords[(i * 12) + 10] = jtv.x1;
		p->texcoords[(i * 12) + 11] = jtv.x2;

		p->colours[(i * 24) + 0] = t.colour.x1;
		p->colours[(i * 24) + 1] = t.colour.x2;
		p->colours[(i * 24) + 2] = t.colour.x3;
		p->colours[(i * 24) + 3] = t.colour.x4;

		p->colours[(i * 24) + 4] = t.colour.x1;
		p->colours[(i * 24) + 5] = t.colour.x2;
		p->colours[(i * 24) + 6] = t.colour.x3;
		p->colours[(i * 24) + 7] = t.colour.x4;

		p->colours[(i * 24) + 8] = t.colour.x1;
		p->colours[(i * 24) + 9] = t.colour.x2;
		p->colours[(i * 24) + 10] = t.colour.x3;
		p->colours[(i * 24) + 11] = t.colour.x4;

		p->colours[(i * 24) + 12] = t.colour.x1;
		p->colours[(i * 24) + 13] = t.colour.x2;
		p->colours[(i * 24) + 14] = t.colour.x3;
		p->colours[(i * 24) + 15] = t.colour.x4;

		p->colours[(i * 24) + 16] = t.colour.x1;
		p->colours[(i * 24) + 17] = t.colour.x2;
		p->colours[(i * 24) + 18] = t.colour.x3;
		p->colours[(i * 24) + 19] = t.colour.x4;

		p->colours[(i * 24) + 20] = t.colour.x1;
		p->colours[(i * 24) + 21] = t.colour.x2;
		p->colours[(i * 24) + 22] = t.colour.x3;
		p->colours[(i * 24) + 23] = t.colour.x4;
	}

	p->active_tile = c->n_tile;
	c->n_tile = 0;
	return 0;
}

int upload_pipeline(pipeline_t *p, batch_t *b) {

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

	return 0;
}

int free_pipeline(pipeline_t *p) {

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

	return 0;
}
