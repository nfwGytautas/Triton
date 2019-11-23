#pragma once

#include "TritonPlatform2/mathematical.h"
#include "TritonPlatform2/CrossTypes/Base.h"
#include "TritonTypes/IO.h"

namespace Triton
{
	namespace Graphics
	{
		class VAO : public Renderable
		{
		public:
			virtual ~VAO() { }

			virtual void enable() = 0;
			virtual void disable() = 0;

			virtual unsigned int getIndiceCount() = 0;

			/**
			 * Updates vertex array vertices buffer
			 * The VAO must have been created with DynamicBuffer level 1 or 2
			 * 
			 * @param vertices Vector of new vertices
			 */
			virtual void updateVertices(std::vector<float>& vertices) = 0;

			/**
			 * Updates vertex array indices buffer
			 * The VAO must have been created with DynamicBuffer level 2
			 *
			 * @param indices Vector of new indices
			 */
			virtual void updateIndices(std::vector<unsigned int>& indices) = 0;
		protected:
			VAO()
			{ }

		protected:
			unsigned int m_stride;
		};

	}
}