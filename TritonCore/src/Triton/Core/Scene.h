#pragma once

#include "TritonPlatform/PlatformAbstraction.h"

#include "Triton\Core\Graphics\Lighting\Light.h"
#include "Triton\File\File.h"

#include "TritonTypes\data_collection.h"

#include "Triton\Entity\Registry.h"
#include "Triton\Entity\Components\TritonComponents.h"
#include "Triton\Camera\Camera.h"
#include "Triton\Serialize\Serialize.h"
#include "Triton\Core\Wrapers\Material.h"
#include "Triton\Core\Wrapers\Mesh.h"

namespace Triton
{
	class Scene
	{
	public:
		Scene(relay_ptr<Triton::PType::Context> context);
		virtual ~Scene();


		// Resource management
		virtual void addLight(std::string type, Triton::Graphics::Light* light);
		virtual void addAsset(size_t id, Triton::Resource::Asset* asset);


		virtual void Prepare();

		virtual void render();
		virtual void Update(float aDelta);

		virtual void destroy();
	public: // Non virtual
		void UpdateProjection(Triton::Matrix44 aNewProjection);

	public:
		Triton::PType::Shader* shader;

		std::unique_ptr<Camera> m_Camera;
		std::unique_ptr<ECS::Registry> Entities;
	private:
		relay_ptr<Triton::PType::Context> m_context;

		std::unordered_map<size_t, Triton::Resource::Asset*> m_Assets;

		std::vector<Triton::Graphics::Light*> m_Lights;
		std::unordered_map<std::string, unsigned int> m_LightCounts;

	private:
		Components::Visual m_CurrVisual;
	};

}