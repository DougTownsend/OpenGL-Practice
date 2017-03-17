#pragma once
#define _CRT_SECURE_NO_DEPRECATE
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

GLuint loadStallVAO(GLuint* index_size, GLuint* index);
GLuint loadPersonVAO(GLuint* index_size, GLuint* index);