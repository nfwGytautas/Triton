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
#include "Triton\Entity\GameObject.h"
#include "Triton\Managers\GameObjectManager.h"

namespace Triton
{
	// Base class that allows for multiple types of scenes that can all be managed by a single manager
	//	The name of the scene comes from the triton class
	class SceneBase : public Core::TritonClass, public Core::GameObjectManager
	{
	public:
		SceneBase();
		virtual ~SceneBase() { }

		// Sets up the scene for rendering
		virtual void setupForRendering() {}

		// Sets up the scene for rendering it's game objects
		virtual void setupForObjects() {}

		// Sets up the scene for rendering it's images
		virtual void setupForImages() {}

		// Scene base calls this method for each game object that is not an image
		virtual void onRenderObject(reference<GameObject>& object, Components::Transform& transform, Components::Visual& visual) {}

		// Scene base calls this method for each game object that is specified to be an image
		virtual void onRenderImage(reference<GameObject>& object, Components::Transform& transform, Components::Image& image) {}

		// Scene base calls this method when all rendering is done and notifies the inheritors about it
		virtual void onRenderDone() {}

		// The update method
		virtual void onUpdate() = 0;

		// The messages received go to this method and relevant messages are sent to specific methods
		virtual void onMessage(size_t message, void* payload) override final;

		// Similar to onMessage but the scene base only sends the messages that aren't handled by itself 
		virtual void relayMessage(size_t message, void* payload) = 0;
	private:
		// The method for rendering the game objects
		//	Among other things it calls onRenderObject and onRenderImage
		void render();
	};

	class Scene : public SceneBase
	{
	public:
		Scene();
		virtual ~Scene();

		// Resource management
		virtual void addLight(std::string type, reference<Graphics::Light> light);
		
		virtual void relayMessage(size_t message, void* payload) override;

		virtual Matrix44& getViewMatrix() const;

		// Inherited via SceneBase
		virtual void setupForRendering() override;
		virtual void setupForObjects() override;
		virtual void setupForImages() override;
		virtual void onRenderObject(reference<GameObject>& object, Components::Transform& transform, Components::Visual& visual) override;
		virtual void onRenderImage(reference<GameObject>& object, Components::Transform& transform, Components::Image& image) override;
	private:
		// Renders the scene background
		void renderBackground();
	public:
		Components::Visual m_CurrVisual;
		reference<PType::Shader> model_shader;
		reference<PType::Shader> image_shader;

		std::unique_ptr<Camera> Camera;
		std::vector<reference<Graphics::Light>> m_Lights;

		reference<Data::Mesh> BackgroundMesh;
		reference<Data::Material> BackgroundMaterial;
	private:
		// Inherited via SceneBase
		virtual void onUpdate() override;
	private:
		std::unordered_map<std::string, unsigned int> m_LightCounts;

		reference<Core::RenderBuffer> m_mainRenderBuffer;
		reference<PType::Context> m_graphicsContext;
		reference<Manager::AssetManager> m_assetManager;
		reference<GameWindow> m_gameWindow;
	};

}