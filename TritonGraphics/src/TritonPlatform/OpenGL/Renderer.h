#pragma once

#include <GLFW\glfw3.h>
#include <glad\glad.h>

#include "Triton\Logger\Log.h"

#include "Macros.h"
#include "Types.h"

#define NAMESPACE_BEGIN namespace Triton { namespace PType {
#define NAMESPACE_END } }

NAMESPACE_BEGIN


inline void GLRenderer::create()
{
	
}

inline void Triton::PType::GLRenderer::render(PlatformObject* renderable)
{
	auto glRenderable = OBJECT_AS(GLVAO, renderable);

	if (glRenderable != nullptr)
	{
		glDrawElements(GL_TRIANGLES, glRenderable->m_indiceCount, GL_UNSIGNED_INT, (void*)0);
	}
	else
	{
		TR_CORE_ERROR("Passed PlatformObject is not of type GLRenderable.");
	}
}

inline void Triton::PType::GLRenderer::default()
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}


NAMESPACE_END