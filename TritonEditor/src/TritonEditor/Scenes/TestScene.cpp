#include "TestScene.h"

#include <Triton/Utility/ClassGetters.h>
#include <Triton/Managers/AssetManager.h>

#include <Triton/Core/Graphics/Lighting/DirectionalLight.h>
#include <Triton/Core/Graphics/Lighting/PointLight.h>
#include <Triton/Core/Graphics/Lighting/SpotLight.h>

Triton::TestScene::TestScene()
{
	this->Listen<KeyPressedEvent>(EventBehavior(std::bind(&TestScene::keyPressed, this, std::placeholders::_1)));
	this->Listen<MouseMovedEvent>(EventBehavior(std::bind(&TestScene::mouseMoved, this, std::placeholders::_1)));
}

void Triton::TestScene::relayMessage(size_t message, void* payload)
{
	Scene::relayMessage(message, payload);

	switch (message)
	{
	case (size_t)Core::TritonMessageType::ClassRegistered:
	{
		TR_GET_EDTR_STATE(m_edtr_state);

		m_edtr_state->CurrentScene = this;

		TR_GET_CLASS(InputManager);

		m_input = InputManager.as<Triton::Core::InputManager>();
		m_input->AddListener(*this);

		m_timer = this->getClassByID((size_t)Core::TritonClasses::Timer).as<Utility::Timer>();

		loadAssets();
		createEntities();

		return;
	}
	case (size_t)Core::TritonMessageType::Update:
	{
		updateEntities();
		return;
	}
	}
}

