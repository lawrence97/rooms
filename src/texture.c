#include <glad/glad.h>

#include <stdio.h>
#include <stdlib.h>

#include "texture.h"

// bitmap info header offsets
#define biOffBits 70
#define biSizeImage 34
#define biWidth 18
#define biHeight 22

int new_texture(texture_t *texture) {

	FILE *f = fopen(texture->path, "rb");
	if (f == NULL) {
		return -1;
	}

	unsigned int size = 0;
	fseek(f, (long)biSizeImage, SEEK_SET);
	fread(&size, sizeof(unsigned int), 1, f);

	unsigned int width = 0;
	fseek(f, (long)biWidth, SEEK_SET);
	fread(&width, sizeof(unsigned int), 1, f);
	texture->width = width;

	unsigned int height = 0;
	fseek(f, (long)biHeight, SEEK_SET);
	fread(&height, sizeof(unsigned int), 1, f);
	texture->height = height;

	if (height * width * sizeof(unsigned int) != size) {
		fclose(f);
		return -1;
	}

	int *buffer = (int *)calloc(size, sizeof(char));
	if (buffer == NULL) {
		fclose(f);
		return -1;
	}
	size_t bytes_read = 0;
	fseek(f, (long)biOffBits, SEEK_SET);
	bytes_read = fread(buffer, sizeof(unsigned int), width * height, f) * sizeof(unsigned int);
	if (bytes_read == 0) {
		free(buffer);
		buffer = NULL;
		fclose(f);
		return -1;
	}

	glGenTextures(1, &texture->handle);
	glBindTexture(GL_TEXTURE_2D, texture->handle);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture->wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture->wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture->min_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture->mag_filter);
	glTexImage2D(GL_TEXTURE_2D, 0, texture->format, width, height, 0, texture->format, texture->type, buffer);
	glActiveTexture(GL_TEXTURE0);

	free(buffer);
	buffer = NULL;
	fclose(f);

	return 0;
}
