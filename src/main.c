#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "batch.h"
#include "construct.h"
#include "debug.h"
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

	GLuint program_debug = 0;
	if (new_program(&program_debug, "./shaders/vert_debug.glsl", "./shaders/frag_debug.glsl") < 0) {
		printf("error - shader program stage.\n");
		glfwDestroyWindow(window);
		return -1;
	}
	GLint uni_col_location = glGetUniformLocation(program_debug, "uni_col");
	float colour_debug_default[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	glUniform4fv(uni_col_location, 1, colour_debug_default);

	GLuint program = 0;
	if (new_program(&program, "./shaders/vert.glsl", "./shaders/frag.glsl") < 0) {
		printf("error - shader program stage.\n");
		glDeleteProgram(program_debug);
		glfwDestroyWindow(window);
		return -1;
	}

	texture_t texture1 = {"./textures/colour.bmp", 0, 0, 0, GL_REPEAT, GL_NEAREST, GL_LINEAR, GL_BGRA,
						  GL_UNSIGNED_BYTE};
	if (new_texture(&texture1) < 0) {
		printf("error - texture stage.");
		glDeleteProgram(program);
		glDeleteProgram(program_debug);
		glfwDestroyWindow(window);
		return -1;
	}
	glUniform1i(glGetUniformLocation(program, "tex_sampler"), texture1.handle);

	unsigned int n_batches = 0;
	unsigned int n_constructs = 0;

	construct_t c1;
	batch_t b1;
	construct_t c2;
	batch_t b2;
	construct_t c3;
	batch_t b3;

	construct_t *constructs[3] = {&c1, &c2, &c3};
	batch_t *batches[3] = {&b1, &b2, &b3};

	if (new_construct(&c1) < 0 || new_batch(&b1, &c1) < 0) {
		printf("error - construct or batch stage.");
		return -1;
	}
	n_constructs++;
	n_batches++;

	if (new_construct(&c2) < 0 || new_batch(&b2, &c2) < 0) {
		printf("error - construct or batch stage.");
		return -1;
	}
	n_constructs++;
	n_batches++;

	if (new_construct(&c3) < 0 || new_batch(&b3, &c3) < 0) {
		printf("error - construct or batch stage.");
		return -1;
	}
	n_constructs++;
	n_batches++;

	debug_t db1;
	db1.colour[0] = .2f;
	db1.colour[1] = 1.0f;
	db1.colour[2] = .5f;
	db1.colour[3] = 1.0f;
	if (new_debug(&db1) < 0) {
		printf("error - new debug stage.");
		glDeleteProgram(program);
		glDeleteProgram(program_debug);
		glfwDestroyWindow(window);
		return -1;
	}
	if (append_debug(&db1, (vec2){constructs[0]->centre.x1, constructs[0]->centre.x2}) < 0 ||
		append_debug(&db1, (vec2){constructs[1]->centre.x1, constructs[1]->centre.x2}) < 0 ||
		append_debug(&db1, (vec2){constructs[2]->centre.x1, constructs[2]->centre.x2}) < 0) {
		printf("error - append debug stage.");
		glDeleteProgram(program);
		glDeleteProgram(program_debug);
		glfwDestroyWindow(window);
		return -1;
	}

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(program);
		glPointSize(1);
		glBindTexture(GL_TEXTURE_2D, texture1.handle);
		for (int i = 0; i < n_batches; i++) {
			glBindVertexArray(batches[i]->handles.vao);
			glDrawArrays(GL_TRIANGLES, 0, batches[i]->n * 6);
		}

		glPointSize(7);
		glUseProgram(program_debug);
		glUniform4fv(uni_col_location, 1, db1.colour);
		glBindVertexArray(db1.vao);
		glDrawArrays(GL_POINTS, 0, db1.n);

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

	for (int i = 0; i < n_constructs; i++) {
		construct_t c = *constructs[i];
		free(c.tiles);
		c.tiles = NULL;
	}

	glDeleteVertexArrays(1, &db1.vao);
	glDeleteBuffers(1, &db1.vbo);
	free(db1.points);
	db1.points = NULL;

	glfwDestroyWindow(window);

	return 0;
}
