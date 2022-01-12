#include "input.h"

struct Q_MOUSECONTEXT g_mouse_ctx = { 0 };

void QInputHandleMouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	g_mouse_ctx.xpos = xpos;
	g_mouse_ctx.ypos = ypos;
}

Q_STATUS QInputProcessKeyboard(GLFWwindow* window, struct Q_PLAYEROBJECT* player, struct Q_FRAMECONTEXT* frame_ctx)
{
	vec3 movement_direction;
	const float cameraSpeed = player->movement_speed * frame_ctx->delta_time; // adjust accordingly

	glm_normalize(player->front);
	
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		glm_vec3_scale(player->front, cameraSpeed, movement_direction);
		glm_vec3_add(player->velocity, movement_direction, player->velocity);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		glm_vec3_scale(player->front, cameraSpeed, movement_direction);
		glm_vec3_negate(movement_direction);

		glm_vec3_add(player->velocity, movement_direction, player->velocity);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		glm_cross(player->front, player->up, movement_direction);
		glm_normalize(movement_direction);
		glm_vec3_scale(movement_direction, cameraSpeed, movement_direction);
		glm_vec3_negate(movement_direction);

		glm_vec3_add(player->velocity, movement_direction, player->velocity);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		glm_cross(player->front, player->up, movement_direction);
		glm_normalize(movement_direction);
		glm_vec3_scale(movement_direction, cameraSpeed, movement_direction);

		glm_vec3_add(player->velocity, movement_direction, player->velocity);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		if (player->is_grounded && frame_ctx->last_frame_time - player->last_jump_tick > 0.25)
		{
			glm_vec3_scale(player->up, player->jump_force, movement_direction);
			glm_vec3_add(player->velocity, movement_direction, player->velocity);

			player->last_jump_tick = frame_ctx->last_frame_time;
		}
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

Q_STATUS QInputProcessMouse(GLFWwindow* window, struct Q_PLAYEROBJECT* player, struct Q_FRAMECONTEXT* frame_ctx)
{
	float xoffset = g_mouse_ctx.xpos - g_mouse_ctx.last_xpos;
	float yoffset = g_mouse_ctx.last_ypos - g_mouse_ctx.ypos;
	g_mouse_ctx.last_xpos = g_mouse_ctx.xpos;
	g_mouse_ctx.last_ypos = g_mouse_ctx.ypos;
	
	xoffset *= g_mouse_ctx.sensitivity;
	yoffset *= g_mouse_ctx.sensitivity;

	player->cam->yaw += xoffset;
	player->cam->pitch += yoffset;

	if (player->cam->pitch > 89.0f)
		player->cam->pitch = 89.0f;
	if (player->cam->pitch < -89.0f)
		player->cam->pitch = -89.0f;

	vec3 direction;
	direction[0] = cos(glm_rad(player->cam->yaw)) * cos(glm_rad(player->cam->pitch));
	direction[1] = sin(glm_rad(player->cam->pitch));
	direction[2] = sin(glm_rad(player->cam->yaw)) * cos(glm_rad(player->cam->pitch));
	glm_normalize(direction);

	glm_vec3_copy(direction, player->cam->front);

	player->front[0] = direction[0];
	player->front[2] = direction[2];

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

Q_STATUS QInputProcess(GLFWwindow* window, struct Q_PLAYEROBJECT* player, struct Q_FRAMECONTEXT* frame_ctx)
{
	if (!window)
		return Q_ERROR;

	QInputProcessMouse(window, player, frame_ctx);
	QInputProcessKeyboard(window, player, frame_ctx);

	return Q_SUCCESS;
}