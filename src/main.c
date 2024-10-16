#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "batch.h"
#include "construct.h"
#include "shader.h"
#include "texture.h"

#define WIDTH 640
#define HEIGHT 640

int main() {

	int width = WIDTH;
	int height = HEIGHT;

	srand(time(NULL));

	if (glfwInit() == 0) {
		printf("error - glfw stage.\n");
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow *window = glfwCreateWindow(width, height, "glfw", NULL, NULL);
	if (window == NULL) {
		printf("error - window stage.\n");
		return -1;
	}

	glfwMakeContextCurrent(window);
	if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0) {
		glfwTerminate();
		printf("error - glad stage.\n");
		glfwDestroyWindow(window);
		return -1;
	}

	glfwSwapInterval(1);
	glViewport(0, 0, width, height);
	glClearColor(.1f, .1f, .1f, 1.0f);

	GLuint program = 0;
	if (new_program(&program, "./shaders/vert.glsl", "./shaders/frag.glsl") < 0) {
		printf("error - shader program stage.\n");
		glfwDestroyWindow(window);
		return -1;
	}

	texture_t texture1 = {"./textures/colour.bmp", 0, 0, 0, GL_REPEAT, GL_NEAREST, GL_LINEAR, GL_BGRA,
						  GL_UNSIGNED_BYTE};
	if (new_texture(&texture1) < 0) {
		printf("error - texture stage.");
		glDeleteProgram(program);
		glfwDestroyWindow(window);
		return -1;
	}
	glUniform1i(glGetUniformLocation(program, "tex_sampler"), texture1.handle);

	unsigned int n_batches = 0;
	batch_t *batches[4];

	construct_t c1;
	batch_t b1;
	new_construct(&c1);
	new_batch(&b1, &c1);
	batches[0] = &b1;
	n_batches++;
	free(c1.tiles);
	c1.tiles = NULL;

	construct_t c2;
	batch_t b2;
	new_construct(&c2);
	new_batch(&b2, &c2);
	batches[1] = &b2;
	n_batches++;
	free(c2.tiles);
	c2.tiles = NULL;

	construct_t c3;
	batch_t b3;
	new_construct(&c3);
	new_batch(&b3, &c3);
	batches[2] = &b3;
	n_batches++;
	free(c3.tiles);
	c3.tiles = NULL;

	construct_t c4;
	batch_t b4;
	new_construct(&c4);
	new_batch(&b4, &c4);
	batches[3] = &b4;
	n_batches++;
	free(c4.tiles);
	c4.tiles = NULL;

	glUseProgram(program);
	glBindTexture(GL_TEXTURE_2D, texture1.handle);

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		for (int i = 0; i < n_batches; i++) {
			glBindVertexArray(batches[i]->handles.vao);
			glDrawArrays(GL_TRIANGLES, 0, batches[i]->n * 6);
		}

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glDeleteTextures(1, &texture1.handle);
	glDeleteProgram(program);

	for (int i = 0; i < n_batches; i++) {
		batch_t b = *batches[i];
		GLuint buffers[3] = {b.handles.pbo, b.handles.tbo, b.handles.cbo};
		glDeleteVertexArrays(1, &b.handles.vao);
		glDeleteBuffers(3, buffers);
		free(b.positions);
		b.positions = NULL;
		free(b.texcoords);
		b.texcoords = NULL;
		free(b.colours);
		b.colours = NULL;
	}

	glfwDestroyWindow(window);

	return 0;
}
