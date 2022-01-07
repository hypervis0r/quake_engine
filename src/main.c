#include "main.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

int main(size_t argc, char** argv)
{
	GLFWwindow* window = NULL;
	Q_STATUS qResult = 0;

	/* initialize glfw */
	glfwInit();
	
	/*
		Set OpenGL version to 3.3
		and set the profile to core
	*/
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/*
		Create a new window for the renderer
	*/
	qResult = QWindowCreate(&window, Q_DEFAULT_WINDOW_WIDTH, Q_DEFAULT_WINDOW_HEIGHT, "QClone", NULL, NULL);
	if (qResult != Q_SUCCESS)
	{
		glfwTerminate();
		QFatalError("Failed to create window");
	}

	/*
		Set the GLAD GL loader to GLFW's GetProcAddress
	*/
	if (!gladLoadGLLoader(glfwGetProcAddress))
	{
		QFatalError("Failed to load GLAD GL Loader");
	}

	/*
		Set OpenGL viewport the the window size
	*/
	glViewport(0, 0, Q_DEFAULT_WINDOW_WIDTH, Q_DEFAULT_WINDOW_HEIGHT);

	/*
		Set a callback to change the viewport size whenever the window is resized
	*/
	glfwSetFramebufferSizeCallback(window, QWindowFramebufferSizeCallback);

	glEnable(GL_DEPTH_TEST);

	/*
		Enable mouse movement
	*/
	QInputInitializeMouse(window, 0.1f);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, QInputHandleMouseCallback);

	/*
		Enter the main render loop
	*/
	QRenderLoop(window);

	glfwTerminate();
	return 0;
}