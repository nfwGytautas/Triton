#include <Triton.h>
#include <string>

Triton::ECS::Entity Cubes[3][3][3];
Triton::Vector3 Rotations[3][3][3];

void HorizontalRotation(std::unique_ptr<Triton::ECS::Registry>& aRegistry, unsigned int camera, float value)
{
	Triton::Vector3 singleLineRotations[8];

	for (int z = 0; z < 3; z++)
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				Rotations[z][i][j] = aRegistry->get<Triton::Components::Transform>(Cubes[z][i][j]).Rotation;
			}
		}
	}

	//Orange side
	if(camera == 1)
	{
		singleLineRotations[0] = Rotations[0][2][2];
		singleLineRotations[1] = Rotations[1][2][2];
		singleLineRotations[2] = Rotations[2][2][2];
		singleLineRotations[3] = Rotations[2][1][2];
		singleLineRotations[4] = Rotations[2][0][2];
		singleLineRotations[5] = Rotations[1][0][2];
		singleLineRotations[6] = Rotations[0][0][2];
		singleLineRotations[7] = Rotations[0][1][2];

		for (int i = 0; i < 8; i++)
		{
			singleLineRotations[i] += Triton::Vector3(0.0f, 0.0f, value);
		}

		Rotations[0][2][2] = singleLineRotations[6];
		Rotations[1][2][2] = singleLineRotations[7];
		Rotations[2][2][2] = singleLineRotations[0];
		Rotations[2][1][2] = singleLineRotations[1];
		Rotations[2][0][2] = singleLineRotations[2];
		Rotations[1][0][2] = singleLineRotations[3];
		Rotations[0][0][2] = singleLineRotations[4];
		Rotations[0][1][2] = singleLineRotations[5];
	}
	else if (camera == 2) //Blue side
	{
		singleLineRotations[0] = Rotations[0][2][0];
		singleLineRotations[1] = Rotations[0][2][1];
		singleLineRotations[2] = Rotations[0][2][2];
		singleLineRotations[3] = Rotations[0][1][2];
		singleLineRotations[4] = Rotations[0][0][2];
		singleLineRotations[5] = Rotations[0][0][1];
		singleLineRotations[6] = Rotations[0][0][0];
		singleLineRotations[7] = Rotations[0][1][0];

		for (int i = 0; i < 8; i++)
		{
			singleLineRotations[i] += Triton::Vector3(value, 0.0f, 0.0f);
		}

		Rotations[0][2][0] = singleLineRotations[6];
		Rotations[0][2][1] = singleLineRotations[7];
		Rotations[0][2][2] = singleLineRotations[0];
		Rotations[0][1][2] = singleLineRotations[1];
		Rotations[0][0][2] = singleLineRotations[2];
		Rotations[0][0][1] = singleLineRotations[3];
		Rotations[0][0][0] = singleLineRotations[4];
		Rotations[0][1][0] = singleLineRotations[5];
	}
	else if (camera == 3) //White side
	{
		singleLineRotations[0] = Rotations[2][2][0];
		singleLineRotations[1] = Rotations[2][2][1];
		singleLineRotations[2] = Rotations[2][2][2];
		singleLineRotations[3] = Rotations[1][2][2];
		singleLineRotations[4] = Rotations[0][2][2];
		singleLineRotations[5] = Rotations[0][2][1];
		singleLineRotations[6] = Rotations[0][2][0];
		singleLineRotations[7] = Rotations[1][2][0];

		for (int i = 0; i < 8; i++)
		{
			singleLineRotations[i] += Triton::Vector3(0.0f, value, 0.0f);
		}

		Rotations[2][2][0] = singleLineRotations[6];
		Rotations[2][2][1] = singleLineRotations[7];
		Rotations[2][2][2] = singleLineRotations[0];
		Rotations[1][2][2] = singleLineRotations[1];
		Rotations[0][2][2] = singleLineRotations[2];
		Rotations[0][2][1] = singleLineRotations[3];
		Rotations[0][2][0] = singleLineRotations[4];
		Rotations[1][2][0] = singleLineRotations[5];
	}
	else if (camera == 4) //Yellow side
	{
		singleLineRotations[0] = Rotations[2][2][0];
		singleLineRotations[1] = Rotations[2][2][1];
		singleLineRotations[2] = Rotations[2][2][2];
		singleLineRotations[3] = Rotations[2][1][2];
		singleLineRotations[4] = Rotations[2][0][2];
		singleLineRotations[5] = Rotations[2][0][1];
		singleLineRotations[6] = Rotations[2][0][0];
		singleLineRotations[7] = Rotations[2][1][0];

		for (int i = 0; i < 8; i++)
		{
			singleLineRotations[i] += Triton::Vector3(value, 0.0f, 0.0f);
		}

		Rotations[2][2][0] = singleLineRotations[6];
		Rotations[2][2][1] = singleLineRotations[7];
		Rotations[2][2][2] = singleLineRotations[0];
		Rotations[2][1][2] = singleLineRotations[1];
		Rotations[2][0][2] = singleLineRotations[2];
		Rotations[2][0][1] = singleLineRotations[3];
		Rotations[2][0][0] = singleLineRotations[4];
		Rotations[2][1][0] = singleLineRotations[5];
	}
	else if (camera == 5) //Red sides
	{
		singleLineRotations[0] = Rotations[0][2][0];
		singleLineRotations[1] = Rotations[1][2][0];
		singleLineRotations[2] = Rotations[2][2][0];
		singleLineRotations[3] = Rotations[2][1][0];
		singleLineRotations[4] = Rotations[2][0][0];
		singleLineRotations[5] = Rotations[1][0][0];
		singleLineRotations[6] = Rotations[0][0][0];
		singleLineRotations[7] = Rotations[0][1][0];

		for (int i = 0; i < 8; i++)
		{
			singleLineRotations[i] += Triton::Vector3(0.0f, 0.0f, value);
		}

		Rotations[0][2][0] = singleLineRotations[6];
		Rotations[1][2][0] = singleLineRotations[7];
		Rotations[2][2][0] = singleLineRotations[0];
		Rotations[2][1][0] = singleLineRotations[1];
		Rotations[2][0][0] = singleLineRotations[2];
		Rotations[1][0][0] = singleLineRotations[3];
		Rotations[0][0][0] = singleLineRotations[4];
		Rotations[0][1][0] = singleLineRotations[5];
	}
	else if (camera == 6) //Green side
	{
		singleLineRotations[0] = Rotations[2][0][0];
		singleLineRotations[1] = Rotations[2][0][1];
		singleLineRotations[2] = Rotations[2][0][2];
		singleLineRotations[3] = Rotations[1][0][2];
		singleLineRotations[4] = Rotations[0][0][2];
		singleLineRotations[5] = Rotations[0][0][1];
		singleLineRotations[6] = Rotations[0][0][0];
		singleLineRotations[7] = Rotations[1][0][0];

		for (int i = 0; i < 8; i++)
		{
			singleLineRotations[i] += Triton::Vector3(0.0f, value, 0.0f);
		}

		Rotations[2][0][0] = singleLineRotations[6];
		Rotations[2][0][1] = singleLineRotations[7];
		Rotations[2][0][2] = singleLineRotations[0];
		Rotations[1][0][2] = singleLineRotations[1];
		Rotations[0][0][2] = singleLineRotations[2];
		Rotations[0][0][1] = singleLineRotations[3];
		Rotations[0][0][0] = singleLineRotations[4];
		Rotations[1][0][0] = singleLineRotations[5];
	}

	for (int z = 0; z < 3; z++)
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				aRegistry->get<Triton::Components::Transform>(Cubes[z][i][j]).Rotation = Rotations[z][i][j];
			}
		}
	}
}

