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

namespace Triton
{
	// Base class that allows for multiple types of scenes that can all be managed by a single manager
	class SceneBase
	{
	public:
		SceneBase(Triton::PType::Context* context, reference<Core::InputManager> iManager);
		virtual ~SceneBase() { }

		virtual void update(float delta) = 0;
		virtual void render(Core::RenderBuffer* renderBuffer) = 0;
		virtual void destroy() = 0;
	protected:
		Triton::PType::Context* Context;
		reference<Core::InputManager> Input;
	};

	class Scene : public SceneBase
	{
	public:
		Scene(Triton::PType::Context* context, reference<Core::InputManager> iManager);
		virtual ~Scene();


		// Resource management
		virtual void addLight(std::string type, reference<Graphics::Light> light);

		virtual void Prepare();

		virtual void render(Core::RenderBuffer* renderBuffer) override;
		virtual void update(float delta) override;

		virtual void destroy() override;

		virtual Matrix44& getViewMatrix() const;
	public: // Non virtual
		void UpdateProjection(Matrix44 aNewProjection);
		void UpdateOrthographic(Matrix44 aNewOrthographic);
	public:
		Components::Visual m_CurrVisual;
		reference<PType::Shader> model_shader;
		reference<PType::Shader> image_shader;

		std::unique_ptr<Camera> Camera;
		std::unique_ptr<ECS::Registry> Entities;
		std::vector<reference<Graphics::Light>> m_Lights;

		reference<Data::Mesh> BackgroundMesh;
		reference<Data::Material> BackgroundMaterial;
	private:
		std::unordered_map<std::string, unsigned int> m_LightCounts;
	private:
	};

}