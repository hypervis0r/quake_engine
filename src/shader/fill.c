#include "shader/fill.h"

Q_STATUS QShaderFillApplyUniforms(struct Q_MATERIAL* this)
{
	struct Q_FILLMATERIAL* mat = this->material_data;

	QShaderSetUniformVec3(this->shader_program_id, "color", mat->color);

	return Q_SUCCESS;
}

Q_STATUS QShaderFillCreate(struct Q_MATERIAL* this, vec3 color)
{
	GLuint vertex_id = 0;
	GLuint fill_id = 0;
	GLuint fill_shader = 0;

	QShaderCompile(&vertex_id, GL_VERTEX_SHADER, "resources\\shaders\\vertex.vert.glsl");
	QShaderCompile(&fill_id, GL_FRAGMENT_SHADER, "resources\\shaders\\fill.frag.glsl");

	GLuint fill_shader_compiled[] = { vertex_id, fill_id };

	QShaderLink(&fill_shader, fill_shader_compiled, ARRAY_COUNT(fill_shader_compiled, sizeof(fill_shader_compiled)));

	glDeleteShader(vertex_id);
	glDeleteShader(fill_id);

	this->shader_program_id = fill_shader;
	this->apply_shader_uniforms = QShaderFillApplyUniforms;
	this->bind_textures = NULL;
	this->set_shader_light = NULL;

	this->material_size = sizeof(struct Q_FILLMATERIAL);
	this->material_data = malloc(this->material_size);
	if (!this->material_data)
	{
		glDeleteProgram(this->shader_program_id);

		return Q_OUT_OF_MEMORY;
	}

	struct Q_FILLMATERIAL* mat = this->material_data;

	glm_vec3_copy(color, mat->color);

	return Q_SUCCESS;
}

Q_STATUS QShaderFillFree(struct Q_MATERIAL* mat)
{
	glDeleteProgram(mat->shader_program_id);

	free(mat->material_data);

	return Q_SUCCESS;
}