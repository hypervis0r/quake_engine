#include "player.h"

Q_STATUS QPlayerCreate(struct Q_PLAYEROBJECT* player)
{
	player->cam = malloc(sizeof(struct Q_CAMERAOBJECT));
	if (!player->cam)
		return Q_OUT_OF_MEMORY;

	vec3 cameraPos = { 0.0f, 0.0f,  3.0f };
	vec3 cameraFront = { 0.0f, 0.0f, -1.0f };
	vec3 cameraUp = { 0.0f, 1.0f, 0.0f };
	QRenderInitializeCameraObject(player->cam, cameraPos, cameraFront, cameraUp, 90.f);

	player->movement_speed = 2.5f;

	return Q_SUCCESS;
}

Q_STATUS QPlayerFree(struct Q_PLAYEROBJECT* player)
{
	free(player->cam);
	player->cam = NULL;

	return Q_SUCCESS;
}

Q_STATUS QPlayerMove(struct Q_PLAYEROBJECT* player, vec3 direction)
{
	glm_vec3_add(player->cam->pos, direction, player->cam->pos);

	return Q_SUCCESS;
}