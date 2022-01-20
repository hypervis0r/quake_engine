#pragma once

#include <stdint.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <cglm/cglm.h>

#include "common.h"
#include "input.h"
#include "shader/shader.h"
#include "shader/material.h"
#include "texture.h"
#include "model.h"
#include "renderer.h"

struct Q_OBJECT
{
	struct Q_MODELOBJECT* model;
	struct Q_MATERIAL* mat;
	vec3 box_collider;

	vec3 pos;
	vec3 front;
	vec3 up;
};

Q_STATUS QObjectCreate(
	struct Q_OBJECT* object, 
	const struct Q_MODELOBJECT* model, 
	const struct Q_MATERIAL* mat, 
	const vec3 box_collider);

Q_STATUS QObjectSetWorldPosition(struct Q_OBJECT* object, vec3 pos);