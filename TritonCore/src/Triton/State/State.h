#pragma once

#include "Triton\Core\Data\Mesh.h"
#include "Triton\Core\Data\Texture.h"
#include "Triton\Core\Data\Material.h"
#include "Triton\Core\Shader\Shader.h"

#include "Triton\Core\Graphics\Lighting\Package.h"

#include "Triton\Limits.h"

namespace Triton
{
	//Forward application declaration for managing the instance of singleton
	class Application;

	namespace Singleton
	{

		// Class for keeping track of the current state of the hardware.
		// Since there can only be a single State at a given time this
		// class has been made into a singleton
		class State
		{
			//===============================================================
			//SINGLETON SPECIFIC
			//===============================================================
		private:
			// Allow the main class to control the lifetime of state
			friend Application;

			// Enforce single instance
			State();
			~State();

			// Instance
			static State* m_Instance;

			// Create instance
			static void Create();

			// Destroy instance
			static void Destroy();
		public:
			// Enforce single instance
			State(State const&) = delete;
			void operator=(State const&) = delete;

			// Create instance or get already existing one
			static relay_ptr<State> GetInstance();
			//===============================================================



			//===============================================================
			//STATE
			//===============================================================
		private:
			// Internal enum for light count tracking
			enum LightType : size_t
			{
				Directional = 0,
				Point = 1,
				Spot = 2
			};
		public:
			// Resets ALL lights. Use when changing scene however you will have to add all the 
			// desired ones again, consider changing the value of the current lights since
			// relay_ptr reflects the actual state of the object
			void ResetLights();

			// Bind directional light and return the current count, returns the free slot for light
			// NOTE1: The light isn't bound inside the state, it must be done sometime by the user
			// NOTE2: If the maximum amount is exceeded then the amount is set 0
			unsigned int AddLight(relay_ptr<Graphics::DirectionalLight> aLight);

			// Bind point light and return the current count, returns the free slot for light
			// NOTE: The light isn't bound inside the state, it must be done sometime by the user
			// NOTE2: If the maximum amount is exceeded then the amount is set 0
			unsigned int AddLight(relay_ptr<Graphics::PointLight> aLight);

			// Bind spot light and return the current count, returns the free slot for light
			// NOTE: The light isn't bound inside the state, it must be done sometime by the user
			// NOTE2: If the maximum amount is exceeded then the amount is set 0
			unsigned int AddLight(relay_ptr<Graphics::SpotLight> aLight);

			//Updates the light count uniforms
			void UpdateLightCounts();

			// Updates all generic uniforms that are tracked by the state.
			// NOTE1: This function binds all the tracked lights
			void UpdateUniforms();

			// Set current shader
			// NOTE: Uniforms aren't updated to do that call UpdateUniforms()
			void BindShader(relay_ptr<Core::Shader> aShader);

			// Return the current shader
			relay_ptr<Core::Shader> Shader() const
			{
				return m_CurrentShader;
			}
		public:
			relay_ptr<Data::Mesh> CurrentMesh;
			relay_ptr<Data::Material> CurrentMaterial;

		private:
			// Current shader
			relay_ptr<Core::Shader> m_CurrentShader;

			// Keeps track of the directional lights
			std::array<relay_ptr<Graphics::DirectionalLight>, TR_NR_MAX_DIR_LIGHTS> m_DirectionalLights;
			// Keep track of actually filled directional lights
			unsigned int m_count_dir = 0;

			// Keeps track of the point lights
			std::array<relay_ptr<Graphics::PointLight>, TR_NR_MAX_POINT_LIGHTS> m_PointLights;
			// Keep track of actually filled point lights
			unsigned int m_count_point = 0;

			// Keeps track of the spot lights
			std::array<relay_ptr<Graphics::SpotLight>, TR_NR_MAX_SPOT_LIGHTS> m_SpotLights;
			// Keep track of actually filled spot lights
			unsigned int m_count_spot = 0;

		};
	}
}