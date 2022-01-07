#include "window.h"

void QWindowFramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

Q_STATUS QWindowCreate(
	GLFWwindow** window,
	int width,
	int height,
	char* window_name,
	GLFWmonitor* monitor,
	GLFWwindow* share)
{
	if (!window)
		return Q_ERROR;

	*window = glfwCreateWindow(width, height, window_name, monitor, share);
	if (*window == NULL)
		return Q_ERROR;

	/*
		Set the current GLFW thread context to use new window
		TODO: this is probably bad?
	*/
	glfwMakeContextCurrent(*window);

	return Q_SUCCESS;
}