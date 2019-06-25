#include <Triton.h>
#include "EditorScene.h"
#include <string>

namespace Triton
{

}

class UnitTest1 : public Triton::Application
{
	Triton::reference<Triton::PType::Framebuffer> m_viewPort;


	size_t m_StallMesh = 1;
	size_t m_StallMaterial = 2;
	size_t m_BitmapID = 3;

	size_t Ent;

	Triton::Matrix44 m_Transformation;

	size_t m_Light1;
	size_t m_Light2;

	double lastX;
	double lastY;
	bool m_firstMouse = true;

	Triton::reference<Triton::Scene> m_MainScene;
	Triton::reference<Triton::Data::Material> mat;
	Triton::reference<Triton::Data::Mesh> mesh;
	Triton::reference<Triton::Data::Image> image;

	Triton::reference<Triton::EditorScene> m_EditorScene;

	Triton::reference<Triton::PType::Bitmap> bitmap;
public:
	void CreateResources()
	{
		m_MainScene = SceneManager->createScene();
		m_EditorScene = SceneManager->createSceneCustom<Triton::EditorScene>((Triton::Core::EventManager*)this);

		Triton::PType::TextureCreateParams* fbo_params = new Triton::PType::TextureCreateParams();
		fbo_params->width = 1280;
		fbo_params->height = 720;
		m_viewPort = Context->factory->createFramebuffer(fbo_params).as<Triton::PType::Framebuffer>();
		delete fbo_params;

		m_EditorScene->ViewPort = m_viewPort;

		// Create shader
		Triton::PType::ShaderCreateParams* shader_params = new Triton::PType::ShaderCreateParams();
		//shader_params->vertexPath = "D:/Programming/Test files/nfw/shaders/triton/v4.shader";
		//shader_params->fragmentPath = "D:/Programming/Test files/nfw/shaders/triton/fragment_lighting.shader";

		shader_params->vertexPath = "D:/Programming/Test files/nfw/shaders/directx/vertex_lighting.hlsl";
		shader_params->fragmentPath = "D:/Programming/Test files/nfw/shaders/directx/fragment_lighting.hlsl";

		Triton::PType::ShaderLayout shader_layout(
			{			
				{"POSITION", Triton::PType::ShaderDataType::Float4},
				{"TEXCOORD", Triton::PType::ShaderDataType::Float2},
				{"NORMAL", Triton::PType::ShaderDataType::Float3}
			},
			{ 
				{ 
					"Persistant", 
					Triton::PType::BufferUpdateType::PERSISTANT,
					Triton::PType::BufferShaderType::VERTEX, 
					{ 
						{"projectionMatrix", Triton::PType::ShaderDataType::Mat4}
					}
				},
				{
					"PerFrame",
					Triton::PType::BufferUpdateType::FRAME,
					Triton::PType::BufferShaderType::VERTEX,
					{
						{"viewMatrix", Triton::PType::ShaderDataType::Mat4}
					}
				},
				{
					"PerObject",
					Triton::PType::BufferUpdateType::OBJECT,
					Triton::PType::BufferShaderType::VERTEX,
					{
						{"transformationMatrix", Triton::PType::ShaderDataType::Mat4}
					}
				},
				{
					"CameraBuffer",
					Triton::PType::BufferUpdateType::FRAME,
					Triton::PType::BufferShaderType::VERTEX,
					{
						{"cameraPosition", Triton::PType::ShaderDataType::Float3},
						{"padding", Triton::PType::ShaderDataType::Float}
					}
				},
				{
					"LightBuffer",
					Triton::PType::BufferUpdateType::FRAME,
					Triton::PType::BufferShaderType::PIXEL,
					{
						{"ambientColor", Triton::PType::ShaderDataType::Float4},
						{"diffuseColor", Triton::PType::ShaderDataType::Float4},
						{"specularColor", Triton::PType::ShaderDataType::Float4},
						{"lightDirection", Triton::PType::ShaderDataType::Float3},
						{"specularPower", Triton::PType::ShaderDataType::Float},
					}
				},
			});

		shader_params->layout = &shader_layout;

		auto Shader = Context->factory->createShader(shader_params).as<Triton::PType::Shader>();
		m_MainScene->model_shader = Shader;

		delete shader_params;

		Shader->enable();
		auto proj_mat = Context->renderer->projection();
		Shader->setBufferValue("Persistant", "projectionMatrix", &proj_mat);


		// Create bitmap shader
		Triton::PType::ShaderCreateParams* shader2_params = new Triton::PType::ShaderCreateParams();
		//shader_params->vertexPath = "D:/Programming/Test files/nfw/shaders/triton/v4.shader";
		//shader_params->fragmentPath = "D:/Programming/Test files/nfw/shaders/triton/fragment_lighting.shader";

		shader2_params->vertexPath = "D:/Programming/Test files/nfw/shaders/directx/vertex_texture.hlsl";
		shader2_params->fragmentPath = "D:/Programming/Test files/nfw/shaders/directx/fragment_texture.hlsl";

		Triton::PType::ShaderLayout shader2_layout(
			{
				{"POSITION", Triton::PType::ShaderDataType::Float4},
				{"TEXCOORD", Triton::PType::ShaderDataType::Float2},
			},
			{
				{
					"Persistant",
					Triton::PType::BufferUpdateType::PERSISTANT,
					Triton::PType::BufferShaderType::VERTEX,
					{
						{"projectionMatrix", Triton::PType::ShaderDataType::Mat4}
					}
				},
				{
					"PerFrame",
					Triton::PType::BufferUpdateType::FRAME,
					Triton::PType::BufferShaderType::VERTEX,
					{
						{"viewMatrix", Triton::PType::ShaderDataType::Mat4}
					}
				},
				{
					"PerObject",
					Triton::PType::BufferUpdateType::OBJECT,
					Triton::PType::BufferShaderType::VERTEX,
					{
						{"transformationMatrix", Triton::PType::ShaderDataType::Mat4}
					}
				},
			});

		shader2_params->layout = &shader2_layout;

		auto Shader2 = Context->factory->createShader(shader2_params).as<Triton::PType::Shader>();
		m_MainScene->image_shader = Shader2;

		delete shader2_params;


		// Create VAO
		Triton::PType::VAOCreateParams* vao_params = new Triton::PType::VAOCreateParams();
		Triton::Data::File::ReadMesh("D:/Programming/Test files/nfw/stall.obj", vao_params);

		auto vao = Context->factory->createVAO(vao_params).as<Triton::PType::VAO>();

		delete vao_params;

		// Create mesh
		mesh = Triton::reference<Triton::Data::Mesh>(new Triton::Data::Mesh(vao));
		m_MainScene->addAsset(m_StallMesh, mesh.as<Triton::Resource::Asset>());


		// Create texture
		Triton::PType::TextureCreateParams* tex_params = new Triton::PType::TextureCreateParams();
		Triton::Data::File::ReadTexture("D:/Programming/Test files/nfw/stallTexture.png", tex_params);

		auto texture = Context->factory->createTexture(tex_params).as<Triton::PType::Texture>();

		delete tex_params;

		Triton::PType::BitmapCreateParams* bitmap_params = new Triton::PType::BitmapCreateParams();

		bitmap_params->height = 100;
		bitmap_params->width = 100;
		bitmap_params->texture = texture;

		bitmap = Context->factory->createBitmap(bitmap_params).as<Triton::PType::Bitmap>();
		bitmap->setPosition(150, 150);

		image = Triton::reference<Triton::Data::Image>(new Triton::Data::Image(bitmap));
		m_MainScene->addAsset(m_BitmapID, image.as<Triton::Resource::Asset>());

		delete bitmap_params;

		// Create material
		mat = Triton::reference<Triton::Data::Material>(new Triton::Data::Material(texture));
		mat->Shader = Shader;
		mat->Ambient = Triton::Vector3(0.5f, 0.5f, 0.5f);
		mat->Shininess = 32;
		m_MainScene->addAsset(m_StallMaterial, mat.as<Triton::Resource::Asset>());

		m_MainScene->m_Camera->Position = Triton::Vector3(0.0f, 0.0f, -5.0f);		
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


		for (int i = 0; i < 1; i++)
		{
			Ent = m_MainScene->Entities->create();
		
			//m_MainScene->Entities->assign<Triton::Components::Transform>(Ent).Position = Triton::Vector3(0.0, 0.0, -25.0);
			auto& transform = m_MainScene->Entities->assign<Triton::Components::Transform>(Ent);

			transform.Position = Triton::Vector3(0.0, 0.0, 25.0);
			//transform.Scale = Triton::Vector3(10, 10, 10);
			transform.Rotation = Triton::Vector3(0.0, 180.0, 0.0);
		
			m_MainScene->Entities->assign<Triton::Components::Visual>(Ent, m_StallMesh, m_StallMaterial);
		}

		for (int i = 0; i < 1; i++)
		{
			uint32_t Ent2 = m_MainScene->Entities->create();

			//m_MainScene->Entities->assign<Triton::Components::Transform>(Ent).Position = Triton::Vector3(0.0, 0.0, -25.0);
			auto& transform = m_MainScene->Entities->assign<Triton::Components::Transform>(Ent2);

			transform.Position = Triton::Vector3(0.0, 0.0, 15.0);

			m_MainScene->Entities->assign<Triton::Components::Image>(Ent2, m_BitmapID);
		}
		
		//auto Ent1 = prtc_Scene->Registry()->create();
		//prtc_Scene->Registry()->assign<Triton::Components::LightEmitter>(Ent1, m_Light1);
		//
		//auto Ent2 = prtc_Scene->Registry()->create();
		//prtc_Scene->Registry()->assign<Triton::Components::LightEmitter>(Ent2, m_Light2);
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
		const Triton::MouseMovedEvent& mme = dynamic_cast<const Triton::MouseMovedEvent&>(event);

		double xpos = mme.GetX();
		double ypos = mme.GetY();

		if (m_firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			m_firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
		lastX = xpos;
		lastY = ypos;

		float sensitivity = 0.1f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		m_MainScene->m_Camera->Yaw += xoffset;
		m_MainScene->m_Camera->Pitch += yoffset;

		return false;
	}

	virtual void Render() override
	{
		m_viewPort->enable();
		m_viewPort->clear(1.0f, 0.0f, 1.0f, 0.0f);

		m_MainScene->render();

		Context->renderer->default();

		Context->setViewPort(0, 0, 1280, 720);

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