#pragma once

#include "TritonPlatform2/mathematical.h"
#include "Triton2/Scene/Lighting.h"
#include "Triton2/Limits.h"

namespace Triton
{
	class DirectionalLight;
	class SpotLight;
	class PointLight;

	namespace Graphics
	{
		/**
		 * The buffer containing all the settings for the shader
		 * that are used during rendering
		 */
		struct SettingsBuffer
		{
			/// Don't set automatically calculated by the shader
			float PointLights;

			/// Don't set automatically calculated by the shader
			float DirectionalLights;

			/// Don't set automatically calculated by the shader
			float SpotLights;

			float padding;
		};

		/**
		 * The buffer containing all the necessary matrices
		 */
		struct MatrixBuffer
		{
			Matrix44 Projection;
			Matrix44 View;
			Matrix44 Model;
		};

		/**
		 * The buffer containing all point lights that are passed to shader
		 */
		struct PointLights
		{
			Vector4 Position[TR_NR_MAX_POINT_LIGHTS];
			Vector4 Color[TR_NR_MAX_POINT_LIGHTS];
			Vector4 AtennuationAndRange[TR_NR_MAX_POINT_LIGHTS];
		};

		/**
		 * The buffer containing all spot lights that are passed to shader
		 */
		struct SpotLights
		{
			Vector4 Position[TR_NR_MAX_SPOT_LIGHTS];
			Vector4 ColorAndAngle[TR_NR_MAX_SPOT_LIGHTS];
			Vector4 Direction[TR_NR_MAX_SPOT_LIGHTS];
			Vector4 AtennuationAndRange[TR_NR_MAX_SPOT_LIGHTS];
		};

		/**
		 * The buffer containing all directional lights that are passed to shader
		 */
		struct DirectionalLights
		{
			Vector4 Position[TR_NR_MAX_DIR_LIGHTS];
			Vector4 Color[TR_NR_MAX_DIR_LIGHTS];
			Vector4 Direction[TR_NR_MAX_DIR_LIGHTS];
		};

		/**
		 * The buffer containing all the necessary variables for lighting calculations
		 */
		struct LightBuffer
		{
			/**
			 * Vector containing all directional lights used in the scene if
			 * size of the vector is bigger than the supported count of directional
			 * lights then the ensuing lights are ignored
			 */
			std::vector<DirectionalLight> DirLights;

			/**
			 * Vector containing all positional lights used in the scene if
			 * size of the vector is bigger than the supported count of point
			 * lights then the ensuing lights are ignored
			 */
			std::vector<PointLight> PointLights;

			/**
			 * Vector containing all spot lights used in the scene if
			 * size of the vector is bigger than the supported count of spot
			 * lights then the ensuing lights are ignored
			 */
			std::vector<SpotLight> SpotLights;
		};

		/**
		 * The buffer containing all camera variables
		 */
		struct CameraBuffer
		{
			Vector4 Position;
			Vector4 ViewDirection;
		};

		/**
		 * The buffer containing all the vertex input information
		 */
		struct VertexInput
		{
			Vector3 Position;
			Vector2 UV;
			Vector3 Normal;
		};
	}
}