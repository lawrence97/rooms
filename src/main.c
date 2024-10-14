#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <stdio.h>
#include <stdlib.h>

#include "maths.h"
#include "render.h"
#include "shader.h"
#include "texture.h"

#define WIDTH 640
#define HEIGHT 640

int main() {

	int width = WIDTH;
	int height = HEIGHT;

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

	vec3 centre = {.0f, .0f, .0f};
	vec3 lower_right = {100.0f, -100.0f, .0f};
	vec3 lower_left = {-100.0f, -100.0f, .0f};
	vec2 tile1_index = {.75f, .25f};
	vec2 tile2_index = {.25f, .25f};
	vec2 tile_dim = {16.0f, 16.0f};
	vec2 tile_larger_dim = {32.0f, 32.0f};
	vec2 tex_dim = {.5f, .5f};
	vec4 white_c = {1.0f, 1.0f, 1.0f, 1.0f};
	vec4 yellow_c = {1.0f, 1.0f, 0.0f, 1.0f};
	vec4 green_c = {0.0f, 1.0f, 1.0f, 1.0f};
	vec4 blue_c = {0.0f, 0.0f, 1.0f, 1.0f};

	tile_t t1 = {centre, tile_dim, tile1_index, tex_dim, {yellow_c, yellow_c, yellow_c, green_c}};
	tile_t t2 = {lower_right, tile_dim, tile1_index, tex_dim, {white_c, white_c, white_c, white_c}};
	tile_t t3 = {lower_left, tile_larger_dim, tile2_index, tex_dim, {green_c, blue_c, green_c, blue_c}};

	tile_t tiles[3] = {t1, t2, t3};
	batch_t batch1;
	if (new_batch(&batch1, tiles, 3) < 0) {
		printf("error - batch stage.\n");
		glDeleteProgram(program);
		glDeleteTextures(1, &texture1.handle);
		glfwDestroyWindow(window);
		return -1;
	}

	unsigned int n_batches = 0;
	batch_t *batches[1];
	batches[0] = &batch1;
	n_batches++;

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
