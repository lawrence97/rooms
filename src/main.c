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

	vec3 origin1 = {.0f, .0f, .0f};
	vec2 dim1 = {16.0f, 16.0f};
	vec2 texorigin1 = {.75f, .25f};
	vec2 texdim1 = {.5f, .5f};
	vec4 colour1 = {1.0f, 1.0f, .0f, 1.0f};
	vec4 colour2 = {0.0f, 1.0f, 1.0f, 1.0f};

	vec3 origin2 = {100.0f, -100.0f, .0f};
	vec4 colour3 = {1.0f, 1.0f, 1.0f, 1.0f};

	tile_t t1 = {origin1, dim1, texorigin1, texdim1, {colour1, colour1, colour1, colour2}};
	tile_t t2 = {origin2, dim1, texorigin1, texdim1, {colour3, colour3, colour3, colour3}};

	tile_t tiles[2] = {t1, t2};
	batch_t batch1;
	new_batch(&batch1, tiles, 2);

	glUseProgram(program);
	glBindTexture(GL_TEXTURE_2D, texture1.handle);
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(batch1.handles.vao);
		glDrawArrays(GL_TRIANGLES, 0, batch1.n * 6);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &batch1.handles.vao);
	glDeleteBuffers(1, &batch1.handles.pbo);
	glDeleteBuffers(1, &batch1.handles.tbo);
	glDeleteBuffers(1, &batch1.handles.cbo);

	glDeleteTextures(1, &texture1.handle);
	glDeleteProgram(program);

	free(batch1.positions);
	free(batch1.texcoords);
	free(batch1.colours);

	glfwDestroyWindow(window);

	return 0;
}
