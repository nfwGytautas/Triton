#include <Triton.h>

#define UNIT_TEST

#include <string>

#ifdef UNIT_TEST

class UnitTest1 : public Triton::Application
{
	size_t m_StallMesh = 1;
	size_t m_StallMaterial = 2;

	Triton::Matrix44 m_Transformation;

	size_t m_Light1;
	size_t m_Light2;

	double lastX;
	double lastY;
	bool m_firstMouse = true;

	Triton::relay_ptr<Triton::Scene> m_MainScene;
	Triton::Data::Material* mat;
	Triton::Data::Mesh* mesh;
public:
	void CreateResources()
	{
		m_MainScene = SceneManager->createScene();

		// Create shader
		Triton::PType::ShaderCreateParams* shader_params = new Triton::PType::ShaderCreateParams();
		shader_params->vertexPath = "D:/Programming/Test files/nfw/shaders/triton/v4.shader";
		shader_params->fragmentPath = "D:/Programming/Test files/nfw/shaders/triton/fragment_lighting.shader";

		auto Shader = dynamic_cast<Triton::PType::Shader*>(Context->factory->createShader(shader_params));
		m_MainScene->shader = Shader;
		//m_Objects.push_back((Triton::PType::FactoryObject*)Shader);

		delete shader_params;


		Shader->enable();
		auto proj_mat = GetProjectionMatrix();
		Shader->setUniformMatrix44("projectionMatrix", proj_mat);


		// Create VAO
		Triton::PType::VAOCreateParams* vao_params = new Triton::PType::VAOCreateParams();
		Triton::Data::File::ReadMesh("D:/Programming/Test files/nfw/stall.obj", vao_params);

		auto vao = dynamic_cast<Triton::PType::VAO*>(Context->factory->createVAO(vao_params));

		delete vao_params;

		// Create mesh
		mesh = new Triton::Data::Mesh(vao);
		m_MainScene->addAsset(m_StallMesh, mesh);


		// Create texture
		Triton::PType::TextureCreateParams* tex_params = new Triton::PType::TextureCreateParams();
		Triton::Data::File::ReadTexture("D:/Programming/Test files/nfw/stallTexture.png", tex_params);

		auto texture = dynamic_cast<Triton::PType::Texture*>(Context->factory->createTexture(tex_params));

		delete tex_params;

		// Create material
		mat = new Triton::Data::Material(texture);
		mat->Shader = Shader;
		mat->Ambient = Triton::Vector3(0.5f, 0.5f, 0.5f);
		m_MainScene->addAsset(m_StallMaterial, mat);
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

		Context->window->showCursor(false);
		//prtc_Display->SetVSync(true);

		m_MainScene->addLight("pointlight", new Triton::Graphics::PointLight(Triton::Vector3(20.0, 0.0, -25.0)));		
		m_MainScene->addLight("spotlight", new Triton::Graphics::SpotLight(Triton::Vector3(0.0, 0.0, 0.0), Triton::Vector3(0.0f, 0.0f, -1.0f)));


		for (int i = 0; i < 10; i++)
		{
			auto Ent = m_MainScene->Entities->create();
		
			m_MainScene->Entities->assign<Triton::Components::Transform>(Ent).Position = Triton::Vector3(0.0, 0.0, -25.0);
		
			m_MainScene->Entities->assign<Triton::Components::Visual>(Ent, m_StallMesh, m_StallMaterial);
		}
		
		//auto Ent1 = prtc_Scene->Registry()->create();
		//prtc_Scene->Registry()->assign<Triton::Components::LightEmitter>(Ent1, m_Light1);
		//
		//auto Ent2 = prtc_Scene->Registry()->create();
		//prtc_Scene->Registry()->assign<Triton::Components::LightEmitter>(Ent2, m_Light2);
	}

	void OnUpdate() override
	{
		m_MainScene->Update(prtc_Delta);
		Triton::Impl::logErrors();
	}

	bool keyPressed(const Triton::Event& event)
	{
		const Triton::KeyPressedEvent& kpe = dynamic_cast<const Triton::KeyPressedEvent&>(event);

		int keycode = kpe.GetKeyCode();

		float cameraSpeed = 2.5 * prtc_Delta * 10;
		if (keycode == (int)'W')
			m_MainScene->m_Camera->Position += cameraSpeed * m_MainScene->m_Camera->GetViewDirection();
		if (keycode == (int)'S')
			m_MainScene->m_Camera->Position -= cameraSpeed * m_MainScene->m_Camera->GetViewDirection();
		if (keycode == (int)'A')
			m_MainScene->m_Camera->Position -= glm::normalize(glm::cross(m_MainScene->m_Camera->GetViewDirection(), Triton::Vector3(0.0f, 1.0f, 0.0f))) * cameraSpeed;
		if (keycode == (int)'D')
			m_MainScene->m_Camera->Position += glm::normalize(glm::cross(m_MainScene->m_Camera->GetViewDirection(), Triton::Vector3(0.0f, 1.0f, 0.0f))) * cameraSpeed;
		if (keycode == (int)'`')
		{
			try
			{
				this->Restart();
			}
			catch (const std::runtime_error &re) {
				OutputDebugStringA(re.what());
				::MessageBoxA(NULL, re.what(), "Error initializing sample", MB_OK);
			}
		}

		return true;
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

		return true;
	}

	virtual void Render() override
	{
		m_MainScene->render();
	}
};

#endif // UNIT_TEST


Triton::Application* Triton::CreateApplication(Triton::AppSettings& aSettings)
{
	aSettings.WindowWidth = 1280;
	aSettings.WindowHeight = 720;
	return new UnitTest1(aSettings);
}