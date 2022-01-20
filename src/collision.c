#include "collision.h"

Q_BOOL QAABBIsColliding(struct Q_OBJECT* a, struct Q_OBJECT* b)
{
	/*
		TODO: this is going to break in the future.
		at the time of writing this, Q_AABB is just 2 `vec3`s.
		glm_aabb_contains() expects AABB boxes of type `vec3[2]`
		we can just pass the structs in as they basically function the same.

		if a variable ever gets put before the two members, you're
		going to have a bad time debugging.
	*/

	vec3 a_aabb[2];
	vec3 b_aabb[2];

	glm_vec3_add(a->pos, a->box_collider, a_aabb[1]);
	glm_vec3_add(b->pos, b->box_collider, b_aabb[1]);

	return glm_aabb_contains(a_aabb, b_aabb);
}