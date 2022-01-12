#include "model.h"

Q_STATUS QMeshCreate(
	struct Q_MESHOBJECT* vertex_obj,
	struct Q_VERTEX* vertices, size_t vertices_size,
	GLuint* indices, size_t indices_size,
	GLuint* attrib_order, size_t attrib_size)
{
	if (!vertex_obj || !vertices || !attrib_order)
		return Q_ERROR;

	GLuint current_offset = 0;
	GLuint next_vertex_offset = 0;

	/*
		Get the offset to the next vertex by adding the attrib order
		TODO: this is dumb
	*/
	for (GLuint i = 0; i < ARRAY_COUNT(attrib_order, attrib_size); i++)
		next_vertex_offset += attrib_order[i];

	/*
		Set the obj vertices, indices, shader program
	*/
	vertex_obj->vertices = vertices;
	vertex_obj->vertices_size = vertices_size;
	vertex_obj->vertices_count = ARRAY_COUNT(vertices, vertices_size);
	vertex_obj->indices = indices;
	vertex_obj->indices_size = indices_size;
	vertex_obj->attrib_order = attrib_order;
	vertex_obj->attrib_size = attrib_size;

	/*
		Create the new objects
	*/
	glGenBuffers(1, &vertex_obj->vbo);
	glGenBuffers(1, &vertex_obj->ebo);
	glGenVertexArrays(1, &vertex_obj->vao);

	/*
		Copy the vertices into the VBO
	*/
	glBindBuffer(GL_ARRAY_BUFFER, vertex_obj->vbo);
	glBufferData(GL_ARRAY_BUFFER, vertex_obj->vertices_size, vertex_obj->vertices, GL_STATIC_DRAW);

	/*
		Copy the indices into the EBO
	*/
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_obj->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertex_obj->indices_size, vertex_obj->indices, GL_STATIC_DRAW);

	/*
		Set the VAO stride and index
	*/
	glBindVertexArray(vertex_obj->vao);

	/*
		Set the VAO attributes with the given attribute order
	*/
	for (GLuint i = 0; i < ARRAY_COUNT(vertex_obj->attrib_order, vertex_obj->attrib_size); i++)
	{
		glVertexAttribPointer(
			i,
			vertex_obj->attrib_order[i],
			GL_FLOAT,
			GL_FALSE,
			next_vertex_offset * sizeof(float),
			(void*)(current_offset * sizeof(float)));
		glEnableVertexAttribArray(i);

		current_offset += vertex_obj->attrib_order[i];
	}

	return Q_SUCCESS;
}

Q_STATUS QMeshDelete(struct Q_MESHOBJECT* mesh)
{
	glDeleteBuffers(1, &mesh->vbo);
	glDeleteBuffers(1, &mesh->ebo);

	glDeleteVertexArrays(1, &mesh->vao);

	return Q_SUCCESS;
}

