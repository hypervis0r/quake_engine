#pragma once

#include <stdint.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "common.h"

#define TARGET_OPENGL_VERSION_MAJOR 3
#define TARGET_OPENGL_VERSION_MINOR 3

#define Q_DEFAULT_WINDOW_WIDTH 800
#define Q_DEFAULT_WINDOW_HEIGHT 600

void QWindowFramebufferSizeCallback(GLFWwindow* window, int width, int height);

Q_STATUS QWindowCreate(
	GLFWwindow** window, 
	int width, 
	int height, 
	char* window_name, 
	GLFWwindow* monitor, 
	GLFWwindow* share);