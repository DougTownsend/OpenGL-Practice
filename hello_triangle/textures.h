#pragma once
#include "stb_image.h"
#include "textures.h"
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>


int load_texture(const char* file_name, GLuint* tex, int texture_unit);