#pragma once

#include <stdint.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <cglm/cglm.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "common.h"
#include "input.h"
#include "shader/shader.h"
#include "texture.h"
#include "log.h"

struct Q_VERTEX
{
	vec3 pos;
	vec3 normal;
	vec2 tex_coords;
};

/*
	Struct to handle rendering models
*/
struct Q_MESHOBJECT
{
	struct Q_VERTEX* vertices;
	size_t vertices_size;
	size_t vertices_count;
	GLuint* indices;
	size_t indices_size;
	size_t indices_count;
	GLuint* attrib_order;
	size_t attrib_size;

	GLuint vbo;
	GLuint vao;
	GLuint ebo;
};

struct Q_MODELOBJECT
{
	struct Q_MESHOBJECT* meshes;
	size_t meshes_size;
	size_t meshes_count;
};

Q_STATUS QMeshCreate(
	struct Q_MESHOBJECT* vertex_obj,
	struct Q_VERTEX* vertices, size_t vertices_size,
	GLuint* indices, size_t indices_size,
	GLuint* attrib_order, size_t attrib_size);
Q_STATUS QMeshDelete(struct Q_MESHOBJECT* mesh);

Q_STATUS QModelLoad(struct Q_MODELOBJECT *model, const char* path);
Q_STATUS QModelFree(struct Q_MODELOBJECT* model);