class Sandbox : public Triton::Application , private Triton::EventListener
{
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

		CubeMesh = Triton::Data::Mesh::Create(*prtc_MainMeshStorage.get(), CubeVertices, CubeColors);
		createCubes();
	}

	void OnUpdate() override
	{
		m_Camera->OnUpdate();
		prtc_Shader->SetUniform("viewMatrix", m_Camera->ViewMatrix());
	}

private:
	void createCubes()
	{
		for (int z = 0; z < 3; z++)
		{
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					Cubes[z][i][j] = prtc_EntityRegistry->create();
					prtc_EntityRegistry->assign<Triton::Components::Transform>(Cubes[z][i][j]).Position = Triton::Vector3(1.0f * z, 1.0f * i, 1.0f * j);
					prtc_EntityRegistry->assign<Triton::Components::MeshFilter>(Cubes[z][i][j]).Meshes.push_back(CubeMesh);
				}
			}
		}
	}

	bool keyPressed(const Triton::Event& event)
	{
		const Triton::KeyPressedEvent& kpe = dynamic_cast<const Triton::KeyPressedEvent&>(event);

		int keycode = kpe.GetKeyCode();

		if (keycode == (int)(' '))
		{
			m_AlternateAction = true;
		}

		static int h = 0;

		if(m_AlternateAction)
		{
			if (keycode == (int)'O')
			{
				HorizontalRotation(prtc_EntityRegistry, 1, -90.0f);
			}

			if (keycode == (int)'B')
			{
				HorizontalRotation(prtc_EntityRegistry, 2, 90.0f);
			}

			if (keycode == (int)'W')
			{
				HorizontalRotation(prtc_EntityRegistry, 3, -90.0f);
			}

			if (keycode == (int)'Y')
			{
				HorizontalRotation(prtc_EntityRegistry, 4, -90.0f);
			}

			if (keycode == (int)'R')
			{
				HorizontalRotation(prtc_EntityRegistry, 5, 90.0f);
			}

			if (keycode == (int)'G')
			{
				HorizontalRotation(prtc_EntityRegistry, 6, 90.0f);
			}
		}
		else
		{
			float cameraSpeed = 2.5 * m_Delta;
			if (keycode == (int)'W')
				m_Camera->Position += cameraSpeed * m_Camera->GetViewDirection();
			if (keycode == (int)'S')
				m_Camera->Position -= cameraSpeed * m_Camera->GetViewDirection();
			if (keycode == (int)'A')
				m_Camera->Position -= glm::normalize(glm::cross(m_Camera->GetViewDirection(), Triton::Vector3(0.0f,1.0f,0.0f))) * cameraSpeed;
			if (keycode == (int)'D')
				m_Camera->Position += glm::normalize(glm::cross(m_Camera->GetViewDirection(), Triton::Vector3(0.0f, 1.0f, 0.0f))) * cameraSpeed;
		}

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

		TR_INFO("YAW: {0} PITCH: {1}", m_Camera->Yaw, m_Camera->Pitch);
		
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

	std::vector<float> CubeVertices = {
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f
	};
	std::vector<float> CubeColors =	{
		1.0f, 0.0f, 0.0f,//LEFT
		1.0f, 0.0f, 0.0f,//LEFT
		1.0f, 0.0f, 0.0f,//LEFT
		1.0f, 0.0f, 0.0f,//LEFT
		1.0f, 0.0f, 0.0f,//LEFT
		1.0f, 0.0f, 0.0f,//LEFT

		1.0f, 0.5f, 0.0f,//RIGHT
		1.0f, 0.5f, 0.0f,//RIGHT
		1.0f, 0.5f, 0.0f,//RIGHT
		1.0f, 0.5f, 0.0f,//RIGHT
		1.0f, 0.5f, 0.0f,//RIGHT
		1.0f, 0.5f, 0.0f,//RIGHT

		0.0f, 0.0f, 1.0f,//FRONT
		0.0f, 0.0f, 1.0f,//FRONT
		0.0f, 0.0f, 1.0f,//FRONT
		0.0f, 0.0f, 1.0f,//FRONT
		0.0f, 0.0f, 1.0f,//FRONT
		0.0f, 0.0f, 1.0f,//FRONT

		1.0f, 1.0f, 0.0f,//BACK
		1.0f, 1.0f, 0.0f,//BACK
		1.0f, 1.0f, 0.0f,//BACK
		1.0f, 1.0f, 0.0f,//BACK
		1.0f, 1.0f, 0.0f,//BACK
		1.0f, 1.0f, 0.0f,//BACK

		0.0f, 1.0f, 0.0f,//BOTTOM
		0.0f, 1.0f, 0.0f,//BOTTOM
		0.0f, 1.0f, 0.0f,//BOTTOM
		0.0f, 1.0f, 0.0f,//BOTTOM
		0.0f, 1.0f, 0.0f,//BOTTOM
		0.0f, 1.0f, 0.0f,//BOTTOM

		1.0f, 1.0f, 1.0f,//TOP
		1.0f, 1.0f, 1.0f,//TOP
		1.0f, 1.0f, 1.0f,//TOP
		1.0f, 1.0f, 1.0f,//TOP
		1.0f, 1.0f, 1.0f,//TOP
		1.0f, 1.0f, 1.0f //TOP
	};

	std::shared_ptr<Triton::Data::Mesh> CubeMesh;

	bool m_AlternateAction = false;

	bool m_firstMouse = true;

	double lastX;
	double lastY;
};

Triton::Application* Triton::CreateApplication()
{
	return new Sandbox();
}