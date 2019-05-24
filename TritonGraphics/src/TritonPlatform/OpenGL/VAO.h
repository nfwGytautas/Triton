#pragma once

#include <GLFW\glfw3.h>
#include <glad\glad.h>

#include "Triton\Logger\Log.h"

#include "Macros.h"
#include "Types.h"

#define NAMESPACE_BEGIN namespace Triton { namespace PType {
#define NAMESPACE_END } }

NAMESPACE_BEGIN


unsigned int CreateVAO()
{
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	return vao;
}
unsigned int CreateIBO(std::vector<unsigned int> aIndices)
{
	unsigned int ebo;
	glGenBuffers(1, &ebo);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, aIndices.size() * sizeof(unsigned int),
		&aIndices[0], GL_STATIC_DRAW);

	return ebo;
}

template<class T>
unsigned int StoreDataInVBO(std::vector<T> aData)
{
	unsigned int vbo;
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, aData.size() * sizeof(T), &aData[0], GL_STATIC_DRAW);

	return vbo;
}
unsigned int storeDataInAttributes(unsigned int location, unsigned int dataSize, std::vector<float> data)
{
	unsigned int vboID;

	glGenBuffers(1, &vboID);

	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);

	glVertexAttribPointer(
		location,
		dataSize,
		GL_FLOAT,
		false,
		dataSize * sizeof(float),
		(const void*)0
	);

	glEnableVertexAttribArray(location);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return vboID;
}

template <class T>
void VAOAttribPointer(unsigned int location, unsigned int dataSize, const void* pointer)
{
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, dataSize, GL_FLOAT, GL_FALSE, sizeof(T), pointer);
}


void Triton::PType::GLVAO::enable()
{
	glBindVertexArray(m_vaoID);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
}

inline void Triton::PType::GLVAO::disable()
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glBindVertexArray(0);
}

inline void Triton::PType::GLVAO::create(FactoryCreateParams* createParams)
{
	auto glParams = OBJECT_AS(VAOCreateParams, createParams);

	m_vaoID = CreateVAO();

	glBindVertexArray(m_vaoID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	m_iboID = CreateIBO(glParams->indices);

	m_vbos.push_back(StoreDataInVBO<VAOCreateParams::Vertex>(glParams->vertices));
	VAOAttribPointer<VAOCreateParams::Vertex>(0, 3, (void*)0);
	VAOAttribPointer<VAOCreateParams::Vertex>(1, 2, (void*)offsetof(VAOCreateParams::Vertex, UV));
	VAOAttribPointer<VAOCreateParams::Vertex>(2, 3, (void*)offsetof(VAOCreateParams::Vertex, Normal));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	m_indiceCount = glParams->indices.size();

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glBindVertexArray(0);
}

inline void Triton::PType::GLVAO::destroy(FactoryDestroyParams * destroyParams)
{
	glDeleteVertexArrays(1, &m_vaoID);
	glDeleteBuffers(0, &m_iboID);
	glDeleteBuffers(m_vbos.size(), &m_vbos[0]);
}


NAMESPACE_END