bool Triton::TestScene::keyPressed(const Event & event)
{
	const Triton::KeyPressedEvent& kpe = dynamic_cast<const Triton::KeyPressedEvent&>(event);

	int keycode = kpe.GetKeyCode();

	TR_SYSTEM_TRACE("{0}", keycode);

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

bool Triton::TestScene::mouseMoved(const Event & event)
{
	if (m_input->getMouse()->Keys[(size_t)Triton::MouseKey::BUTTON_MIDDLE] && m_input->focused())
	{
		const Triton::MouseMovedEvent& mme = dynamic_cast<const Triton::MouseMovedEvent&>(event);
	
		float sensitivity = 0.3f;
		float xoffset = mme.GetX() * sensitivity;
		float yoffset = mme.GetY() * sensitivity;
	
		Camera->Yaw += xoffset;
		Camera->Pitch += yoffset;
	}

	return false;
}

void Triton::TestScene::createEntities()
{
	TR_GET_CLASS_AS(Context, PType::Context);

	Context->window->showCursor(true);
	//prtc_Display->SetVSync(true);
	auto dlight = new Triton::Graphics::DirectionalLight(Triton::Vector3(1.0f, 0.0f, 1.0f));
	dlight->Ambient = Triton::Vector3(0.15f, 0.15f, 0.15f);
	dlight->Specular = Triton::Vector3(1.0f, 1.0f, 1.0f);
	//dlight->Ambient = Triton::Vector3(0.0f, 0.0f, 0.0f);
	addLight("directionallight", dlight);
	addLight("pointlight", new Triton::Graphics::PointLight(Triton::Vector3(20.0, 0.0, -25.0)));
	addLight("spotlight", new Triton::Graphics::SpotLight(Triton::Vector3(0.0, 0.0, 0.0), Triton::Vector3(0.0f, 0.0f, -1.0f)));


	for (int i = 0; i < 2; i++)
	{
		Ent = newObject();

		//m_MainScene->Entities->assign<Triton::Components::Transform>(Ent).Position = Triton::Vector3(0.0, 0.0, -25.0);
		auto& transform = Ent->addComponent<Triton::Components::Transform>();

		transform.Position = Triton::Vector3(0.0 + (10 * i), 0.0, 25.0);
		//transform.Scale = Triton::Vector3(10, 10, 10);
		transform.Rotation = Triton::Vector3(0.0, 180.0, 0.0);

		Ent->addComponent<Triton::Components::Visual>(mesh->getAssetID(), mat->getAssetID());

		m_edtr_state->CurrentGameObject = Ent;
		Ent->setName("stall" + std::to_string(i));
	}

	Ent->attachScript("Class1");

	//for (int i = 0; i < 1; i++)
	//{
	//	uint32_t Ent2 = Entities->create();
	//
	//	//m_MainScene->Entities->assign<Triton::Components::Transform>(Ent).Position = Triton::Vector3(0.0, 0.0, -25.0);
	//	auto& transform = Entities->assign<Triton::Components::Transform>(Ent2);
	//
	//	transform.Position = Triton::Vector3(0.0, 0.0, 15.0);
	//
	//	Entities->assign<Triton::Components::Image>(Ent2, image->getAssetID());
	//}

	//auto Ent1 = m_MainScene->Entities->create();
	//m_MainScene->Entities->assign<Triton::Components::LightEmitter>(Ent1, m_Light1);
	//
	//auto Ent2 = m_MainScene->Entities->create();
	//m_MainScene->Entities->assign<Triton::Components::LightEmitter>(Ent2, m_Light2);
}

void Triton::TestScene::updateEntities()
{
	//static int movingDir = 1;
	//
	//if(Ent->hasComponent<Triton::Components::Transform>())
	//{
	//	auto& transform = Ent->getComponent<Triton::Components::Transform>();
	//	transform.Rotation.y += m_timer->updateDelta() * 15;
	//
	//	transform.Position.y += movingDir * m_timer->updateDelta() * 5;
	//
	//	if (transform.Position.y > 5)
	//	{
	//		movingDir = -1;
	//	}
	//	else if (transform.Position.y < -5)
	//	{
	//		movingDir = 1;
	//	}
	//}

	Triton::Impl::logErrors();
}

void Triton::TestScene::loadAssets()
{
	TR_GET_CLASS_AS(AssetManager, Manager::AssetManager);

	// Asset creation description
	Triton::Resource::AssetCreateParams asset_desc;
	asset_desc.Type = Triton::Resource::AssetCreateParams::AssetType::SHADER;
	asset_desc.Paths[0] = "D:/Programming/Test files/nfw/shaders/directx/vertex_lighting.hlsl";
	asset_desc.Paths[1] = "D:/Programming/Test files/nfw/shaders/directx/fragment_lighting.hlsl";

	asset_desc.Arguments[0] = "ColorVertexShader";
	asset_desc.Arguments[1] = "ColorPixelShader";
	asset_desc.Name = "Shader1";

	// Shader creation process
	auto Shader1 = AssetManager->createAsset(asset_desc).as<Triton::Data::ShaderProgram>();
	model_shader = Shader1->Program;

	// Mesh creation process
	asset_desc.Type = Triton::Resource::AssetCreateParams::AssetType::MESH;
	asset_desc.Paths[0] = "D:/Programming/Test files/nfw/stall.obj";
	asset_desc.Name = "stall_mesh";

	mesh = AssetManager->createAsset(asset_desc).as<Triton::Data::Mesh>();

	asset_desc.Paths[0] = "D:/Programming/Test files/nfw/simple_cube.obj";

	cubeMesh = AssetManager->createAsset(asset_desc).as<Triton::Data::Mesh>();


	// Material creation process
	asset_desc.Type = Triton::Resource::AssetCreateParams::AssetType::MATERIAL;
	asset_desc.Paths[0] = "D:/Programming/Test files/nfw/stallTexture.png";
	asset_desc.Name = "stall_mat";

	mat = AssetManager->createAsset(asset_desc).as<Triton::Data::Material>();

	asset_desc.Type = Triton::Resource::AssetCreateParams::AssetType::CUBEMAP;
	asset_desc.Paths[0] = "D:/Programming/Test files/nfw/right.jpg";
	asset_desc.Paths[1] = "D:/Programming/Test files/nfw/left.jpg";
	asset_desc.Paths[2] = "D:/Programming/Test files/nfw/bottom.jpg";
	asset_desc.Paths[3] = "D:/Programming/Test files/nfw/top.jpg";
	asset_desc.Paths[4] = "D:/Programming/Test files/nfw/front.jpg";
	asset_desc.Paths[5] = "D:/Programming/Test files/nfw/back.jpg";

	cubeMat = AssetManager->createAsset(asset_desc).as<Triton::Data::Material>();

	mat->Shader = Shader1;
	mat->Ambient = Triton::Vector3(0.5f, 0.5f, 0.5f);
	cubeMat->Ambient = Triton::Vector3(0.5f, 0.5f, 0.5f);
	mat->Shininess = 32;
	cubeMat->Shininess = 32;

	// Image creation process
	asset_desc.Type = Triton::Resource::AssetCreateParams::AssetType::IMAGE;
	asset_desc.Width = 100;
	asset_desc.Height = 100;
	asset_desc.Paths[0] = "D:/Programming/Test files/nfw/stallTexture.png";
	asset_desc.Name = "stall_image";

	image = AssetManager->createAsset(asset_desc).as<Triton::Data::Image>();
	image->Bitmap->setPosition(150, 150);


	Camera->Position = Triton::Vector3(0.0f, 0.0f, -5.0f);


	// Create bitmap shader
	asset_desc.Type = Triton::Resource::AssetCreateParams::AssetType::SHADER;
	asset_desc.Paths[0] = "D:/Programming/Test files/nfw/shaders/directx/vertex_texture.hlsl";
	asset_desc.Paths[1] = "D:/Programming/Test files/nfw/shaders/directx/fragment_texture.hlsl";


	image_shader = AssetManager->createAsset(asset_desc).as<Triton::Data::ShaderProgram>()->Program;

	asset_desc.Type = Triton::Resource::AssetCreateParams::AssetType::SHADER;
	asset_desc.Paths[0] = "D:/Programming/Test files/nfw/shaders/directx/vertex_skybox.hlsl";
	asset_desc.Paths[1] = "D:/Programming/Test files/nfw/shaders/directx/fragment_skybox.hlsl";

	cubeMat->Shader = AssetManager->createAsset(asset_desc).as<Triton::Data::ShaderProgram>();


	BackgroundMesh = cubeMesh;
	BackgroundMaterial = cubeMat;

	m_edtr_state->AllMaterials.push_back(mat);
	m_edtr_state->AllMeshes.push_back(mesh);
	m_edtr_state->AllShaders.push_back(Shader1);
}
