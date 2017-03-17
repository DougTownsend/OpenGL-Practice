#pragma once
#include <math.h>

void identityMat4(float matrix[]);
void multiplyMat4byMat4(float matrix1[], float matrix2[], float result[]);
void copyMat4(float input[], float result[]);
void translationMat4(float x, float y, float z, float matrix[], float result[]);
void scalingMat4(float x, float y, float z, float matrix[], float result[]);
void rotateXMat4(float degrees, float matrix[], float result[]);
void rotateYMat4(float degrees, float matrix[], float result[]);
void rotateZMat4(float degrees, float matrix[], float result[]);

#define PI_OVER_180 (0.0174532293f)