#include <Triton.h>
#include "EditorScene.h"
#include <string>

namespace Triton
{

}

class UnitTest1 : public Triton::Application
{
	Triton::reference<Triton::Data::Viewport> m_viewPort;

	size_t Ent;

	Triton::Matrix44 m_Transformation;

	size_t m_Light1;
	size_t m_Light2;

	Triton::reference<Triton::Scene> m_MainScene;
	Triton::reference<Triton::Data::Material> mat;
	Triton::reference<Triton::Data::Material> cubeMat;
	Triton::reference<Triton::Data::Mesh> mesh;
	Triton::reference<Triton::Data::Mesh> cubeMesh;
	Triton::reference<Triton::Data::Image> image;

	Triton::reference<Triton::EditorScene> m_EditorScene;
public:
	void CreateResources()
	{
		m_MainScene = SceneManager->createScene();
		m_EditorScene = SceneManager->createSceneCustom<Triton::EditorScene>((Triton::Core::EventManager*)this);

		// Create shader
		Triton::PType::ShaderCreateParams* shader_params = new Triton::PType::ShaderCreateParams();
		//shader_params->vertexPath = "D:/Programming/Test files/nfw/shaders/triton/v4.shader";
		//shader_params->fragmentPath = "D:/Programming/Test files/nfw/shaders/triton/fragment_lighting.shader";

		shader_params->vertexPath = "D:/Programming/Test files/nfw/shaders/directx/vertex_lighting.hlsl";
		shader_params->fragmentPath = "D:/Programming/Test files/nfw/shaders/directx/fragment_lighting.hlsl";

		shader_params->entryPointVertex = "ColorVertexShader";
		shader_params->entryPointFragment = "ColorPixelShader";

		Triton::PType::ShaderLayout shader_layout = Triton::Data::File::ReadShaderLayout(shader_params);

		shader_params->layout = &shader_layout;

		auto Shader = Context->factory->createShader(shader_params).as<Triton::PType::Shader>();
		m_MainScene->model_shader = Shader;

		delete shader_params;

		// Mesh creation process
		Triton::Resource::AssetCreateParams asset_desc;
		asset_desc.Type = Triton::Resource::AssetCreateParams::AssetType::MESH;
		asset_desc.Paths[0] = "D:/Programming/Test files/nfw/stall.obj";

		mesh = this->createAsset(asset_desc).as<Triton::Data::Mesh>();

		asset_desc.Paths[0] = "D:/Programming/Test files/nfw/simple_cube.obj";

		cubeMesh = this->createAsset(asset_desc).as<Triton::Data::Mesh>();


		// Material creation process
		asset_desc.Type = Triton::Resource::AssetCreateParams::AssetType::MATERIAL;
		asset_desc.Paths[0] = "D:/Programming/Test files/nfw/stallTexture.png";

		mat = this->createAsset(asset_desc).as<Triton::Data::Material>();

		asset_desc.Type = Triton::Resource::AssetCreateParams::AssetType::CUBEMAP;
		asset_desc.Paths[0] = "D:/Programming/Test files/nfw/right.jpg";
		asset_desc.Paths[1] = "D:/Programming/Test files/nfw/left.jpg";
		asset_desc.Paths[2] = "D:/Programming/Test files/nfw/bottom.jpg";
		asset_desc.Paths[3] = "D:/Programming/Test files/nfw/top.jpg";
		asset_desc.Paths[4] = "D:/Programming/Test files/nfw/front.jpg";
		asset_desc.Paths[5] = "D:/Programming/Test files/nfw/back.jpg";

		cubeMat = this->createAsset(asset_desc).as<Triton::Data::Material>();

		mat->Shader = Shader;
		mat->Ambient = Triton::Vector3(0.5f, 0.5f, 0.5f);
		cubeMat->Ambient = Triton::Vector3(0.5f, 0.5f, 0.5f);
		mat->Shininess = 32;
		cubeMat->Shininess = 32;

		// Viewport creation process
		asset_desc.Type = Triton::Resource::AssetCreateParams::AssetType::VIEWPORT;
		asset_desc.Width = 1280;
		asset_desc.Height = 720;

		m_viewPort = this->createAsset(asset_desc).as<Triton::Data::Viewport>();
		m_EditorScene->ViewPort = m_viewPort;

		// Image creation process
		asset_desc.Type = Triton::Resource::AssetCreateParams::AssetType::IMAGE;
		asset_desc.Width = 100;
		asset_desc.Height = 100;
		asset_desc.Paths[0] = "D:/Programming/Test files/nfw/stallTexture.png";

		image = this->createAsset(asset_desc).as<Triton::Data::Image>();
		image->Bitmap->setPosition(150, 150);


		m_MainScene->Camera->Position = Triton::Vector3(0.0f, 0.0f, -5.0f);


		// Create bitmap shader
		Triton::PType::ShaderCreateParams* shader2_params = new Triton::PType::ShaderCreateParams();
		//shader_params->vertexPath = "D:/Programming/Test files/nfw/shaders/triton/v4.shader";
		//shader_params->fragmentPath = "D:/Programming/Test files/nfw/shaders/triton/fragment_lighting.shader";

		shader2_params->vertexPath = "D:/Programming/Test files/nfw/shaders/directx/vertex_texture.hlsl";
		shader2_params->fragmentPath = "D:/Programming/Test files/nfw/shaders/directx/fragment_texture.hlsl";

		shader2_params->entryPointVertex = "ColorVertexShader";
		shader2_params->entryPointFragment = "ColorPixelShader";

		Triton::PType::ShaderLayout shader2_layout = Triton::Data::File::ReadShaderLayout(shader2_params);

		shader2_params->layout = &shader2_layout;

		auto Shader2 = Context->factory->createShader(shader2_params).as<Triton::PType::Shader>();
		m_MainScene->image_shader = Shader2;

		delete shader2_params;

		// Create shader
		Triton::PType::ShaderCreateParams* shader3_params = new Triton::PType::ShaderCreateParams();
		//shader_params->vertexPath = "D:/Programming/Test files/nfw/shaders/triton/v4.shader";
		//shader_params->fragmentPath = "D:/Programming/Test files/nfw/shaders/triton/fragment_lighting.shader";

		shader3_params->vertexPath = "D:/Programming/Test files/nfw/shaders/directx/vertex_skybox.hlsl";
		shader3_params->fragmentPath = "D:/Programming/Test files/nfw/shaders/directx/fragment_skybox.hlsl";

		shader3_params->entryPointVertex = "ColorVertexShader";
		shader3_params->entryPointFragment = "ColorPixelShader";

		Triton::PType::ShaderLayout shader3_layout = Triton::Data::File::ReadShaderLayout(shader3_params);

		shader3_params->layout = &shader3_layout;

		cubeMat->Shader = Context->factory->createShader(shader3_params).as<Triton::PType::Shader>();

		delete shader3_params;


		m_MainScene->BackgroundMesh = cubeMesh;
		m_MainScene->BackgroundMaterial = cubeMat;
	}

