#include <stdio.h>
#include <stdlib.h>
#include "shaderLoader.h"
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>


GLuint shaderLoader(char* filename, unsigned int type)
{
	long int size;
	char* shader = NULL;
	FILE* fp = fopen(filename, "r");
	fseek(fp, 0L, SEEK_END);
	size = ftell(fp) +1;
	rewind(fp);
	shader = (char*)calloc(sizeof(char) * size, sizeof(char));
	fread(shader, sizeof(char), size, fp);
	fclose(fp);

	GLuint vs = glCreateShader(type);
	glShaderSource(vs, 1, (const char* const*)&shader, NULL);
	glCompileShader(vs);
	free(shader);

	return vs;
}

