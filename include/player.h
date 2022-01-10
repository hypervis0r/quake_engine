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

struct Q_PLAYEROBJECT
{
	struct Q_CAMERAOBJECT* cam;
	float movement_speed;
};

Q_STATUS QPlayerCreate(struct Q_PLAYEROBJECT* player);

Q_STATUS QPlayerMove(struct Q_PLAYEROBJECT* player, vec3 direction);