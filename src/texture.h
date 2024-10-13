#pragma once

typedef struct texture_t {
	char *path;
	unsigned int handle;
	unsigned int width;
	unsigned int height;
	unsigned int wrap;
	unsigned int min_filter;
	unsigned int mag_filter;
	unsigned int format;
	unsigned int type;
} texture_t;

int new_texture(texture_t *texture);
