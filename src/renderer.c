#include "renderer.h"

Q_STATUS QRenderDrawMeshObject(struct Q_MESHOBJECT* vertex_obj)
{
	glDrawElements(GL_TRIANGLES, ARRAY_COUNT(vertex_obj->indices, vertex_obj->indices_size), GL_UNSIGNED_INT, 0);

	return Q_SUCCESS;
}

Q_STATUS QRenderBindMeshObject(struct Q_MESHOBJECT* vertex_obj, GLuint shader_program_id, GLuint texture_id)
{
	if (!vertex_obj)
		return Q_ERROR;

	glUseProgram(shader_program_id);

	glBindVertexArray(vertex_obj->vao);

	glBindBuffer(GL_ARRAY_BUFFER, vertex_obj->vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_obj->ebo);

	glBindTexture(GL_TEXTURE_2D, texture_id);

	for (GLuint i = 0; i < ARRAY_COUNT(vertex_obj->attrib_order, vertex_obj->attrib_size); i++)
		glEnableVertexAttribArray(i);

	return Q_SUCCESS;
}

Q_STATUS QRenderModelObject(
	struct Q_MODELOBJECT* model, 
	struct Q_CAMERAOBJECT* cam, 
	GLuint shader_program_id,
	GLuint texture_id,
	vec3 world_pos, 
	vec3 scale, 
	vec3 rot_axis, float rot_angle)
{
	for (size_t i = 0; i < model->meshes_count; i++)
	{
		QRenderMeshObject(&model->meshes[i], cam, shader_program_id, texture_id, world_pos, scale, rot_axis, rot_angle);
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
	GLuint shader_program_id, 
	GLuint texture_id,
	vec3 world_pos,
	vec3 scale,
	vec3 rotation_axis, float angle)
{
	mat4 projection, view, model;

	QRenderCalculateViewMatrix(cam, view);
	QRenderCalculateProjectionMatrix(cam, projection);

	glm_mat4_identity(model);
	glm_translate(model, world_pos);

	if (scale)
		glm_scale(model, scale);
	
	if (rotation_axis)
		glm_rotate(model, glm_rad(angle), rotation_axis);

	QRenderBindMeshObject(mesh, shader_program_id, texture_id);

	QShaderSetUniformMat4(shader_program_id, "view", view);
	QShaderSetUniformMat4(shader_program_id, "projection", projection);
	QShaderSetUniformMat4(shader_program_id, "model", model);

	QRenderDrawMeshObject(mesh);
}

Q_STATUS QRenderLoop(GLFWwindow* window)
{
	if (!window)
		return Q_ERROR;

	struct Q_CAMERAOBJECT cam_obj = { 0 };
	struct Q_FRAMECONTEXT frame_ctx = { 0 };

	GLuint phong_shader_program = 0;
	GLuint light_shader_program = 0;

	GLuint vertex_shader = 0;
	GLuint light_shader = 0;
	GLuint phong_shader = 0;

	QShaderCompile(&vertex_shader, GL_VERTEX_SHADER, "resources\\shaders\\vertex.vert.glsl");
	QShaderCompile(&phong_shader, GL_FRAGMENT_SHADER, "resources\\shaders\\phong.frag.glsl");
	QShaderCompile(&light_shader, GL_FRAGMENT_SHADER, "resources\\shaders\\light.frag.glsl");

	GLuint phong_shader_c[] = { vertex_shader, phong_shader };
	GLuint light_shader_c[] = { vertex_shader, light_shader };

	QShaderLink(&phong_shader_program, phong_shader_c, ARRAY_COUNT(phong_shader_c, sizeof(phong_shader_c)));
	QShaderLink(&light_shader_program, light_shader_c, ARRAY_COUNT(light_shader_c, sizeof(light_shader_c)));

	glDeleteShader(vertex_shader);
	glDeleteShader(light_shader);
	glDeleteShader(phong_shader);

	GLuint texture_id = 0;
	QTextureCreate(&texture_id, "C:\\Users\\atom0\\Pictures\\1635065320374.jpg");

	struct Q_MODELOBJECT main_model = { 0 };
	QModelLoad(&main_model, "resources\\models\\monkey.obj");

	struct Q_MODELOBJECT light_model = { 0 };
	QModelLoad(&light_model, "resources\\models\\cube.obj");

	vec3 cameraPos = { 0.0f, 0.0f,  3.0f };
	vec3 cameraFront = { 0.0f, 0.0f, -1.0f };
	vec3 cameraUp = { 0.0f, 1.0f, 0.0f };
	QRenderInitializeCameraObject(&cam_obj, cameraPos, cameraFront, cameraUp, 90.f);
	
	QRenderInitializeFrameContext(&frame_ctx);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	while (!glfwWindowShouldClose(window))
	{
		QInputProcess(window, &cam_obj, &frame_ctx);

		vec4 background_color = { 0.0, 0.0, 0.0, 1.0 };
		QRenderClearScreen(background_color);

		vec3 world_pos = { 0., 0., 0. };
		vec3 rotation = { 1.0f, 0.3f, 0.5f };
		vec3 scale = { .1, .1, .1 };
		float angle = 20.0f;

		vec3 objectColor = { 1.0f, 0.5f, 0.31f };
		vec3 lightColor = { 1.0f, 1.0f, 1.0f };
		QShaderSetUniformVec3(phong_shader_program, "objectColor", objectColor);
		QShaderSetUniformVec3(phong_shader_program, "lightColor", lightColor);

		QRenderModelObject(&main_model, &cam_obj, phong_shader_program, texture_id, world_pos, NULL, NULL, 0);

		world_pos[0] = 1.;
		world_pos[1] = 1.;
		world_pos[2] = 1.;

		QRenderModelObject(&light_model, &cam_obj, light_shader_program, texture_id, world_pos, scale, NULL, 0);

		QRenderUpdateFrameContext(&frame_ctx);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return Q_SUCCESS;
}