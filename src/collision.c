#include "collision.h"

Q_BOOL QAABBIsColliding(struct Q_AABB* a, struct Q_AABB* b)
{
	/*
		TODO: this is going to break in the future.
		at the time of writing this, Q_AABB is just 2 `vec3`s.
		glm_aabb_contains() expects AABB boxes of type `vec3[2]`
		we can just pass the structs in as they basically function the same.

		if a variable ever gets put before the two members, you're
		going to have a bad time debugging.
	*/
	return glm_aabb_contains(a, b);
}