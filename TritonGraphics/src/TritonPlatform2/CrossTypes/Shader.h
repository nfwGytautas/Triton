#pragma once

#include "TritonPlatform2/mathematical.h"

#include "TritonTypes/reference.h"
#include "TritonTypes/ShaderBuffers.h"

namespace Triton
{
	namespace Graphics
	{
		enum class Buffer : size_t
		{
			MATRICE = 0
		};

		class Shader
		{
		public:
			Shader();
			virtual ~Shader();

			/**
			 * Enables/binds the shader
			 */
			virtual void enable() = 0;

			/**
			 * Disables/unbinds the shader mainly used for debugging
			 */
			virtual void disable() = 0;

			/**
			 * Get the matrix buffer instance of this shader
			 *
			 * @return The matrix buffer instance
			 */
			MatrixBuffer& buffer_matrices();

			/**
			 * Updates the matrices buffer uniforms
			 */
			virtual void update_matrices() = 0;
			
		protected:
			/// Variable to keep track if the shader is bound or not
			bool enabled = false;
			
			/// Buffer containing the matrices used in shader calculations
			MatrixBuffer prtc_Matrices;
		};
	}
}