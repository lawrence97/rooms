#include <glad/glad.h>

#include "batch.h"
#include "scene.h"

void new_batch(scene_t *scene, int *batch_index_ptr) {
	unsigned int batch_index = 0;
	batch_t b;
	update_construct(scene->construct, scene->prev, scene->next);
	*scene->prev = *scene->next;
	send_pipeline(scene->pipeline, scene->construct);
	upload_pipeline(scene->pipeline, &b);
	batch_index = (scene->n_batches % (scene->capacity_batches));

	batch_t old_batch = scene->batches[batch_index];
	handles_t handles = old_batch.handles;
	unsigned int buffers[3] = {handles.pbo, handles.tbo, handles.cbo};
	glDeleteVertexArrays(1, &handles.vao);
	glDeleteBuffers(3, buffers);

	scene->batches[batch_index] = b;
	scene->n_batches++;
	*batch_index_ptr = batch_index;
}
