#pragma once

#include "batch.h"
#include "construct.h"

typedef struct pipeline_t {
	unsigned int capacity_tile;
	unsigned int active_tile;
	float *positions;
	float *texcoords;
	float *colours;
} pipeline_t;

int new_pipeline(pipeline_t *p);

int send_pipeline(pipeline_t *p, construct_t *c);

int upload_pipeline(pipeline_t *p, batch_t *b);

int free_pipeline(pipeline_t *p);
