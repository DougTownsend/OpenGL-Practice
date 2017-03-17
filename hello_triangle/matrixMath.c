#include "matrixMath.h"
#include <math.h>

void identityMat4(float matrix[])
{
	for (int n=0; n < 16; n++)
	{
		if ((n == 0) || (n == 5) || (n == 10) || (n == 15)) matrix[n] = 1.0f;
		else matrix[n] = 0.0f;
	}
	return;
}

void copyMat4(float input[], float result[])
{
	for (int n = 0; n < 16; n++)
	{
		result[n] = input[n];
	}
	return;
}

void multiplyMat4byMat4(float matrixin1[], float matrixin2[], float result[])
{
	float matrix1[16];
	float matrix2[16];
	copyMat4(matrixin1, matrix1);
	copyMat4(matrixin2, matrix2);

	result[0] = matrix1[0] * matrix2[0] + matrix1[4] * matrix2[1] + matrix1[8] * matrix2[2] + matrix1[12] * matrix2[3];
	result[1] = matrix1[1] * matrix2[0] + matrix1[5] * matrix2[1] + matrix1[9] * matrix2[2] + matrix1[13] * matrix2[3];
	result[2] = matrix1[2] * matrix2[0] + matrix1[6] * matrix2[1] + matrix1[10] * matrix2[2] + matrix1[14] * matrix2[3];
	result[3] = matrix1[3] * matrix2[0] + matrix1[7] * matrix2[1] + matrix1[11] * matrix2[2] + matrix1[15] * matrix2[3];

	result[4] = matrix1[0] * matrix2[4] + matrix1[4] * matrix2[5] + matrix1[8] * matrix2[6] + matrix1[12] * matrix2[7];
	result[5] = matrix1[1] * matrix2[4] + matrix1[5] * matrix2[5] + matrix1[9] * matrix2[6] + matrix1[13] * matrix2[7];
	result[6] = matrix1[2] * matrix2[4] + matrix1[6] * matrix2[5] + matrix1[10] * matrix2[6] + matrix1[14] * matrix2[7];
	result[7] = matrix1[3] * matrix2[4] + matrix1[7] * matrix2[5] + matrix1[11] * matrix2[6] + matrix1[15] * matrix2[7];

	result[8] = matrix1[0] * matrix2[8] + matrix1[4] * matrix2[9] + matrix1[8] * matrix2[10] + matrix1[12] * matrix2[11];
	result[9] = matrix1[1] * matrix2[8] + matrix1[5] * matrix2[9] + matrix1[9] * matrix2[10] + matrix1[13] * matrix2[11];
	result[10] = matrix1[2] * matrix2[8] + matrix1[6] * matrix2[9] + matrix1[10] * matrix2[10] + matrix1[14] * matrix2[11];
	result[11] = matrix1[3] * matrix2[8] + matrix1[7] * matrix2[9] + matrix1[11] * matrix2[10] + matrix1[15] * matrix2[11];

	result[12] = matrix1[0] * matrix2[12] + matrix1[4] * matrix2[13] + matrix1[8] * matrix2[14] + matrix1[12] * matrix2[15];
	result[13] = matrix1[1] * matrix2[12] + matrix1[5] * matrix2[13] + matrix1[9] * matrix2[14] + matrix1[13] * matrix2[15];
	result[14] = matrix1[2] * matrix2[12] + matrix1[6] * matrix2[13] + matrix1[10] * matrix2[14] + matrix1[14] * matrix2[15];
	result[15] = matrix1[3] * matrix2[12] + matrix1[7] * matrix2[13] + matrix1[11] * matrix2[14] + matrix1[15] * matrix2[15];

	return;
}

void translationMat4(float x, float y, float z, float matrix[], float result[])
{
	float transformMatrix[16];
	identityMat4(transformMatrix);
	transformMatrix[12] = x;
	transformMatrix[13] = y;
	transformMatrix[14] = z;
	multiplyMat4byMat4(matrix, transformMatrix, result);
	return;
}

void scalingMat4(float x, float y, float z, float matrix[], float result[])
{
	float transformMatrix[16];
	identityMat4(transformMatrix);
	transformMatrix[0] = x;
	transformMatrix[5] = y;
	transformMatrix[10] = z;
	multiplyMat4byMat4(matrix, transformMatrix, result);
	return;
}

void rotateXMat4(float degrees, float matrix[], float result[])
{
	float transformMatrix[16];
	identityMat4(transformMatrix);
	float radians = degrees * PI_OVER_180;
	transformMatrix[5] = cosf(radians);
	transformMatrix[6] = sinf(radians);
	transformMatrix[9] = -sinf(radians);
	transformMatrix[10] = cosf(radians);
	multiplyMat4byMat4(matrix, transformMatrix, result);
	return;
}

void rotateYMat4(float degrees, float matrix[], float result[])
{
	float transformMatrix[16];
	identityMat4(transformMatrix);
	float radians = degrees * PI_OVER_180;
	transformMatrix[0] = cosf(radians);
	transformMatrix[8] = sinf(radians);
	transformMatrix[2] = -sinf(radians);
	transformMatrix[10] = cosf(radians);
	multiplyMat4byMat4(matrix, transformMatrix, result);
	return;
}
void rotateZMat4(float degrees, float matrix[], float result[])
{
	float transformMatrix[16];
	identityMat4(transformMatrix);
	float radians = degrees * PI_OVER_180;
	transformMatrix[0] = cosf(radians);
	transformMatrix[1] = sinf(radians);
	transformMatrix[4] = -sinf(radians);
	transformMatrix[5] = cosf(radians);
	multiplyMat4byMat4(matrix, transformMatrix, result);
	return;
}