	UnitTest1(const Triton::AppSettings& aSettings)
		: Application(aSettings)
	{
		this->Listen<Triton::KeyPressedEvent>(Triton::EventBehavior(std::bind(&UnitTest1::keyPressed, this, std::placeholders::_1)));
		this->Listen<Triton::MouseMovedEvent>(Triton::EventBehavior(std::bind(&UnitTest1::mouseMoved, this, std::placeholders::_1)));
	}

	~UnitTest1()
	{

	}

	void PreExecutionSetup() override
	{
		CreateResources();	

		Context->window->showCursor(true);
		//prtc_Display->SetVSync(true);
		auto dlight = new Triton::Graphics::DirectionalLight(Triton::Vector3(1.0f, 0.0f, 1.0f));
		dlight->Ambient = Triton::Vector3(0.15f, 0.15f, 0.15f);
		dlight->Specular = Triton::Vector3(1.0f, 1.0f, 1.0f);
		//dlight->Ambient = Triton::Vector3(0.0f, 0.0f, 0.0f);
		m_MainScene->addLight("directionallight", dlight);
		m_MainScene->addLight("pointlight", new Triton::Graphics::PointLight(Triton::Vector3(20.0, 0.0, -25.0)));		
		m_MainScene->addLight("spotlight", new Triton::Graphics::SpotLight(Triton::Vector3(0.0, 0.0, 0.0), Triton::Vector3(0.0f, 0.0f, -1.0f)));


		for (int i = 0; i < 2; i++)
		{
			Ent = m_MainScene->Entities->create();
		
			//m_MainScene->Entities->assign<Triton::Components::Transform>(Ent).Position = Triton::Vector3(0.0, 0.0, -25.0);
			auto& transform = m_MainScene->Entities->assign<Triton::Components::Transform>(Ent);

			transform.Position = Triton::Vector3(0.0 + (10 * i), 0.0, 25.0);
			//transform.Scale = Triton::Vector3(10, 10, 10);
			transform.Rotation = Triton::Vector3(0.0, 180.0, 0.0);
		
			m_MainScene->Entities->assign<Triton::Components::Visual>(Ent, mesh->getAssetID(), mat->getAssetID());
		}

		//for (int i = 0; i < 2; i++)
		//{
		//	auto entity = m_MainScene->Entities->create();
		//
		//	//m_MainScene->Entities->assign<Triton::Components::Transform>(Ent).Position = Triton::Vector3(0.0, 0.0, -25.0);
		//	auto& transform = m_MainScene->Entities->assign<Triton::Components::Transform>(entity);
		//
		//	transform.Position = Triton::Vector3(-5.0 + -(10 * i), 0.0, 25.0);
		//	//transform.Scale = Triton::Vector3(10, 10, 10);
		//	transform.Rotation = Triton::Vector3(0.0, 0.0, 0.0);
		//
		//	m_MainScene->Entities->assign<Triton::Components::Visual>(entity, cubeMesh->getAssetID(), cubeMat->getAssetID());
		//}

		for (int i = 0; i < 1; i++)
		{
			uint32_t Ent2 = m_MainScene->Entities->create();

			//m_MainScene->Entities->assign<Triton::Components::Transform>(Ent).Position = Triton::Vector3(0.0, 0.0, -25.0);
			auto& transform = m_MainScene->Entities->assign<Triton::Components::Transform>(Ent2);

			transform.Position = Triton::Vector3(0.0, 0.0, 15.0);

			m_MainScene->Entities->assign<Triton::Components::Image>(Ent2, image->getAssetID());
		}
		
		//auto Ent1 = m_MainScene->Entities->create();
		//m_MainScene->Entities->assign<Triton::Components::LightEmitter>(Ent1, m_Light1);
		//
		//auto Ent2 = m_MainScene->Entities->create();
		//m_MainScene->Entities->assign<Triton::Components::LightEmitter>(Ent2, m_Light2);
	}

