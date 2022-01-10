#include "texture.h"

Q_STATUS QTextureCreate(GLuint* texture_id, const char* path)
{
	if (!texture_id || !path)
		return Q_ERROR;

	GLuint new_texture_id = 0;
	glGenTextures(1, &new_texture_id);
	glBindTexture(GL_TEXTURE_2D, new_texture_id);

	/* 
		set the texture wrapping / filtering options(on the currently bound texture object)
	*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	/*
		Load image into buffer
	*/
	stbi_set_flip_vertically_on_load(TRUE);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
	if (!data)
	{
		QLogWarn("Failed to load texture (%s)\n", path);
		return Q_IO_ERROR;
	}

	/*
		Set the channel type
		TODO: this is so fucking retarded
	*/
	GLint channel_type = 0;
	switch (nrChannels)
	{
	case 3: 
		channel_type = GL_RGB; break;
	case 4: 
		channel_type = GL_RGBA; break;
	default: 
		QLogWarn("Unsupported channel value in texture (%s)\n", path); return;
	}

	/*
		Copy image into the bound texture ID
	*/
	glTexImage2D(GL_TEXTURE_2D, 0, channel_type, width, height, 0, channel_type, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	*texture_id = new_texture_id;

	return Q_SUCCESS;
}

Q_STATUS QTextureMapCreate(
	struct Q_TEXTUREMAP* map,
	const char* diffuse_path,
	const char* specular_path,
	const char* emmision_path,
	vec3 diffuse_albedo,
	vec3 specular_albedo,
	vec3 emmision_albedo)
{
	if (!map)
		return Q_ERROR;

	map->diffuse_id = 0;
	map->specular_id = 0;
	map->emission_id = 0;

	/*
		We don't check for errors here because
		it doesn't really matter if a texture fails to load.
	*/
	if (diffuse_path)
		QTextureCreate(&map->diffuse_id, diffuse_path);
	if (specular_path)
		QTextureCreate(&map->specular_id, specular_path);
	if (emmision_path)
		QTextureCreate(&map->emission_id, emmision_path);

	/*
		Set color albedos.
		if the albedo wasn't specified, just set it to `1.0`s,
		so we don't fuck up the textures
	*/
	if (diffuse_albedo)
		glm_vec3_copy(diffuse_albedo, map->diffuse_albedo);
	else
		glm_vec3_one(map->diffuse_albedo);
	
	if (specular_albedo)
		glm_vec3_copy(specular_albedo, map->specular_albedo);
	else
		glm_vec3_one(map->specular_albedo);
	
	if (emmision_albedo)
		glm_vec3_copy(emmision_albedo, map->emission_albedo);
	else
		glm_vec3_one(map->emission_albedo);

	return Q_SUCCESS;
}