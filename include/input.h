#pragma once

#include <stdint.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "common.h"
#include "renderer.h"

struct Q_MOUSECONTEXT
{
	double xpos;
	double ypos;
	double last_xpos;
	double last_ypos;
	float sensitivity;
};

extern struct Q_MOUSECONTEXT g_mouse_ctx;

void QInputHandleMouseCallback(GLFWwindow* window, double xpos, double ypos);
Q_STATUS QInputInitializeMouse(GLFWwindow* window, float sensitivity);

Q_STATUS QInputProcessMouse(GLFWwindow* window, struct Q_CAMERAOBJECT* cam, struct Q_FRAMECONTEXT* frame_ctx);
Q_STATUS QInputProcessKeyboard(GLFWwindow* window, struct Q_CAMERAOBJECT* cam, struct Q_FRAMECONTEXT* frame_ctx);
Q_STATUS QInputProcess(GLFWwindow* window, struct Q_CAMERAOBJECT *cam, struct Q_FRAMECONTEXT *frame_ctx);
