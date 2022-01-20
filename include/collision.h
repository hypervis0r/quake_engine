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
#include "object.h"

Q_BOOL QAABBIsColliding(struct Q_OBJECT *a, struct Q_OBJECT* b);