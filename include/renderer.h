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
#include "shader/phong.h"
#include "shader/fill.h"

/*
	Struct to hold camera values
	TODO: separate into its own translation unit
*/
struct Q_CAMERAOBJECT
{
	/*
		View Matrix
	*/
	vec3 pos;
	vec3 front;
	vec3 up;

	/*
		Euler Angles
	*/
	float yaw;
	float pitch;
	float roll;

	float fov;
};

/*
	Struct containing info on frames
*/
struct Q_FRAMECONTEXT
{
	float delta_time;
	float last_frame_time;
};

Q_STATUS QRenderMeshObject(
	struct Q_MESHOBJECT* mesh,
	struct Q_CAMERAOBJECT* cam,
	struct Q_MATERIAL* mat,
	vec3 world_pos,
	vec3 scale,
	vec3 rotation_axis, float angle);
Q_STATUS QRenderModelObject(
	struct Q_MODELOBJECT* model,
	struct Q_CAMERAOBJECT* cam,
	struct Q_MATERIAL* mat,
	vec3 world_pos,
	vec3 scale,
	vec3 rot_axis, float rot_angle);

Q_STATUS QRenderInitializeCameraObject(
	struct Q_CAMERAOBJECT* cam,
	vec3 pos, vec3 front, vec3 up,
	float fov);
Q_STATUS QRenderLoop(GLFWwindow* window);