#pragma once

#include <stdint.h>
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "common.h"
#include "log.h"
#include "renderer.h"

struct Q_PHONGLIGHT
{
	vec3 position;
	vec3 view_pos;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Q_PHONGMATERIAL
{
	GLuint shader_program_id;

	GLuint diffuse_texture_id;
	vec3 diffuse_albedo;
	GLuint specular_texture_id;
	vec3 specular_albedo;
	float shininess;

	struct Q_PHONGLIGHT light;
};

Q_STATUS QShaderPhongSetLight(
	struct Q_PHONGMATERIAL* mat,
	struct Q_CAMERAOBJECT* cam,
	vec3 ambient, vec3 diffuse, vec3 specular,
	vec3 position);
Q_STATUS QShaderPhongApplyUniforms(struct Q_PHONGMATERIAL* mat);
Q_STATUS QShaderPhongCreate(
	struct Q_PHONGMATERIAL* mat,
	GLuint diffuse_texture, vec3 diffuse_albedo,
	GLuint specular_texture, vec3 specular_albedo,
	float shininess);