#include "shader/material.h"

Q_STATUS QShaderPhongSetLight(
	struct Q_PHONGMATERIAL* mat,
	struct Q_CAMERAOBJECT* cam,
	vec3 ambient, vec3 diffuse, vec3 specular,
	vec3 position)
{
	glm_vec3_copy(ambient, mat->light.ambient);
	glm_vec3_copy(diffuse, mat->light.diffuse);
	glm_vec3_copy(specular, mat->light.specular);
	glm_vec3_copy(position, mat->light.position);
	glm_vec3_copy(cam->pos, mat->light.view_pos);

	return Q_SUCCESS;
}

Q_STATUS QShaderPhongApplyUniforms(struct Q_PHONGMATERIAL* mat)
{
	QShaderSetUniformInt(mat->shader_program_id, "material.diffuse", 0);
	QShaderSetUniformInt(mat->shader_program_id, "material.specular", 1);
	QShaderSetUniformFloat(mat->shader_program_id, "material.shininess", mat->shininess);
	QShaderSetUniformVec3(mat->shader_program_id, "material.diffuse_albedo", mat->diffuse_albedo);
	QShaderSetUniformVec3(mat->shader_program_id, "material.specular_albedo", mat->specular_albedo);

	QShaderSetUniformVec3(mat->shader_program_id, "light.position", mat->light.position);
	QShaderSetUniformVec3(mat->shader_program_id, "light.diffuse", mat->light.diffuse);
	QShaderSetUniformVec3(mat->shader_program_id, "light.ambient", mat->light.ambient);
	QShaderSetUniformVec3(mat->shader_program_id, "light.specular", mat->light.specular);

	QShaderSetUniformVec3(mat->shader_program_id, "viewPos", mat->light.view_pos);

	if (mat->diffuse_texture_id == (GLuint)-1 || mat->specular_texture_id == (GLuint)-1)
		QShaderSetUniformBool(mat->shader_program_id, "b_Untextured", GL_TRUE);

	return Q_SUCCESS;
}

Q_STATUS QShaderPhongCreate(
	struct Q_PHONGMATERIAL* mat,
	GLuint diffuse_texture, vec3 diffuse_albedo,
	GLuint specular_texture, vec3 specular_albedo,
	float shininess)
{
	GLuint vertex_id = 0;
	GLuint phong_id = 0;
	GLuint phong_shader = 0;

	QShaderCompile(&vertex_id, GL_VERTEX_SHADER, "resources\\shaders\\vertex.vert.glsl");
	QShaderCompile(&phong_id, GL_FRAGMENT_SHADER, "resources\\shaders\\phong.frag.glsl");

	GLuint phong_shader_c[] = { vertex_id, phong_id };

	QShaderLink(&phong_shader, phong_shader_c, ARRAY_COUNT(phong_shader_c, sizeof(phong_shader_c)));

	glDeleteShader(vertex_id);
	glDeleteShader(phong_id);

	mat->shader_program_id = phong_shader;
	mat->diffuse_texture_id = diffuse_texture;
	mat->specular_texture_id = specular_texture;
	mat->shininess = shininess;

	if (diffuse_albedo)
		glm_vec3_copy(diffuse_albedo, mat->diffuse_albedo);
	else
		glm_vec3_one(mat->diffuse_albedo);

	if (specular_albedo)
		glm_vec3_copy(specular_albedo, mat->specular_albedo);
	else
		glm_vec3_one(mat->specular_albedo);

	return Q_SUCCESS;
}