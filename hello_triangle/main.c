
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "shaderLoader.h"
#include "matrixMath.h"
#include "stb_image.h"
#include "textures.h"
#include "modelLoader.h"

int main()
{
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	GLFWmonitor* mon = glfwGetPrimaryMonitor();
	const GLFWvidmode* vmode = glfwGetVideoMode(mon);
	GLFWwindow* window = glfwCreateWindow(
		vmode->width, vmode->height, "Extended GL Init", mon, NULL
	);

	if (!window)
	{
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();


	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	float model_matrix[] =
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	float transformForward[16];
	translationMat4(0, 0, -5, transformForward, transformForward);


	GLuint vs = shaderLoader("vertex_shader.txt", GL_VERTEX_SHADER);
	GLuint fs = shaderLoader("fragment_shader.txt", GL_FRAGMENT_SHADER);
	GLuint shader_program = glCreateProgram();
	glAttachShader(shader_program, fs);
	glAttachShader(shader_program, vs);
	glLinkProgram(shader_program);

	int model_matrix_location = glGetUniformLocation(shader_program, "model");
	glUseProgram(shader_program);
	glUniformMatrix4fv(model_matrix_location, 1, GL_FALSE, model_matrix);

	glClearColor(.1f, .1f, .1f, 1.0f);

	float speed = 5.0f;

	//Camera Variables
	float cam_speed = 3.0f;
	float cam_yaw_speed = 100.0f;
	float cam_pos[3] = { 0.0f, 2.0f, 2.0f };
	float cam_yaw = 0.0f;

	//Initialize view matrix
	float cam_translate[16];
	identityMat4(cam_translate);
	translationMat4(-cam_pos[0], -cam_pos[1], -cam_pos[2], cam_translate, cam_translate);
	float cam_rotate[16];
	identityMat4(cam_rotate);
	rotateYMat4(-cam_yaw, cam_rotate, cam_rotate);
	float view_matrix[16];
	multiplyMat4byMat4(cam_rotate, cam_translate, view_matrix);

	//Projection
	float near = 0.3f;
	float far = 100.0f;
	float fov = 67.0f * PI_OVER_180;
	float aspect = (float)vmode->width / (float)vmode->height;
	float range = tanf(fov*0.5f) * near;
	float Sx = (2.0f * near) / (range * aspect * 2);
	float Sy = near / range;
	float Sz = -(far + near) / (far - near);
	float Pz = -(2.0f * far * near) / (far - near);
	float projection_matrix[16] = 
	{
		Sx, 0.0f, 0.0f, 0.0f,
		0.0f, Sy, 0.0f, 0.0f,
		0.0f, 0.0f, Sz, -1.0f,
		0.0f, 0.0f, Pz, 0.0f
	};

	//Camera uniforms
	int view_matrix_location = glGetUniformLocation(shader_program, "view");
	glUseProgram(shader_program);
	glUniformMatrix4fv(view_matrix_location, 1, GL_FALSE, view_matrix);
	int projection_matrix_location = glGetUniformLocation(shader_program, "proj");
	glUseProgram(shader_program);
	glUniformMatrix4fv(projection_matrix_location, 1, GL_FALSE, projection_matrix);

	float last_position = 0.0f;
	float next_position;
	double previous_seconds = glfwGetTime();
	double current_seconds;
	double elapsed_seconds;
	float transformMatrix[16];
	
	int cam_moved = 0;

	GLuint tex = 0;
	load_texture("stalltexture.png", &tex, 0);
	GLuint person_tex = 0;
	load_texture("doorguy.png", &person_tex, 0);

	GLuint vao_index_size;
	GLuint vao = 0;
	GLuint index_vbo = 0;
	vao = loadStallVAO(&vao_index_size, &index_vbo);

	GLuint person_vao = 0;
	GLuint person_vao_size = 0;
	GLuint person_index_vbo = 0;
	person_vao = loadPersonVAO(&person_vao_size, &person_index_vbo);


	printf("%d", glGetError());

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	while (!glfwWindowShouldClose(window))
	{
		current_seconds = glfwGetTime();
		elapsed_seconds = current_seconds - previous_seconds;
		previous_seconds = current_seconds;

		if (glfwGetKey(window, GLFW_KEY_A))
		{
			cam_pos[0] -= (cam_speed * (float)elapsed_seconds) * cosf(cam_yaw * PI_OVER_180);
			cam_pos[2] += (cam_speed * (float)elapsed_seconds) * sinf(cam_yaw * PI_OVER_180);
			cam_moved = 1;
		}
		if (glfwGetKey(window, GLFW_KEY_D))
		{
			cam_pos[0] += (cam_speed * (float)elapsed_seconds) * cosf(cam_yaw * PI_OVER_180);
			cam_pos[2] -= (cam_speed * (float)elapsed_seconds) * sinf(cam_yaw * PI_OVER_180);
			cam_moved = 1;
		}
		if (glfwGetKey(window, GLFW_KEY_W))
		{
			cam_pos[2] -= (cam_speed * (float)elapsed_seconds) * cosf(PI_OVER_180 * cam_yaw);
			cam_pos[0] -= (cam_speed * (float)elapsed_seconds) * sinf(PI_OVER_180 * cam_yaw);
			cam_moved = 1;
		}
		if (glfwGetKey(window, GLFW_KEY_S))
		{
			cam_pos[2] += (cam_speed * (float)elapsed_seconds) * cosf(PI_OVER_180 * cam_yaw);
			cam_pos[0] += (cam_speed * (float)elapsed_seconds) * sinf(PI_OVER_180 * cam_yaw);
			cam_moved = 1;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT))
		{
			cam_yaw += cam_yaw_speed * (float)elapsed_seconds;
			cam_moved = 1;
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT))
		{
			cam_yaw -= cam_yaw_speed * (float)elapsed_seconds;
			cam_moved = 1;
		}
		if (cam_moved)
		{
			identityMat4(cam_translate);
			translationMat4(-cam_pos[0], -cam_pos[1], -cam_pos[2], cam_translate, cam_translate);
			identityMat4(cam_rotate);
			rotateYMat4(-cam_yaw, cam_rotate, cam_rotate);
			multiplyMat4byMat4(cam_rotate, cam_translate, view_matrix);
			glUniformMatrix4fv(view_matrix_location, 1, GL_FALSE, view_matrix);
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shader_program);


		glBindTexture(GL_TEXTURE_2D, tex);
		glBindVertexArray(vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_vbo);
		//glDrawElements(
		//	GL_TRIANGLES, vao_index_size, GL_UNSIGNED_INT, NULL
		//);

		glBindTexture(GL_TEXTURE_2D, person_tex);
		glBindVertexArray(person_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, person_index_vbo);
		glDrawElements(
			GL_TRIANGLES, person_vao_size, GL_UNSIGNED_INT, NULL
		);

		glfwPollEvents();
		
		glfwSwapBuffers(window);

		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(window, 1);
		}
	}

	glfwTerminate();
	return 0;
}