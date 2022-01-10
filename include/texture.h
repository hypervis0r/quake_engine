#pragma once

#include <stdint.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <stb_image.h>

#include "common.h"
#include "input.h"
#include "shader/shader.h"

struct Q_TEXTUREMAP
{
	/*
		Texture IDs
	*/
	GLuint diffuse_id;
	GLuint specular_id;
	GLuint emission_id;

	/*
		In the absence of textures, we can fall back to
		flat color albedos (or mix them in with the textures)
	*/
	vec3 diffuse_albedo;
	vec3 specular_albedo;
	vec3 emission_albedo;
};

Q_STATUS QTextureCreate(GLuint *texture_id, const char* path);
Q_STATUS QTextureMapCreate(
	struct Q_TEXTUREMAP* map, 
	const char* diffuse_path, 
	const char* specular_path, 
	const char* emmision_path,
	vec3 diffuse_albedo,
	vec3 specular_albedo,
	vec3 emmision_albedo);