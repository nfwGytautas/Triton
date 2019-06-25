#include "mathematical.h"

#ifdef TR_PLATFORM_WINDOWS

Triton::Matrix44 Triton::Core::CreateProjectionMatrix(float aWindowWidth, float aWindowHeight, float aFoV, float aNearRenderPlane, float aFarRenderPlane)
{
	Matrix44 returnMatrix;

	float screenAspect = (float)aWindowWidth / (float)aWindowHeight;
	returnMatrix = DirectX::XMMatrixPerspectiveFovLH(aFoV, screenAspect, aNearRenderPlane, aFarRenderPlane);

	return returnMatrix;
}

Triton::Matrix44 Triton::Core::CreateTransformationMatrix(Vector3 translation, Vector3 rotation, Vector3 scale)
{
	Matrix44 translationMatrix = Matrix44();

	DirectX::XMMATRIX matRotateX = DirectX::XMMatrixRotationX(Triton::to_radians(rotation.x));
	DirectX::XMMATRIX matRotateY = DirectX::XMMatrixRotationY(Triton::to_radians(rotation.y));
	DirectX::XMMATRIX matRotateZ = DirectX::XMMatrixRotationZ(Triton::to_radians(rotation.z));
	DirectX::XMMATRIX matRotate = matRotateX * matRotateY*matRotateZ;

	translationMatrix = translationMatrix * matRotate;

	translationMatrix = translationMatrix * DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

	translationMatrix = translationMatrix * DirectX::XMMatrixTranslation(translation.x, translation.y, translation.z);


	return translationMatrix;
}

Triton::Matrix44 Triton::Core::createViewMatrix(Vector3 position, float pitch, float yaw, float roll)
{
	using namespace DirectX;

	XMFLOAT3 up, lookAt;
	XMVECTOR upVector, positionVector, lookAtVector;
	XMMATRIX rotationMatrix;


	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Load it into a XMVECTOR structure.
	upVector = XMLoadFloat3(&up);

	// Load it into a XMVECTOR structure.
	positionVector = XMLoadFloat3(&position);

	// Setup where the camera is looking by default.
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// Load it into a XMVECTOR structure.
	lookAtVector = XMLoadFloat3(&lookAt);

	// Create the rotation matrix from the yaw, pitch, and roll values.
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	// Finally create the view matrix from the three updated vectors.
	return XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
}

Triton::Matrix44 Triton::Core::CreateOrthographicMatrix(float aWindowWidth, float aWindowHeight, float aNearRenderPlane, float aFarRenderPlane)
{
	Matrix44 returnMatrix;

	returnMatrix = DirectX::XMMatrixOrthographicLH(aWindowWidth, aWindowHeight, aNearRenderPlane, aFarRenderPlane);

	return returnMatrix;
}

#else

#define GLM_ENABLE_EXPERIMENTAL
#include <glm\gtx\transform.hpp>
#include <glm\gtc\matrix_transform.hpp>

Triton::Matrix44 Triton::Core::CreateProjectionMatrix(float aWindowWidth, float aWindowHeight, float aFoV, float aNearRenderPlane, float aFarRenderPlane)
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

	Matrix44 returnMatrix = glm::perspective(aFoV, (float)aWindowWidth / (float)aWindowHeight, aNearRenderPlane, aFarRenderPlane);

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

#endif