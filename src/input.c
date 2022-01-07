#include "input.h"

struct Q_MOUSECONTEXT g_mouse_ctx = { 0 };

void QInputHandleMouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	g_mouse_ctx.xpos = xpos;
	g_mouse_ctx.ypos = ypos;
}

Q_STATUS QInputProcessKeyboard(GLFWwindow* window, struct Q_CAMERAOBJECT* cam, struct Q_FRAMECONTEXT* frame_ctx)
{
	const float cameraSpeed = 2.5f * frame_ctx->delta_time; // adjust accordingly
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		vec3 temp;
		glm_vec3_scale(cam->front, cameraSpeed, temp);
		glm_vec3_add(cam->pos, temp, cam->pos);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		vec3 temp;
		glm_vec3_scale(cam->front, cameraSpeed, temp);
		glm_vec3_sub(cam->pos, temp, cam->pos);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		vec3 temp;

		glm_cross(cam->front, cam->up, temp);
		glm_normalize(temp);
		glm_vec3_scale(temp, cameraSpeed, temp);

		glm_vec3_sub(cam->pos, temp, cam->pos);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		vec3 temp;

		glm_cross(cam->front, cam->up, temp);
		glm_normalize(temp);
		glm_vec3_scale(temp, cameraSpeed, temp);

		glm_vec3_add(cam->pos, temp, cam->pos);
	}

	/*
		If ESC pressed, close the window
		TODO: Set key callbacks
	*/
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, TRUE);
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	return Q_SUCCESS;
}

Q_STATUS QInputProcessMouse(GLFWwindow* window, struct Q_CAMERAOBJECT* cam, struct Q_FRAMECONTEXT* frame_ctx)
{
	float xoffset = g_mouse_ctx.xpos - g_mouse_ctx.last_xpos;
	float yoffset = g_mouse_ctx.last_ypos - g_mouse_ctx.ypos;
	g_mouse_ctx.last_xpos = g_mouse_ctx.xpos;
	g_mouse_ctx.last_ypos = g_mouse_ctx.ypos;
	
	xoffset *= g_mouse_ctx.sensitivity;
	yoffset *= g_mouse_ctx.sensitivity;

	cam->yaw += xoffset;
	cam->pitch += yoffset;

	if (cam->pitch > 89.0f)
		cam->pitch = 89.0f;
	if (cam->pitch < -89.0f)
		cam->pitch = -89.0f;

	vec3 direction;
	direction[0] = cos(glm_rad(cam->yaw)) * cos(glm_rad(cam->pitch));
	direction[1] = sin(glm_rad(cam->pitch));
	direction[2] = sin(glm_rad(cam->yaw)) * cos(glm_rad(cam->pitch));
	glm_normalize(direction);

	glm_vec3_copy(direction, cam->front);

	return Q_SUCCESS;
}

Q_STATUS QInputInitializeMouse(GLFWwindow* window, float sensitivity)
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);

	/*
		Center mouse at center of screen
	*/
	g_mouse_ctx.last_xpos = (double)width / 2.;
	g_mouse_ctx.last_ypos = (double)height / 2.;

	g_mouse_ctx.xpos = 0.f;
	g_mouse_ctx.ypos = 0.f;

	g_mouse_ctx.sensitivity = sensitivity;

	return Q_SUCCESS;
}

Q_STATUS QInputProcess(GLFWwindow* window, struct Q_CAMERAOBJECT* cam, struct Q_FRAMECONTEXT* frame_ctx)
{
	if (!window)
		return Q_ERROR;

	QInputProcessMouse(window, cam, frame_ctx);
	QInputProcessKeyboard(window, cam, frame_ctx);

	return Q_SUCCESS;
}