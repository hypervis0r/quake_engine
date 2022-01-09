#pragma once

#include <stdint.h>
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "common.h"
#include "log.h"
#include "renderer.h"
#include "shader/shader.h"
#include "shader/material.h"

struct Q_FILLMATERIAL
{
	vec3 color;
};

Q_STATUS QShaderFillApplyUniforms(struct Q_MATERIAL* this);
Q_STATUS QShaderFillCreate(struct Q_MATERIAL* this, vec3 color);
Q_STATUS QShaderPhongFree(struct Q_MATERIAL* mat);