#include "Shader.h"

namespace Triton
{
	namespace Graphics
	{

		Shader::Shader(const Flags::ShaderFlagset& flags)
			: m_flags(flags)
		{
		}

		Shader::~Shader()
		{
		}

		MatrixBuffer& Shader::buffer_matrices()
		{
			return prtc_Matrices;
		}

		SettingsBuffer& Shader::buffer_settings()
		{
			return prtc_Settings;
		}

		LightBuffer& Shader::buffer_lights()
		{
			m_recalculateLightBuffer = true;
			return prtc_Lights;
		}

		CameraBuffer& Shader::buffer_camera()
		{
			return prtc_Camera;
		}

		void Shader::update_lights()
		{
			if (prtc_Lights.PointLights.size() < TR_NR_MAX_POINT_LIGHTS)
			{
				prtc_Settings.PointLights = prtc_Lights.PointLights.size();
			}
			else
			{
				prtc_Settings.PointLights = TR_NR_MAX_POINT_LIGHTS;
			}

			if (prtc_Lights.SpotLights.size() < TR_NR_MAX_SPOT_LIGHTS)
			{
				prtc_Settings.SpotLights = prtc_Lights.SpotLights.size();
			}
			else
			{
				prtc_Settings.SpotLights = TR_NR_MAX_SPOT_LIGHTS;
			}

			if (prtc_Lights.DirLights.size() < TR_NR_MAX_DIR_LIGHTS)
			{
				prtc_Settings.DirectionalLights = prtc_Lights.DirLights.size();
			}
			else
			{
				prtc_Settings.DirectionalLights = TR_NR_MAX_DIR_LIGHTS;
			}

			for (int i = 0; i < prtc_Settings.PointLights; i++)
			{
				Vector4 position =
					Vector4(
						prtc_Lights.PointLights[i].Position.x,
						prtc_Lights.PointLights[i].Position.y,
						prtc_Lights.PointLights[i].Position.z,
						1.0f
					);

				Vector4 color =
					Vector4(
						prtc_Lights.PointLights[i].Color.x,
						prtc_Lights.PointLights[i].Color.y,
						prtc_Lights.PointLights[i].Color.z,
						1.0f
					);

				Vector4 attenuationAndRange =
					Vector4(
						prtc_Lights.PointLights[i].Constant,
						prtc_Lights.PointLights[i].Linear,
						prtc_Lights.PointLights[i].Quadratic,
						prtc_Lights.PointLights[i].Range
					);

				prtc_PLights.Position[i] = position;
				prtc_PLights.Color[i] = color;
				prtc_PLights.AtennuationAndRange[i] = attenuationAndRange;
			}

			for (int i = 0; i < prtc_Settings.SpotLights; i++)
			{
				Vector4 position =
					Vector4(
						prtc_Lights.SpotLights[i].Position.x,
						prtc_Lights.SpotLights[i].Position.y,
						prtc_Lights.SpotLights[i].Position.z,
						1.0f
					);

				Vector4 colorandangle =
					Vector4(
						prtc_Lights.SpotLights[i].Color.x,
						prtc_Lights.SpotLights[i].Color.y,
						prtc_Lights.SpotLights[i].Color.z,
						prtc_Lights.SpotLights[i].Angle
					);

				Vector4 direction =
					Vector4(
						prtc_Lights.SpotLights[i].Direction.x,
						prtc_Lights.SpotLights[i].Direction.y,
						prtc_Lights.SpotLights[i].Direction.z,
						1.0f
					);

				Vector4 attenuationAndRange =
					Vector4(
						prtc_Lights.SpotLights[i].Constant,
						prtc_Lights.SpotLights[i].Linear,
						prtc_Lights.SpotLights[i].Quadratic,
						prtc_Lights.SpotLights[i].Range
					);

				prtc_SLights.Position[i] = position;
				prtc_SLights.ColorAndAngle[i] = colorandangle;
				prtc_SLights.Direction[i] = direction;
				prtc_SLights.AtennuationAndRange[i] = attenuationAndRange;
			}

			for (int i = 0; i < prtc_Settings.DirectionalLights; i++)
			{
				Vector4 position =
					Vector4(
						prtc_Lights.DirLights[i].Position.x,
						prtc_Lights.DirLights[i].Position.y,
						prtc_Lights.DirLights[i].Position.z,
						1.0f
					);

				Vector4 color =
					Vector4(
						prtc_Lights.DirLights[i].Color.x,
						prtc_Lights.DirLights[i].Color.y,
						prtc_Lights.DirLights[i].Color.z,
						1.0f
					);

				Vector4 direction =
					Vector4(
						prtc_Lights.DirLights[i].Direction.x,
						prtc_Lights.DirLights[i].Direction.y,
						prtc_Lights.DirLights[i].Direction.z,
						1.0f
					);

				prtc_DLights.Position[i] = position;
				prtc_DLights.Color[i] = color;
				prtc_DLights.Direction[i] = direction;
			}

			m_recalculateLightBuffer = false;
		}
	}
}