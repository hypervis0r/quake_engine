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

struct Q_AABB
{
	vec3 min_pos;
	vec3 max_pos;
};

Q_BOOL QAABBIsColliding(struct Q_AABB *a, struct Q_AABB* b);