#pragma once

typedef struct handles_t {
	unsigned int vao;
	unsigned int pbo;
	unsigned int tbo;
	unsigned int cbo;
} handles_t;

typedef struct batch_t {
	handles_t handles;
	unsigned int n_tiles;
} batch_t;
