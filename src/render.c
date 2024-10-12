#include <glad/glad.h>

#include "render.h"

void new_buffers(handles_t *handles, vertices_t *verts) {

	glGenVertexArrays(1, &handles->vao);
	glGenBuffers(1, &handles->pbo);
	glGenBuffers(1, &handles->tbo);
	glGenBuffers(1, &handles->cbo);

	glBindVertexArray(handles->vao);

	glBindBuffer(GL_ARRAY_BUFFER, handles->pbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts->positions), &verts->positions, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

	glBindBuffer(GL_ARRAY_BUFFER, handles->tbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts->texcoords), &verts->texcoords, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

	glBindBuffer(GL_ARRAY_BUFFER, handles->cbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts->colours), &verts->colours, GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, (void *)0);
}
