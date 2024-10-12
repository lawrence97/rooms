#include <glad/glad.h>

#include <stdio.h>
#include <stdlib.h>

#include "shader.h"

int new_shader(unsigned int *shader, const char *path, int type) {

	FILE *f = NULL;
	size_t capacity = 0;

	f = fopen(path, "rb");
	if (f == NULL) {
		return -1;
	}

	fseek(f, 0l, SEEK_END);
	capacity = ftell(f);
	fseek(f, 0l, SEEK_SET);

	char *buffer = (char *)calloc(sizeof(char), capacity);

	unsigned long long bytes_read = 0;
	bytes_read = fread(buffer, sizeof(char), capacity, f);

	*shader = glCreateShader(type);

	const GLchar *buffer_ptr = buffer;
	const GLint *buffer_length = (GLint *)&bytes_read;

	glShaderSource(*shader, 1, &buffer_ptr, buffer_length);
	glCompileShader(*shader);

	free(buffer);
	buffer = NULL;

	return 0;
}

int new_program(GLuint *program, const char *vertex_path, const char *fragment_path) {

	GLuint vertex_shader = 0;
	new_shader(&vertex_shader, vertex_path, GL_VERTEX_SHADER);

	GLuint fragment_shader = 0;
	new_shader(&fragment_shader, fragment_path, GL_FRAGMENT_SHADER);

	*program = glCreateProgram();

	glAttachShader(*program, vertex_shader);
	glAttachShader(*program, fragment_shader);

	glLinkProgram(*program);

	glDetachShader(*program, vertex_shader);
	glDetachShader(*program, fragment_shader);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	return 0;
}