Q_STATUS QModelProcessMesh(struct Q_MODELOBJECT* model, const struct aiMesh* mesh, const struct aiScene* scene)
{
	struct Q_MESHOBJECT* n_mesh = &model->meshes[model->meshes_count];

	/*
		TODO: check for integer overflow
	*/
	n_mesh->vertices_count = 0;
	n_mesh->vertices_size = mesh->mNumVertices * sizeof(*n_mesh->vertices);
	n_mesh->vertices = malloc(n_mesh->vertices_size);
	if (!n_mesh->vertices)
		return Q_OUT_OF_MEMORY;
	
	n_mesh->indices_size = 0;
	n_mesh->indices_count = 0;
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		n_mesh->indices_size += (mesh->mFaces[i].mNumIndices * sizeof(*n_mesh->indices));
	}

	n_mesh->indices = malloc(n_mesh->indices_size);
	if (!n_mesh->indices)
	{
		free(n_mesh->vertices);
		n_mesh->vertices = NULL;

		return Q_OUT_OF_MEMORY;
	}

	n_mesh->attrib_size = 3 * sizeof(*n_mesh->attrib_order);
	n_mesh->attrib_order = malloc(n_mesh->attrib_size);
	if (!n_mesh->attrib_order)
	{
		free(n_mesh->vertices);
		n_mesh->vertices = NULL;

		free(n_mesh->indices);
		n_mesh->indices = NULL;

		return Q_OUT_OF_MEMORY;
	}
	n_mesh->attrib_order[0] = 3;
	n_mesh->attrib_order[1] = 3;
	n_mesh->attrib_order[2] = 2;

	for (n_mesh->vertices_count = 0; n_mesh->vertices_count < mesh->mNumVertices; n_mesh->vertices_count++)
	{
		/*
			Copy vertex position
		*/
		vec3 vec;

		vec[0] = mesh->mVertices[n_mesh->vertices_count].x;
		vec[1] = mesh->mVertices[n_mesh->vertices_count].y;
		vec[2] = mesh->mVertices[n_mesh->vertices_count].z;

		glm_vec3_copy(vec, n_mesh->vertices[n_mesh->vertices_count].pos);

		/*
			Copy vertex normals
		*/
		vec[0] = mesh->mNormals[n_mesh->vertices_count].x;
		vec[1] = mesh->mNormals[n_mesh->vertices_count].y;
		vec[2] = mesh->mNormals[n_mesh->vertices_count].z;

		glm_vec3_copy(vec, n_mesh->vertices[n_mesh->vertices_count].normal);

		/*
			Copy vertex texture coords
		*/
		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			vec2 vec_2;
			vec_2[0] = mesh->mTextureCoords[0][n_mesh->vertices_count].x;
			vec_2[1] = mesh->mTextureCoords[0][n_mesh->vertices_count].y;

			glm_vec2_copy(vec_2, n_mesh->vertices[n_mesh->vertices_count].tex_coords);
		}
		else
		{
			vec2 vec_2 = { 0.0, 0.0 };
			glm_vec2_copy(vec_2, n_mesh->vertices[n_mesh->vertices_count].tex_coords);
		}
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		struct aiFace *face = &mesh->mFaces[i];
		for (unsigned int j = 0; j < face->mNumIndices; j++)
			n_mesh->indices[n_mesh->indices_count++] = face->mIndices[j];
	}
	
	QMeshCreate(
		n_mesh, 
		n_mesh->vertices, n_mesh->vertices_size, 
		n_mesh->indices, n_mesh->indices_size, 
		n_mesh->attrib_order, n_mesh->attrib_size);

	++model->meshes_count;

	return Q_SUCCESS;
}

Q_STATUS QModelProcessNode(struct Q_MODELOBJECT* model, const struct aiNode* node, const struct aiScene* scene)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		struct aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

		if (QModelProcessMesh(model, mesh, scene) != Q_SUCCESS)
			QFatalError("my balls itch");
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		QModelProcessNode(model, node->mChildren[i], scene);
	}

	return Q_SUCCESS;
}

Q_STATUS QModelLoad(struct Q_MODELOBJECT* model, const char* path)
{	
	struct aiScene *scene = aiImportFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);
	if (!scene)
		return Q_IO_ERROR;

	model->meshes_count = 0;
	model->meshes_size = scene->mNumMeshes * sizeof(*model->meshes);

	glm_vec3_one(model->scale);
	glm_quat_identity(model->rotation);

	/*
		TODO: Check for integer overflow
	*/
	model->meshes = malloc(model->meshes_size);
	if (!model->meshes)
		return Q_OUT_OF_MEMORY;

	QModelProcessNode(model, scene->mRootNode, scene);

	aiReleaseImport(scene);

	return Q_SUCCESS;
}

Q_STATUS QModelFree(struct Q_MODELOBJECT* model)
{
	for (size_t i = 0; i < model->meshes_count; i++)
	{
		free(model->meshes[i].vertices);
		model->meshes[i].vertices = NULL;

		free(model->meshes[i].indices);
		model->meshes[i].indices = NULL;

		free(model->meshes[i].attrib_order);
		model->meshes[i].attrib_order = NULL;

		QMeshDelete(&model->meshes[i]);
	}

	free(model->meshes);
	model->meshes = NULL;

	return Q_SUCCESS;
}

Q_STATUS QModelScale(struct Q_MODELOBJECT* model, vec3 scale)
{
	glm_vec3_copy(scale, model->scale);

	return Q_SUCCESS;
}

Q_STATUS QModelRotate(struct Q_MODELOBJECT* model, versor rotation)
{
	glm_quat_copy(rotation, model->rotation);

	return Q_SUCCESS;
}