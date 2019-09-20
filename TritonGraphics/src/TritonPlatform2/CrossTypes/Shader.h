#pragma once

#include "TritonPlatform2/mathematical.h"

#include "TritonTypes/reference.h"
#include "TritonTypes/ShaderBuffers.h"

#include "TritonTypes/Flags.h"

namespace Triton
{
	namespace Graphics
	{
		class Shader
		{
		public:
			Shader(const Flags::ShaderFlagset& flags);
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
			 * Get the settings buffer instance of this shader
			 *
			 * @return The settings buffer instance
			 */
			SettingsBuffer& buffer_settings();

			/**
			 * Get the light buffer instance of this shader
			 *
			 * @return The light buffer instance
			 */
			LightBuffer& buffer_lights();

			/**
			 * Get the camera buffer instance of this shader
			 *
			 * @return The camera buffer instance
			 */
			CameraBuffer& buffer_camera();

			/**
			 * Updates the matrices buffer uniforms
			 */
			virtual void update_matrices() = 0;

			/**
			 * Regenerates lighting buffers
			 */
			void update_lights();
			
		protected:
			/// Variable to keep track if the shader is bound or not
			bool enabled = false;

			/// If True then the light buffer was changed
			bool m_recalculateLightBuffer = true;
			
			/// Buffer containing settings for configuring shader
			SettingsBuffer prtc_Settings;

			/// Buffer containing the matrices used in shader calculations
			MatrixBuffer prtc_Matrices;

			/// Buffer containing the lights used in shader calculations
			LightBuffer prtc_Lights;

			/// Struct of calculated point light data
			PointLights prtc_PLights;

			/// Struct of calculated spot light data
			SpotLights prtc_SLights;

			/// Struct of calculated directional light data
			DirectionalLights prtc_DLights;

			/// Camera data used in shader calculations
			CameraBuffer prtc_Camera;

			/// Flags for this shader for more look at ShaderFlags enum
			Flags::ShaderFlagset m_flags;
		};
	}
}