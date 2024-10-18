#pragma once

#include "batch.h"
#include "pipeline.h"

typedef struct scene_t {
	unsigned int capacity_batches;
	unsigned int n_batches;
	pipeline_t *pipeline;
	construct_t *construct;
	batch_t *batches;
} scene_t;
