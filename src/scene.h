#pragma once

#include "batch.h"
#include "pipeline.h"

typedef struct scene_t {
	unsigned int capacity_batches;
	unsigned int n_batches;
	pipeline_t *pipeline;
	construct_t *construct;
	batch_t *batches;
	construct_opts *prev;
	construct_opts *next;
} scene_t;

void new_batch(scene_t *scene, int *batch_index_ptr);
