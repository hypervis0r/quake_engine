#include "shader/material.h"

Q_STATUS QShaderCreateLight(
	struct Q_LIGHTOBJECT* light,
	vec3 ambient, vec3 diffuse, vec3 specular, vec3 position)
{
	glm_vec3_copy(ambient, light->ambient);
	glm_vec3_copy(diffuse, light->diffuse);
	glm_vec3_copy(specular, light->specular);
	glm_vec3_copy(position, light->position);

	return Q_SUCCESS;
}