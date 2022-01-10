#include "shader/phong.h"


Q_STATUS QShaderPhongSetLight(
	struct Q_MATERIAL* this,
	struct Q_LIGHTOBJECT* light,
	struct Q_CAMERAOBJECT* cam)
{
	struct Q_PHONGMATERIAL* mat = this->material_data;

	mat->light = light;
	glm_vec3_copy(cam->pos, mat->light->view_pos);

	return Q_SUCCESS;
}

Q_STATUS QShaderPhongBindTextures(struct Q_MATERIAL* this)
{
	struct Q_PHONGMATERIAL* mat = this->material_data;

	if (mat->texture_map->diffuse_id != 0)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mat->texture_map->diffuse_id);
	}

	if (mat->texture_map->specular_id != 0)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, mat->texture_map->specular_id);
	}

	if (mat->texture_map->emission_id != 0)
	{
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, mat->texture_map->emission_id);
	}

	return Q_SUCCESS;
}

Q_STATUS QShaderPhongApplyUniforms(struct Q_MATERIAL* this)
{
	struct Q_PHONGMATERIAL* mat = this->material_data;

	QShaderSetUniformInt(this->shader_program_id, "material.diffuse", 0);
	QShaderSetUniformInt(this->shader_program_id, "material.specular", 1);
	QShaderSetUniformInt(this->shader_program_id, "material.emission", 2);
	QShaderSetUniformFloat(this->shader_program_id, "material.shininess", mat->shininess);
	QShaderSetUniformVec3(this->shader_program_id, "material.diffuse_albedo", mat->texture_map->diffuse_albedo);
	QShaderSetUniformVec3(this->shader_program_id, "material.specular_albedo", mat->texture_map->specular_albedo);
	QShaderSetUniformVec3(this->shader_program_id, "material.emission_albedo", mat->texture_map->emission_albedo);

	QShaderSetUniformVec3(this->shader_program_id, "light.position", mat->light->position);
	QShaderSetUniformVec3(this->shader_program_id, "light.diffuse", mat->light->diffuse);
	QShaderSetUniformVec3(this->shader_program_id, "light.ambient", mat->light->ambient);
	QShaderSetUniformVec3(this->shader_program_id, "light.specular", mat->light->specular);

	QShaderSetUniformVec3(this->shader_program_id, "viewPos", mat->light->view_pos);

	if (mat->texture_map->diffuse_id == 0 || mat->texture_map->specular_id == 0)
		QShaderSetUniformBool(this->shader_program_id, "b_Untextured", GL_TRUE);

	return Q_SUCCESS;
}

Q_STATUS QShaderPhongCreate(
	struct Q_MATERIAL* this,
	struct Q_TEXTUREMAP* map,
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

	this->shader_program_id = phong_shader;
	this->apply_shader_uniforms = QShaderPhongApplyUniforms;
	this->set_shader_light = QShaderPhongSetLight;
	this->bind_textures = QShaderPhongBindTextures;

	this->material_size = sizeof(struct Q_PHONGMATERIAL);
	this->material_data = malloc(this->material_size);
	if (!this->material_data)
	{
		glDeleteProgram(this->shader_program_id);

		return Q_OUT_OF_MEMORY;
	}

	struct Q_PHONGMATERIAL* mat = this->material_data;

	mat->texture_map = map;
	mat->shininess = shininess;

	return Q_SUCCESS;
}

Q_STATUS QShaderPhongFree(struct Q_MATERIAL* mat)
{
	glDeleteProgram(mat->shader_program_id);

	free(mat->material_data);

	return Q_SUCCESS;
}