#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <stdio.h>

#include "maths.h"
#include "primitives.h"
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

	vec3 o1 = {.0f, .0f, .0f};
	vec2 d1 = {16.0f, 16.0f};
	vec2 o2 = {.75f, .25f};
	vec2 d2 = {.5f, .5f};
	vec4 c1 = {1.0f, 1.0f, .0f, 1.0f};
	vec4 c2 = {0.0f, 1.0f, 1.0f, 1.0f};

	tile_t t1 = {o1, d1, o2, d2, {c1, c1, c1, c2}};
	vertices_t v1 = {0};
	new_vertices(&v1, &t1);
	handles_t h1 = {0};
	new_buffers(&h1, &v1);

	glUseProgram(program);
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(program);
		glBindTexture(GL_TEXTURE_2D, texture1.handle);
		glBindVertexArray(h1.vao);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &h1.vao);
	glDeleteBuffers(1, &h1.pbo);
	glDeleteBuffers(1, &h1.tbo);
	glDeleteBuffers(1, &h1.cbo);

	glDeleteTextures(1, &texture1.handle);
	glDeleteProgram(program);

	glfwDestroyWindow(window);

	return 0;
}
