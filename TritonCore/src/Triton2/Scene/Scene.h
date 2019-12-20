#pragma once

#include "TritonTypes/ShaderBuffers.h"
#include "Triton2/Scene/ECS.h"
#include "Triton2/Scene/Camera.h"
#include "Triton2/File/Serializable.h"

namespace Triton
{
	namespace IO
	{
		struct SceneData;
	}

	namespace Graphics
	{
		struct LightBuffer;
	}

	/**
	 * Scene object
	 */
	class Scene : public Serializable
	{
	public:
		/**
		 * Creates an empty Scene instance used for serialization
		 */
		Scene();
		/**
		 * Creates a scene with a given name
		 *
		 * @param name Name of the scene
		 */
		Scene(std::string name);

		/**
		 * Updates all scene objects
		 */
		void onUpdate() const;

		/**
		 * Returns the name of the scene
		 *
		 * @return Name of the scene
		 */
		std::string getName() const;

		/**
		 * Returns the assets used by this scene
		 *
		 * @return Vector of scene assets
		 */
		std::vector<std::string>& assets();

		/**
		 * Returns the lights of this scene
		 *
		 * @return Graphics::LightBuffer structure of lights
		 */
		Graphics::LightBuffer& lights();

		/**
		 * Returns the entities of this scene
		 *
		 * @return Reference to ECS registry instance
		 */
		reference<ECS::Registry>& entities();

		/**
		 * Returns the entities of this scene that satisfies the predicate
		 *
		 * @return Vector containing the registry ids of entities
		 */
		std::vector<ECS::Entity> getByMeta(std::function<bool(const Triton::Components::MetaComponent&)> pred);

		/**
		 * Returns all cameras of this scene
		 *
		 * @return Vector containing all the cameras of this scene
		 */
		std::vector<reference<Camera>>& cameras();

		/**
		 * Returns the current active camera of this scene
		 *
		 * @return Triton reference to active camera
		 */
		reference<Camera> activeCamera() const;

		/**
		 * Sets the active camera to be of the specified name.
		 * If the camera is not part of the scene then a warning is
		 * logged and the first camera in the cameras vector is set as active.
		 *
		 * @return Triton reference to active camera
		 */
		void setActiveCamera(const std::string& name);

		// Inherited via Serializable
		virtual void serialize(BinaryOutputArchive & archive) override;
		// Inherited via Serializable
		virtual void deserialize(BinaryInputArchive & archive) override;
		// Inherited via Serializable
		virtual std::string type() override;
	private:
		/// Name of the scene
		std::string m_name;

		/// Asset names that are used in the scene
		std::vector<std::string> m_assets;

		/// Light buffer of this scene
		Graphics::LightBuffer m_lights;

		/// Entities used in this scene
		reference<ECS::Registry> m_entities;

		/// Cameras of this scene
		std::vector<reference<Camera>> m_cameras;

		/// Current camera
		reference<Camera> m_activeCamera;
	};
}