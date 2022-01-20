#include "object.h"

Q_STATUS QObjectCreate(
	struct Q_OBJECT* object,
	const struct Q_MODELOBJECT* model,
	const struct Q_MATERIAL* mat,
	const vec3 box_collider)
{
	if (!object)
		return Q_ERROR;

	object->model = model;
	object->mat = mat;
	
	if (box_collider)
		glm_vec3_copy(box_collider, object->box_collider);
	else
		glm_vec3_zero(object->box_collider);

	glm_vec3_zero(object->pos);

	vec3 front = { 0.0f, 0.0f, -1.0f };
	vec3 up = { 0.0f, 1.0f, 0.0f };

	glm_vec3_copy(front, object->front);
	glm_vec3_copy(up, object->up);

	return Q_SUCCESS;
}

Q_STATUS QObjectSetWorldPosition(struct Q_OBJECT* object, vec3 pos)
{
	glm_vec3_copy(pos, object->pos);

	return Q_SUCCESS;
}