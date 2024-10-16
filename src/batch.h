#pragma once

#include "blueprint.h"

typedef struct handles_t {
	unsigned int vao;
	unsigned int pbo;
	unsigned int tbo;
	unsigned int cbo;
} handles_t;

typedef struct batch_t {
	handles_t handles;
	unsigned int n;
	float *positions;
	float *texcoords;
	float *colours;
} batch_t;

int new_batch(batch_t *batch, blueprint_t *blueprint);
