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
	player->gravity = 1.f;
	player->is_grounded = TRUE;

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

	if (player->cam->pos[1] > 0.)
		player->is_grounded = FALSE;

	return Q_SUCCESS;
}

Q_STATUS QPlayerUpdateGravity(struct Q_PLAYEROBJECT* player, struct Q_FRAMECONTEXT* frame_ctx)
{
	if (player->is_grounded)
		return Q_SUCCESS;

	player->cam->pos[1] -= player->gravity * frame_ctx->delta_time;

	if (player->cam->pos[1] < 0)
		player->cam->pos[1] = 0;

	if (player->cam->pos[1] == 0)
		player->is_grounded = TRUE;

	return Q_SUCCESS;
}