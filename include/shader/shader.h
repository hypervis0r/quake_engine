#pragma once

#include <stdint.h>
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "common.h"
#include "log.h"

Q_STATUS QShaderGetCompileStatus(GLuint shader_id);
Q_STATUS QShaderGetLinkStatus(GLuint shader_program_id);

Q_STATUS QShaderCompile(GLuint* shader_id, GLenum shader_type, const char* path);
Q_STATUS QShaderLink(GLuint* program_id, const GLuint* shader_id_array, uint32_t shader_count);

Q_STATUS QShaderSetUniformMat4(GLuint program_id, const char* name, const mat4 in);
Q_STATUS QShaderSetUniformMat3(GLuint program_id, const char* name, const mat3 in);
Q_STATUS QShaderSetUniformVec3(GLuint program_id, const char* name, const vec3 in);
Q_STATUS QShaderSetUniformFloat(GLuint program_id, const char* name, const float in);
Q_STATUS QShaderSetUniformInt(GLuint program_id, const char* name, const int in);
Q_STATUS QShaderSetUniformBool(GLuint program_id, const char* name, const GLboolean in);