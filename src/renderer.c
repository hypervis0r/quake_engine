#include "renderer.h"

Q_STATUS QRenderDrawMeshObject(struct Q_MESHOBJECT* vertex_obj)
{
	glDrawElements(GL_TRIANGLES, ARRAY_COUNT(vertex_obj->indices, vertex_obj->indices_size), GL_UNSIGNED_INT, 0);

	return Q_SUCCESS;
}

Q_STATUS QRenderBindMeshObject(struct Q_MESHOBJECT* vertex_obj, struct Q_MATERIAL* mat)
{
	if (!vertex_obj || !mat)
		return Q_ERROR;

	glUseProgram(mat->shader_program_id);

	glBindVertexArray(vertex_obj->vao);

	glBindBuffer(GL_ARRAY_BUFFER, vertex_obj->vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_obj->ebo);

	if (mat->bind_textures)
		mat->bind_textures(mat);

	if (mat->apply_shader_uniforms)
		mat->apply_shader_uniforms(mat);

	for (GLuint i = 0; i < ARRAY_COUNT(vertex_obj->attrib_order, vertex_obj->attrib_size); i++)
		glEnableVertexAttribArray(i);

	return Q_SUCCESS;
}

Q_STATUS QRenderModelObject(
	struct Q_MODELOBJECT* model, 
	struct Q_CAMERAOBJECT* cam, 
	struct Q_MATERIAL* mat,
	vec3 world_pos, 
	vec3 scale, 
	vec3 rot_axis, float rot_angle)
{
	for (size_t i = 0; i < model->meshes_count; i++)
	{
		QRenderMeshObject(&model->meshes[i], cam, mat, world_pos, scale, rot_axis, rot_angle);
	}

	return Q_SUCCESS;
}

Q_STATUS QRenderInitializeCameraObject(
	struct Q_CAMERAOBJECT* cam, 
	vec3 pos, vec3 front, vec3 up,
	float fov)
{
	if (!cam)
		return Q_ERROR;

	glm_vec3_copy(pos, cam->pos);
	glm_vec3_copy(front, cam->front);
	glm_vec3_copy(up, cam->up);

	/*
		yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right 
		so we initially rotate a bit to the left.
	*/
	cam->yaw = -90.0f;
	cam->pitch = 0.f;
	cam->roll = 0.f;

	cam->fov = fov;

	return Q_SUCCESS;
}

Q_STATUS QRenderInitializeFrameContext(struct Q_FRAMECONTEXT* frame_ctx)
{
	if (!frame_ctx)
		return Q_ERROR;

	frame_ctx->delta_time = 0.;
	frame_ctx->last_frame_time = 0.;

	return Q_SUCCESS;
}

Q_STATUS QRenderUpdateFrameContext(struct Q_FRAMECONTEXT* frame_ctx)
{
	float currentFrame = glfwGetTime();
	frame_ctx->delta_time = currentFrame - frame_ctx->last_frame_time;
	frame_ctx->last_frame_time = currentFrame;

	return Q_SUCCESS;
}

Q_STATUS QRenderCalculateViewMatrix(struct Q_CAMERAOBJECT *cam, mat4 view)
{
	vec3 cam_target;
	
	glm_mat4_identity(view);

	glm_vec3_add(cam->pos, cam->front, cam_target);
	glm_lookat(cam->pos, cam_target, cam->up, view);

	return Q_SUCCESS;
}

Q_STATUS QRenderCalculateProjectionMatrix(struct Q_CAMERAOBJECT* cam, mat4 projection)
{
	GLint data[4] = { 0 };
	
	glGetIntegerv(GL_VIEWPORT, data);

	glm_mat4_identity(projection);
	glm_perspective(glm_rad(cam->fov), (float)data[2] / (float)data[3], 0.1f, 100.0f, projection);

	return Q_SUCCESS;
}

