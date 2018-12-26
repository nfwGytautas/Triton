#include "TRpch.h"
#include "Math.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm\gtx\transform.hpp>
#include <glm\gtc\matrix_transform.hpp>

Triton::Matrix44 Triton::Core::CreateProjectionMatrix(float aWindowWidth, float aWindowHeight, float aFoV, float aFarRenderPlane, float aNearRenderPlane)
{
	//Matrix44 returnMatrix = glm::mat4(1);

	//float aspectRatio = aWindowWidth / aWindowHeight;
	//float y_scale = (float)((1.0f / glm::tan(glm::radians(aFoV / 2.0f))) * aspectRatio);
	//float x_scale = y_scale / aspectRatio;
	//float frustum_length = aFarRenderPlane - aNearRenderPlane;
	//
	//returnMatrix[0] = glm::vec4(x_scale, 0, 0, 0);
	//returnMatrix[1] = glm::vec4(0, y_scale, 0, 0);
	//returnMatrix[2] = glm::vec4(0, 0, -((aFarRenderPlane + aNearRenderPlane) / frustum_length), -1);
	//returnMatrix[3] = glm::vec4(0, 0, -((2 * aFarRenderPlane * aNearRenderPlane) / frustum_length), 0);

	Matrix44 returnMatrix = glm::perspective(aFoV, aWindowWidth / aWindowHeight, aNearRenderPlane, aFarRenderPlane);

	return returnMatrix;
}

Triton::Matrix44 Triton::Core::CreateTransformationMatrix(Vector3 translation, Vector3 rotation, Vector3 scale)
{
	//Matrix44 translationMatrix = glm::translate(glm::mat4(1.0f), translation);
	//Matrix44 scaleMatrix = glm::scale(glm::vec3(scale.x, scale.y, scale.z));
	//Matrix44 Rotation = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1, 0, 0));
	//Rotation = glm::rotate(Rotation, glm::radians(rotation.y), glm::vec3(0, 1, 0));
	//Rotation = glm::rotate(Rotation, glm::radians(rotation.z), glm::vec3(0, 0, 1));
	//
	//Matrix44 returnMatrix = translationMatrix * Rotation * scaleMatrix;
	//
	//return returnMatrix;

	Matrix44 translationMatrix = glm::translate(glm::mat4(1.0f), translation);
	translationMatrix = glm::scale(translationMatrix, glm::vec3(scale.x, scale.y, scale.z));
	translationMatrix = glm::rotate(translationMatrix, glm::radians(rotation.x), glm::vec3(1, 0, 0));
	translationMatrix = glm::rotate(translationMatrix, glm::radians(rotation.y), glm::vec3(0, 1, 0));
	translationMatrix = glm::rotate(translationMatrix, glm::radians(rotation.z), glm::vec3(0, 0, 1));
	
	return translationMatrix;
}
