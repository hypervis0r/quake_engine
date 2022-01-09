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

struct Q_LIGHTOBJECT
{
	vec3 position;
	vec3 view_pos;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Q_MATERIAL
{
	GLuint shader_program_id;

	Q_STATUS(*apply_shader_uniforms)(struct Q_MATERIAL* this);
	Q_STATUS(*set_shader_light)(
		struct Q_MATERIAL* this, 
		struct Q_LIGHTOBJECT* light, 
		struct Q_CAMERAOBJECT *cam);
	Q_STATUS(*bind_textures)(struct Q_MATERIAL* this);

	void* material_data;
	size_t material_size;
};

Q_STATUS QShaderCreateLight(
	struct Q_LIGHTOBJECT* light,
	vec3 ambient, vec3 diffuse, vec3 specular, vec3 position);
