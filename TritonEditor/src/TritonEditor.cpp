#include <Triton.h>

#include <Triton/Utility/ClassGetters.h>

#include "Scenes/EditorScene.h"
#include "Scenes/TestScene.h"
#include <string>

namespace Triton
{
	class TritonEditor :
		public Core::TritonClass
	{
	public:
		TritonEditor()
		{
			m_tHost = new Triton::TritonHost();

			Triton::AppSettings appSettings;
			appSettings.WindowWidth = 1280;
			appSettings.WindowHeight = 720;

			m_tHost->init({ appSettings });
			m_tHost->registerClass(
				Triton::reference<Triton::TritonEditor>(this).as<Triton::Core::TritonClass>(),
				Triton::Core::ClassRegisterParams({ false, {}, "editor" }));

			m_tHost->registerClass(
				Triton::reference<Triton::EditorScene>(new Triton::EditorScene()).as<Triton::Core::TritonClass>(),
				Triton::Core::ClassRegisterParams({ false, {}, "editor_scene" }));

			m_tHost->registerClass(
				Triton::reference<Triton::TestScene>(new Triton::TestScene()).as<Triton::Core::TritonClass>(),
				Triton::Core::ClassRegisterParams({ false, {}, "test_scene" }));
		}

		void start()
		{
			m_tHost->start();
		}

	private:
		TritonHost* m_tHost;
	};
}

/*
class UnitTest1 : public Triton::Application
{
	Triton::reference<Triton::Data::Viewport> m_viewPort;



	Triton::reference<Triton::EditorScene> m_EditorScene;

	Triton::reference<Triton::Data::Mesh> edtr_3DPOINTER;
	Triton::reference<Triton::Data::Material> edtr_mat_3DPOINTER;

	size_t edtr_pointer_id;
public:
	
	UnitTest1(const Triton::AppSettings& aSettings)
		: Application(aSettings)
	{
		this->Listen<Triton::KeyPressedEvent>(Triton::EventBehavior(std::bind(&UnitTest1::keyPressed, this, std::placeholders::_1)));
		this->Listen<Triton::MouseMovedEvent>(Triton::EventBehavior(std::bind(&UnitTest1::mouseMoved, this, std::placeholders::_1)));
	}

	~UnitTest1()
	{

	}

	void loadEditorResources()
	{
		for (int i = 0; i < 1; i++)
	{
		edtr_pointer_id = m_MainScene->Entities->create();

		//m_MainScene->Entities->assign<Triton::Components::Transform>(Ent).Position = Triton::Vector3(0.0, 0.0, -25.0);
		auto& transform = m_MainScene->Entities->assign<Triton::Components::Transform>(edtr_pointer_id);

		transform.Position = Triton::Vector3(-5.0 + -(10 * i), 0.0, 25.0);
		transform.Scale = Triton::Vector3(5, 5, 5);
		transform.Rotation = Triton::Vector3(0.0, 0.0, 0.0);

		m_MainScene->Entities->assign<Triton::Components::Visual>(edtr_pointer_id, edtr_3DPOINTER->getAssetID(), edtr_mat_3DPOINTER->getAssetID());
	}

		// 3D pointer mesh
		Triton::Resource::AssetCreateParams asset_desc;
		asset_desc.Type = Triton::Resource::AssetCreateParams::AssetType::MESH;
		asset_desc.Paths[0] = "C:/dev/Triton/Models/3dpointer.obj";

		edtr_3DPOINTER = this->createAsset(asset_desc).as<Triton::Data::Mesh>();

		// 3D pointer material
		asset_desc.Type = Triton::Resource::AssetCreateParams::AssetType::MATERIAL;
		asset_desc.Paths[0] = "C:/dev/Triton/Models/3dpointer.png";

		edtr_mat_3DPOINTER = this->createAsset(asset_desc).as<Triton::Data::Material>();

		// 3D pointer shader
		asset_desc.Type = Triton::Resource::AssetCreateParams::AssetType::SHADER;
		asset_desc.Paths[0] = "C:/dev/Triton/Shaders/v_selector.hlsl";
		asset_desc.Paths[1] = "C:/dev/Triton/Shaders/f_selector.hlsl";

		asset_desc.Arguments[0] = "SelectorVertexShader";
		asset_desc.Arguments[1] = "SelectorPixelShader";

		edtr_mat_3DPOINTER->Shader = this->createAsset(asset_desc).as<Triton::Data::ShaderProgram>();
	}

	void PreExecutionSetup() override
	{
		loadEditorResources();
		CreateResources();	
	}

	void OnUpdate() override
	{

		auto& transform_pointer = m_MainScene->Entities->get<Triton::Components::Transform>(edtr_pointer_id);
		transform_pointer.Position = transform.Position;
	}
};

*/

int main(int argc, char** argv)
{
	try
	{
		Triton::TritonEditor editor;
		editor.start();
	}
	catch (...)
	{
		TR_ASSERT(0, "Unidentified error");
		return 1;
	}
}