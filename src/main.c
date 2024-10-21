#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "construct.h"
#include "pipeline.h"
#include "scene.h"
#include "shader.h"
#include "texture.h"

#define WIDTH 640
#define HEIGHT 640
#define SCENE_SIZE 6

int main() {

	int width = WIDTH;
	int height = HEIGHT;
	srand(time(NULL));

	if (glfwInit() == 0) {
		printf("error - glfw init.\n");
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow *window = glfwCreateWindow(width, height, "glfw", NULL, NULL);
	if (window == NULL) {
		printf("error - glfw window.\n");
		return -1;
	}

	glfwMakeContextCurrent(window);
	if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0) {
		printf("error - glad init.\n");
		return -1;
	}

	glfwSwapInterval(1);
	glViewport(0, 0, width, height);
	glClearColor(.1f, .1f, .1f, 1.0f);

	GLuint program = 0;
	if (new_program(&program, "./shaders/vert.glsl", "./shaders/frag.glsl") < 0) {
		printf("error - shader program.\n");
		return -1;
	}

	texture_t texture1 = {"./textures/colour.bmp", 0, 0, 0, GL_REPEAT, GL_NEAREST, GL_LINEAR, GL_BGRA,
						  GL_UNSIGNED_BYTE};
	if (new_texture(&texture1) < 0) {
		printf("error - texture load.\n");
		return -1;
	}

	pipeline_t pipeline;
	construct_t construct;
	scene_t scene;
	scene.capacity_batches = SCENE_SIZE;
	scene.n_batches = 0;
	scene.batches = (batch_t *)calloc(scene.capacity_batches, sizeof(batch_t));
	if (scene.batches == NULL) {
		printf("error - scene init.\n");
		return -1;
	}

	if (new_pipeline(&pipeline) < 0) {
		printf("error - new pipeline.\n");
		return -1;
	};
	if (new_construct(&construct) < 0) {
		printf("error - new construct.\n");
		return -1;
	};

	scene.pipeline = &pipeline;
	scene.construct = &construct;

	construct_opts construct_options;
	construct_options.prev_centre = (vec2){.0f, .0f};
	construct_options.prev_entry = (vec2){.0f, .0f};
	construct_options.prev_exit = (vec2){.0f, .0f};
	construct_options.prev_dim = (vec2){16.0f, 16.0f};
	construct_options.prev_tile_count = 0;
	construct_options.prev_theta = 0.0;

	unsigned int order = 0;

	for (int i = 0; i < scene.capacity_batches; i++) {
		batch_t b;
		update_construct(&construct, &construct_options, &order);
		send_pipeline(&pipeline, &construct);
		upload_pipeline(&pipeline, &b);
		scene.batches[i] = b;
		scene.n_batches++;
	}

	glBindTexture(GL_TEXTURE0, texture1.handle);
	glUniform1i(glGetUniformLocation(program, "tex_sampler"), texture1.handle);
	glUseProgram(program);
	glUniform2f(glGetUniformLocation(program, "window_scale"), width * .5f, height * .5f);

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		for (int i = 0; i < scene.n_batches; i++) {
			glBindVertexArray(scene.batches[i].handles.vao);
			glDrawArrays(GL_TRIANGLES, 0, scene.batches[i].n_tiles * 6);
		}

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glDeleteTextures(1, &texture1.handle);
	glDeleteProgram(program);

	free_construct(&construct);
	free_pipeline(&pipeline);

	for (int i = 0; i < scene.n_batches; i++) {
		handles_t handles = scene.batches[i].handles;
		GLuint buffers[3] = {handles.pbo, handles.tbo, handles.cbo};
		glDeleteVertexArrays(1, &handles.vao);
		glDeleteBuffers(3, buffers);
	}
	free(scene.batches);
	scene.batches = NULL;

	return 0;
}
