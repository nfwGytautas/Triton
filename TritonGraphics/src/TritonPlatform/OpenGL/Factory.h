#pragma once

#include <GLFW\glfw3.h>
#include <glad\glad.h>

#include "Triton\Logger\Log.h"

#include "Macros.h"
#include "Types.h"

#define NAMESPACE_BEGIN namespace Triton { namespace PType {
#define NAMESPACE_END } }

NAMESPACE_BEGIN


inline FactoryObject* Triton::PType::GLFactory::createShader(FactoryCreateParams* createParams)
{
	GLShader* shader = new GLShader();
	shader->create(createParams);
	return shader;
}

inline FactoryObject* Triton::PType::GLFactory::createVAO(FactoryCreateParams* createParams)
{
	GLVAO* vao = new GLVAO();
	vao->create(createParams);
	return vao;
}

inline FactoryObject* Triton::PType::GLFactory::createTexture(FactoryCreateParams* createParams)
{
	GLTexture* texture = new GLTexture();
	texture->create(createParams);
	return texture;
}

inline void Triton::PType::GLFactory::destroyObject(FactoryObject* object, FactoryDestroyParams* destroyParams)
{
	object->destroy(destroyParams);
	delete object;
}


NAMESPACE_END