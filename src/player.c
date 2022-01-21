#include "player.h"

Q_STATUS QPlayerCreate(struct Q_PLAYEROBJECT* player, vec3 start_pos, float movement_speed, float gravity, float jump_force)
{
	player->cam = malloc(sizeof(*player->cam));
	if (!player->cam)
		return Q_OUT_OF_MEMORY;

	vec3 cameraFront = { 0.0f, 0.0f, -1.0f };
	vec3 cameraUp = { 0.0f, 1.0f, 0.0f };
	QRenderInitializeCameraObject(player->cam, start_pos, cameraFront, cameraUp, 90.f);

	player->object = malloc(sizeof(*player->object));
	if (!player->object)
	{
		free(player->cam);
		player->cam = NULL;

		return Q_OUT_OF_MEMORY;
	}

	vec3 box_collider = { 1., 1., 1. };
	QObjectCreate(player->object, NULL, NULL, box_collider);

	QObjectSetWorldPosition(player->object, start_pos);

	player->movement_speed = movement_speed;
	player->gravity = gravity;
	player->jump_force = jump_force;
	player->is_grounded = TRUE;
	player->last_jump_tick = 0.;

	glm_vec3_zero(player->velocity);

	glm_vec3_fill(player->friction, 0.75);
	player->friction[1] = 1.;

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
	glm_vec3_add(player->object->pos, direction, player->object->pos);

	if (player->cam->pos[1] > 0.)
		player->is_grounded = FALSE;

	return Q_SUCCESS;
}

Q_STATUS QPlayerUpdateVelocity(struct Q_PLAYEROBJECT* player, struct Q_FRAMECONTEXT* frame_ctx)
{
	if (player->is_grounded == FALSE)
		player->velocity[1] -= player->gravity * frame_ctx->delta_time;

	glm_vec3_mul(player->velocity, player->friction, player->velocity);

	QPlayerMove(player, player->velocity);

	if (player->cam->pos[1] < 0)
	{
		player->cam->pos[1] = 0;
		player->object->pos[1] = 0;
	}

	if (player->cam->pos[1] == 0)
	{
		player->is_grounded = TRUE;
		player->velocity[1] = 0;
	}

	return Q_SUCCESS;
}