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

struct Q_PHONGMATERIAL
{
	GLuint diffuse_texture_id;
	vec3 diffuse_albedo;
	GLuint specular_texture_id;
	vec3 specular_albedo;
	float shininess;

	struct Q_LIGHTOBJECT *light;
};

Q_STATUS QShaderPhongSetLight(
	struct Q_MATERIAL* this,
	struct Q_LIGHTOBJECT* light,
	struct Q_CAMERAOBJECT* cam);
Q_STATUS QShaderPhongApplyUniforms(struct Q_MATERIAL* this);
Q_STATUS QShaderPhongBindTextures(struct Q_MATERIAL* this);
Q_STATUS QShaderPhongCreate(
	struct Q_MATERIAL* this,
	GLuint diffuse_texture, vec3 diffuse_albedo,
	GLuint specular_texture, vec3 specular_albedo,
	float shininess);
Q_STATUS QShaderPhongFree(struct Q_MATERIAL* mat);