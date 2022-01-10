#include "shader/shader.h"

Q_STATUS QShaderGetCompileStatus(GLuint shader_id)
{
	int success = 0;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

	return success ? Q_SUCCESS : Q_SHADER_COMPILE_FAILED;
}

Q_STATUS QShaderGetLinkStatus(GLuint shader_program_id)
{
	int success = 0;
	glGetProgramiv(shader_program_id, GL_LINK_STATUS, &success);

	return success ? Q_SUCCESS : Q_SHADER_LINK_FAILED;
}

Q_STATUS QShaderCompile(GLuint* shader_id, GLenum shader_type, char* path)
{
	if (!shader_id || !path)
		return Q_ERROR;

	FILE* shader_file = NULL;
	fopen_s(&shader_file, path, "rb");
	if (!shader_file)
		return Q_IO_ERROR;

	fseek(shader_file, 0, SEEK_END);
	long fsize = ftell(shader_file);
	fseek(shader_file, 0, SEEK_SET);  /* same as rewind(f); */

	char* file_buffer = malloc(fsize + 1);
	if (!file_buffer)
	{
		fclose(shader_file);
		return Q_OUT_OF_MEMORY;
	}

	fread(file_buffer, fsize, 1, shader_file);
	fclose(shader_file);

	file_buffer[fsize] = 0;

	unsigned int new_shader_id;
	new_shader_id = glCreateShader(shader_type);

	glShaderSource(new_shader_id, 1, &file_buffer, NULL);
	glCompileShader(new_shader_id);

	free(file_buffer);

	if (QShaderGetCompileStatus(new_shader_id) != Q_SUCCESS)
	{
		char infoLog[1024];
		GLint balls = 0;

		glGetShaderiv(new_shader_id, GL_INFO_LOG_LENGTH, &balls);

		glGetShaderInfoLog(new_shader_id, sizeof(infoLog), NULL, infoLog);
		QFatalError("Shader (%s) failed to compile: %s\n", path, infoLog);
	}

	*shader_id = new_shader_id;

	return Q_SUCCESS;
}

Q_STATUS QShaderLink(GLuint* program_id, const GLuint* shader_id_array, uint32_t shader_count)
{
	if (!program_id || !shader_id_array)
		return Q_ERROR;

	unsigned int new_program_id;
	new_program_id = glCreateProgram();

	for (uint32_t i = 0; i < shader_count; i++)
	{
		glAttachShader(new_program_id, shader_id_array[i]);
	}

	glLinkProgram(new_program_id);

	if (QShaderGetLinkStatus(new_program_id) != Q_SUCCESS)
	{
		char infoLog[512];

		glGetProgramInfoLog(new_program_id, sizeof(infoLog), NULL, infoLog);
		QFatalError("Shader program failed to link: %s\n", infoLog);
	}

	*program_id = new_program_id;

	return Q_SUCCESS;
}

Q_STATUS QShaderSetUniformMat4(GLuint program_id, const char* name, const mat4 in)
{
	glUseProgram(program_id);

	int loc = glGetUniformLocation(program_id, name);
	if (loc == -1)
		return Q_SHADER_UNIFORM_NONEXIST;

	glUniformMatrix4fv(loc, 1, GL_FALSE, (float*)in);

	return Q_SUCCESS;
}

Q_STATUS QShaderSetUniformMat3(GLuint program_id, const char* name, const mat3 in)
{
	glUseProgram(program_id);

	int loc = glGetUniformLocation(program_id, name);
	if (loc == -1)
		return Q_SHADER_UNIFORM_NONEXIST;

	glUniformMatrix3fv(loc, 1, GL_FALSE, (float*)in);

	return Q_SUCCESS;
}

Q_STATUS QShaderSetUniformVec3(GLuint program_id, const char* name, const vec3 in)
{
	glUseProgram(program_id);

	int loc = glGetUniformLocation(program_id, name);
	if (loc == -1)
		return Q_SHADER_UNIFORM_NONEXIST;

	glUniform3fv(loc, 1, in);

	return Q_SUCCESS;
}

Q_STATUS QShaderSetUniformFloat(GLuint program_id, const char* name, const float in)
{
	glUseProgram(program_id);

	int loc = glGetUniformLocation(program_id, name);
	if (loc == -1)
		return Q_SHADER_UNIFORM_NONEXIST;

	glUniform1f(loc, in);

	return Q_SUCCESS;
}

Q_STATUS QShaderSetUniformInt(GLuint program_id, const char* name, const int in)
{
	glUseProgram(program_id);

	int loc = glGetUniformLocation(program_id, name);
	if (loc == -1)
		return Q_SHADER_UNIFORM_NONEXIST;

	glUniform1i(loc, in);

	return Q_SUCCESS;
}

Q_STATUS QShaderSetUniformBool(GLuint program_id, const char* name, const GLboolean in)
{
	return QShaderSetUniformInt(program_id, name, in);
}