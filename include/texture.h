#pragma once

#include <stdint.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <stb_image.h>

#include "common.h"
#include "input.h"
#include "shader/shader.h"

Q_STATUS QTextureCreate(GLuint *texture_id, const char* path);