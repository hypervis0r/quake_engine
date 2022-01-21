#include "collision.h"

Q_BOOL QAABBIsColliding(struct Q_OBJECT* a, struct Q_OBJECT* b)
{
	if (!a || !b)
		return FALSE;
	if (!a->box_collider || !b->box_collider)
		return FALSE;

	vec3 a_aabb[2];
	vec3 b_aabb[2];

	vec3 a_box_collider_positive_half;
	vec3 a_box_collider_negative_half;
	vec3 b_box_collider_positive_half;
	vec3 b_box_collider_negative_half;

	glm_vec3_divs(a->box_collider, 2., a_box_collider_positive_half);
	glm_vec3_negate_to(a_box_collider_positive_half, a_box_collider_negative_half);

	glm_vec3_divs(b->box_collider, 2., b_box_collider_positive_half);
	glm_vec3_negate_to(b_box_collider_positive_half, b_box_collider_negative_half);

	/*
		Min vec3
	*/
	glm_vec3_add(a->pos, a_box_collider_negative_half, a_aabb[0]);
	glm_vec3_add(b->pos, b_box_collider_negative_half, b_aabb[0]);

	/*
		Max vec3
	*/
	glm_vec3_add(a->pos, a_box_collider_positive_half, a_aabb[1]);
	glm_vec3_add(b->pos, b_box_collider_positive_half, b_aabb[1]);

	return glm_aabb_aabb(a_aabb, b_aabb);
}