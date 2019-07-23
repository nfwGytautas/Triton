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
#include "Triton\Core\Wrapers\Image.h"

#include "Triton\Core\Input\InputManager.h"

#include "Triton\Core\RenderBuffer.h"

#include "Triton\Managers\AssetManager.h"
#include "Triton\Core\GameWindow\GameWindow.h"

namespace Triton
{
	// Base class that allows for multiple types of scenes that can all be managed by a single manager
	class SceneBase : public Core::TritonClass
	{
	public:
		SceneBase();
		virtual ~SceneBase() { }

		reference<ECS::Registry> Entities;
	};

	class Scene : public SceneBase
	{
	public:
		Scene();
		virtual ~Scene();

		// Resource management
		virtual void addLight(std::string type, reference<Graphics::Light> light);

		virtual void Prepare();

		virtual void onMessage(size_t message, void* payload) override;

		virtual Matrix44& getViewMatrix() const;
	public:
		Components::Visual m_CurrVisual;
		reference<PType::Shader> model_shader;
		reference<PType::Shader> image_shader;

		std::unique_ptr<Camera> Camera;
		std::vector<reference<Graphics::Light>> m_Lights;

		reference<Data::Mesh> BackgroundMesh;
		reference<Data::Material> BackgroundMaterial;
	private:
		// Updates all the scene contents
		void onUpdate();
		void onRender();
	private:
		std::unordered_map<std::string, unsigned int> m_LightCounts;

		reference<Core::RenderBuffer> m_mainRenderBuffer;
		reference<PType::Context> m_graphicsContext;
		reference<Manager::AssetManager> m_assetManager;
		reference<GameWindow> m_gameWindow;
	};

}