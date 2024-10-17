#include <glad/glad.h>

#include "stdlib.h"

#include "debug.h"

int new_debug(debug_t *db) {
	db->n = 0;
	db->capacity = 10;

	db->points = (float *)calloc(sizeof(float), 2 * db->capacity);
	if (db->points == NULL) {
		return -1;
	}

	glGenVertexArrays(1, &db->vao);
	glGenBuffers(1, &db->vbo);

	glBindVertexArray(db->vao);
	glBindBuffer(GL_ARRAY_BUFFER, db->vbo);
	glBufferData(GL_ARRAY_BUFFER, db->capacity * sizeof(float), db->points, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

	glBindVertexArray(0);

	return 0;
}

int append_debug(debug_t *db, vec2 new) {
	if (db->n + 1 > db->capacity) {
		return -1;
	}

	db->points[db->n * 2] = new.x1;
	db->points[(db->n * 2) + 1] = new.x2;

	glBindVertexArray(db->vao);
	glBufferSubData(GL_ARRAY_BUFFER, db->n * 2 * sizeof(float), 2 * sizeof(float), (db->points + (db->n * 2)));

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

	db->n = db->n + 1;

	return 0;
}
