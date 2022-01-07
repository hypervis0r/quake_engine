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
		Copy image into the bound texture ID
	*/
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	*texture_id = new_texture_id;

	return Q_SUCCESS;
}