	void OnUpdate() override
	{
		auto& transform = m_MainScene->Entities->get<Triton::Components::Transform>(Ent);
		transform.Rotation.y += 0.01f;
		m_MainScene->update(prtc_Delta);
		m_EditorScene->update(prtc_Delta);
		Triton::Impl::logErrors();
	}

	bool keyPressed(const Triton::Event& event)
	{
		const Triton::KeyPressedEvent& kpe = dynamic_cast<const Triton::KeyPressedEvent&>(event);

		int keycode = kpe.GetKeyCode();

		//float cameraSpeed = 2.5 * prtc_Delta * 10;
		//if (keycode == (int)'W')
		//	m_MainScene->m_Camera->Position += cameraSpeed * m_MainScene->m_Camera->GetViewDirection();
		//if (keycode == (int)'S')
		//	m_MainScene->m_Camera->Position -= cameraSpeed * m_MainScene->m_Camera->GetViewDirection();
		//if (keycode == (int)'A')
		//	m_MainScene->m_Camera->Position -= glm::normalize(glm::cross(m_MainScene->m_Camera->GetViewDirection(), Triton::Vector3(0.0f, 1.0f, 0.0f))) * cameraSpeed;
		//if (keycode == (int)'D')
		//	m_MainScene->m_Camera->Position += glm::normalize(glm::cross(m_MainScene->m_Camera->GetViewDirection(), Triton::Vector3(0.0f, 1.0f, 0.0f))) * cameraSpeed;
		//if (keycode == (int)'`')
		//{
		//	try
		//	{
		//		this->Restart();
		//	}
		//	catch (const std::runtime_error &re) {
		//		OutputDebugStringA(re.what());
		//		::MessageBoxA(NULL, re.what(), "Error initializing sample", MB_OK);
		//	}
		//}

		return false;
	}
	bool mouseMoved(const Triton::Event& event)
	{
		if (Input->getKeyboard()->Keys[(size_t)Triton::Key::KEY_SPACE])
		{
			const Triton::MouseMovedEvent& mme = dynamic_cast<const Triton::MouseMovedEvent&>(event);

			float sensitivity = 0.3f;
			float xoffset = mme.GetX() * sensitivity;
			float yoffset = mme.GetY() * sensitivity;

			m_MainScene->Camera->Yaw += xoffset;
			m_MainScene->Camera->Pitch += yoffset;
		}

		return false;
	}

	virtual void Render() override
	{
		renderScene(m_MainScene, m_viewPort);
		m_EditorScene->render();
	}
};

Triton::Application* Triton::CreateApplication(Triton::AppSettings& aSettings)
{
	aSettings.WindowWidth = 1280;
	aSettings.WindowHeight = 720;
	return new UnitTest1(aSettings);
}

void Triton::Loop(Triton::Application* application)
{
	while (!application->shouldClose())
	{
		application->frame();
	}
}