Q_STATUS QRenderClearScreen(vec4 color)
{
	glClearColor(color[0], color[1], color[2], color[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return Q_SUCCESS;
}

Q_STATUS QRenderMeshObject(
	struct Q_MESHOBJECT *mesh, 
	struct Q_CAMERAOBJECT *cam, 
	struct Q_MATERIAL *mat,
	vec3 world_pos,
	vec3 scale,
	vec3 rotation_axis, float angle)
{
	mat4 projection, view, model, normal_matrix;

	QRenderCalculateViewMatrix(cam, view);
	QRenderCalculateProjectionMatrix(cam, projection);

	glm_mat4_identity(model);
	glm_translate(model, world_pos);

	if (scale)
		glm_scale(model, scale);
	
	if (rotation_axis)
		glm_rotate(model, glm_rad(angle), rotation_axis);

	glm_mat4_inv(model, normal_matrix);
	glm_mat4_transpose(normal_matrix);

	QRenderBindMeshObject(mesh, mat);

	QShaderSetUniformMat4(mat->shader_program_id, "view", view);
	QShaderSetUniformMat4(mat->shader_program_id, "projection", projection);
	QShaderSetUniformMat4(mat->shader_program_id, "model", model);
	QShaderSetUniformMat4(mat->shader_program_id, "normal_matrix", normal_matrix);

	QRenderDrawMeshObject(mesh);

	return Q_SUCCESS;
}

Q_STATUS QRenderLoop(GLFWwindow* window)
{
	if (!window)
		return Q_ERROR;

	struct Q_CAMERAOBJECT cam_obj = { 0 };
	struct Q_FRAMECONTEXT frame_ctx = { 0 };

	struct Q_TEXTUREMAP map = { 0 };
	QTextureMapCreate(&map, 
		"resources\\textures\\crate1.png", "resources\\textures\\crate1_specular.png", "resources\\textures\\matrix.jpg",
		NULL, NULL, NULL);

	struct Q_MATERIAL mat = { 0 };
	QShaderPhongCreate(&mat, &map, 100.0f);

	struct Q_MATERIAL light_mat = { 0 };
	vec3 light_cube_albedo = { 0., 1., 0. };
	QShaderFillCreate(&light_mat, light_cube_albedo);

	struct Q_MODELOBJECT main_model = { 0 };
	QModelLoad(&main_model, "resources\\models\\cube.obj");

	struct Q_MODELOBJECT light_model = { 0 };
	QModelLoad(&light_model, "resources\\models\\cube.obj");

	struct Q_PLAYEROBJECT player = { 0 };
	QPlayerCreate(&player, 3., 1., 0.30);
	
	QRenderInitializeFrameContext(&frame_ctx);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	/*
		Main render loop
	*/
	while (!glfwWindowShouldClose(window))
	{
		QInputProcess(window, &player, &frame_ctx);

		QPlayerUpdateVelocity(&player, &frame_ctx);

		vec4 background_color = { 0.1, 0.1, 0.1, 1.0 };
		QRenderClearScreen(background_color);

		float xpos = 3.0f * sin(glfwGetTime());
		float zpos = 3.5f * cos(glfwGetTime());
		float ypos = 3.0f * sin(glfwGetTime());

		vec3 main_pos = { 0., 0., 0. };
		vec3 light_pos = { xpos, ypos, zpos };

		vec3 rotation = { 1.0f, 0.3f, 0.5f };
		vec3 scale = { .1, .1, .1 };
		float angle = 20.0f;

		vec3 light_ambient = { 0.2f, 0.2f, 0.2f };
		struct Q_LIGHTOBJECT light_obj = { 0 };
		QShaderCreateLight(&light_obj, light_ambient, light_cube_albedo, light_cube_albedo, light_pos);

		mat.set_shader_light(&mat, &light_obj, player.cam);

		QRenderModelObject(&main_model, player.cam, &mat, main_pos, NULL, rotation, angle * (glfwGetTime() * 0.5));

		QRenderModelObject(&light_model, player.cam, &light_mat, light_pos, scale, NULL, 0);

		QRenderUpdateFrameContext(&frame_ctx);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return Q_SUCCESS;
}