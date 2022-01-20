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
#include "collision.h"

struct Q_PLAYEROBJECT
{
	struct Q_CAMERAOBJECT* cam;
	struct Q_OBJECT* object;

	vec3 velocity;
	vec3 friction;
	float movement_speed;
	float gravity;
	float jump_force;
	Q_BOOL is_grounded;
	double last_jump_tick;
};

Q_STATUS QPlayerCreate(struct Q_PLAYEROBJECT* player, float movement_speed, float gravity, float jump_force);

Q_STATUS QPlayerMove(struct Q_PLAYEROBJECT* player, vec3 direction);
Q_STATUS QPlayerUpdateVelocity(struct Q_PLAYEROBJECT* player, struct Q_FRAMECONTEXT* frame_ctx);