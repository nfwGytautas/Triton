#include <Triton.h>
#include <string>

class Sandbox : public Triton::Application , private Triton::EventListener
{
	TR_INCLUDE_STANDARD_SYSTEMS
public:
	Sandbox()
	{
		this->Listen<Triton::KeyPressedEvent>(Triton::EventBehavior(std::bind(&Sandbox::keyPressed, this, std::placeholders::_1)));
		this->Listen<Triton::KeyReleasedEvent>(Triton::EventBehavior(std::bind(&Sandbox::keyReleased, this, std::placeholders::_1)));
		this->Listen<Triton::MouseMovedEvent>(Triton::EventBehavior(std::bind(&Sandbox::mouseMoved, this, std::placeholders::_1)));
	}

	~Sandbox()
	{
		delete m_Camera;
	}

	void PreExecutionSetup() override
	{
		m_Camera = new Triton::Camera(Triton::Vector3(0.0f, 6.0f, 6.0f));
		m_Camera->Yaw = -35;
		m_Camera->Pitch = -30;

		Triton::Data::MeshData mData;
		mData.Vertices = CubeVertices;
		mData.UVs = CubeColors;

		Triton::Data::TextureData mTData;
		mTData << std::string("D:\\Programming\\Test files\\nfw\\dragon.png");

		CubeMesh = Triton::Data::Mesh::Create(mData);
		TestTexture = Triton::Data::Texture::Create(mTData);

		TestModel = prtc_EntityRegistry->create();
		prtc_EntityRegistry->assign<Triton::Components::Transform>(TestModel).Position = Triton::Vector3(1.0f, 1.0f, 1.0f);
		prtc_EntityRegistry->assign<Triton::Components::MeshFilter>(TestModel).Mesh = CubeMesh;
		prtc_EntityRegistry->assign<Triton::Components::MeshRenderer>(TestModel).Material = TestTexture;
	}

	void OnUpdate() override
	{
		TR_STANDARD_SYSTEMS_UPDATE(*prtc_EntityRegistry.get(), 0);

		m_Camera->OnUpdate();
		prtc_Shader->SetUniform("viewMatrix", m_Camera->ViewMatrix());
		prtc_v_RenderBatch = prtc_BatchSystem->GetBatches();
	}

private:
	bool keyPressed(const Triton::Event& event)
	{
		const Triton::KeyPressedEvent& kpe = dynamic_cast<const Triton::KeyPressedEvent&>(event);

		int keycode = kpe.GetKeyCode();

		if (keycode == (int)(' '))
		{
			m_AlternateAction = true;
		}

		float cameraSpeed = 2.5 * m_Delta;
		if (keycode == (int)'W')
			m_Camera->Position += cameraSpeed * m_Camera->GetViewDirection();
		if (keycode == (int)'S')
			m_Camera->Position -= cameraSpeed * m_Camera->GetViewDirection();
		if (keycode == (int)'A')
			m_Camera->Position -= glm::normalize(glm::cross(m_Camera->GetViewDirection(), Triton::Vector3(0.0f,1.0f,0.0f))) * cameraSpeed;
		if (keycode == (int)'D')
			m_Camera->Position += glm::normalize(glm::cross(m_Camera->GetViewDirection(), Triton::Vector3(0.0f, 1.0f, 0.0f))) * cameraSpeed;

		return true;
	}
	bool keyReleased(const Triton::Event& event)
	{
		const Triton::KeyReleasedEvent& kre = dynamic_cast<const Triton::KeyReleasedEvent&>(event);

		int keycode = kre.GetKeyCode();

		if (keycode == (int)(' '))
		{
			m_AlternateAction = false;
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
		
		float sensitivity = 0.1f; // change this value to your liking
		xoffset *= sensitivity;
		yoffset *= sensitivity;
		
		m_Camera->Yaw += xoffset;
		m_Camera->Pitch += yoffset;

		return true;
	}
private:
	Triton::Camera* m_Camera;
	unsigned int ActiveCamera = 0;

	Triton::ECS::Entity TestModel;

	std::vector<float> CubeVertices = {
		 1.000000, -1.000000, -1.000000,
		 1.000000, -1.000000,  1.000000,
		-1.000000, -1.000000,  1.000000,
		-1.000000, -1.000000, -1.000000,
		 1.000000,  1.000000, -1.000000,
		 1.000000,  1.000000,  1.000000,
		-1.000000,  1.000000,  1.000000,
		-1.000000,  1.000000, -1.000000
	};
	std::vector<float> CubeColors =	{
		0.748573, 0.750412,
		0.749279, 0.501284,
		0.999110, 0.501077,
		0.999455, 0.750380,
		0.250471, 0.500702,
		0.249682, 0.749677,
		0.001085, 0.750380,
		0.001517, 0.499994,
		0.499422, 0.500239,
		0.500149, 0.750166,
		0.748355, 0.998230,
		0.500193, 0.998728,
		0.498993, 0.250415,
		0.748953, 0.250920
	};

	std::shared_ptr<Triton::Data::Mesh> CubeMesh;
	std::shared_ptr<Triton::Data::Texture> TestTexture;

	bool m_AlternateAction = false;

	bool m_firstMouse = true;

	double lastX;
	double lastY;
};

Triton::Application* Triton::CreateApplication()
{
	return new Sandbox();
}