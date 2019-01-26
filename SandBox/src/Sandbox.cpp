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
		m_Camera = new Triton::Camera(Triton::Vector3(0.0f, 0.0f, 6.0f));
		m_Camera->Yaw = -90;
		m_Camera->Pitch = 0;

		Triton::Data::MeshData mData;
		mData.Vertices = CubeVertices;
		mData.UVs = CubeColors;

		Triton::Data::TextureData mTData;
		//mTData << std::string("D:\\Programming\\Test files\\nfw\\dragon.png");

		CubeMesh = Triton::Data::Mesh::Create(mData);
		TestTexture = Triton::Data::Texture::Create(mTData);
		TestMaterial = std::make_shared<Triton::Data::Material>(TestTexture);

		TestModel = prtc_EntityRegistry->create();
		prtc_EntityRegistry->assign<Triton::Components::Transform>(TestModel).Position = Triton::Vector3(1.0f, 1.0f, 1.0f);
		prtc_EntityRegistry->assign<Triton::Components::MeshFilter>(TestModel).Mesh = CubeMesh;
		prtc_EntityRegistry->assign<Triton::Components::MeshRenderer>(TestModel).Material = TestMaterial;
	}

	void OnUpdate() override
	{
		TR_STANDARD_SYSTEMS_UPDATE(*prtc_EntityRegistry.get(), 0);

		TR_TRACE("YAW:{0} PITCH:{1}", m_Camera->Yaw, m_Camera->Pitch);

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

		float cameraSpeed = 2.5 * prtc_Delta;
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
	-1.000000, -1.000000,  1.000000,
	-1.000000,  1.000000,  1.000000,
	 1.000000, -1.000000,  1.000000,
	 1.000000, -1.000000,  1.000000,
	 1.000000,  1.000000,  1.000000,
	 -1.000000, 1.000000,  1.000000
	};
	std::vector<float> CubeColors = {
		0.000000, 0.000000,
		0.000000, 1.000000,
		1.000000, 0.000000,
		1.000000, 0.000000,
		1.000000, 1.000000,
		0.000000, 1.000000
	};

	std::shared_ptr<Triton::Data::Mesh> CubeMesh;
	std::shared_ptr<Triton::Data::Texture> TestTexture;
	std::shared_ptr<Triton::Data::Material> TestMaterial;

	bool m_AlternateAction = false;

	bool m_firstMouse = true;

	double lastX;
	double lastY;
};

class MarioTest : public Triton::Application, private Triton::EventListener
{
	TR_INCLUDE_STANDARD_SYSTEMS

private: //GAME VARIABLE
	float gv_JumpPower = -0.5f;
	bool gv_NotFalling = true;

	bool gv_Jump = false;
	bool gv_MoveLeft = false;
	bool gv_MoveRight = false;

	Triton::ECS::Entity Mario;
	std::vector<Triton::ECS::Entity> Ground;
private: //GAME FUNCTIONS
	void Jump()
	{
		if (gv_JumpPower >= -0.5f)
			gv_JumpPower -= 0.01f * prtc_Delta;

		auto& marioTransform = prtc_EntityRegistry->get<Triton::Components::Transform>(Mario);

		marioTransform.Position.y += gv_JumpPower * prtc_Delta;
	}

	void CreateObjects()
	{
		//Create data struct
		Triton::Data::MeshData mData;
		mData.Vertices = CubeVertices;
		mData.UVs = CubeColors;

		Triton::Data::TextureData mTData;
		Triton::Data::TextureData marioTData;
		marioTData << std::string("D:\\Programming\\Test files\\mario\\Mario\\marioTest.png");

		//Create Mesh, Material & Texture variables
		CubeMesh = Triton::Data::Mesh::Create(mData);

		TestTexture = Triton::Data::Texture::Create(mTData);
		TestMaterial = std::make_shared<Triton::Data::Material>(TestTexture);
		TestMaterial->SetDiffuse(Triton::Vector3(1.0f, 0.0f, 0.0f));

		MarioTexture = Triton::Data::Texture::Create(marioTData);
		MarioMaterial = std::make_shared<Triton::Data::Material>(MarioTexture);
		MarioMaterial->SetDiffuse(Triton::Vector3(0.0f, 0.0f, 0.0f));

		//Create Mario Entity
		Mario = prtc_EntityRegistry->create();
		prtc_EntityRegistry->assign<Triton::Components::Transform>(Mario).Position = Triton::Vector3(0.0f, 0.0f, 1.0f);
		prtc_EntityRegistry->assign<Triton::Components::MeshFilter>(Mario).Mesh = CubeMesh;
		prtc_EntityRegistry->assign<Triton::Components::MeshRenderer>(Mario).Material = MarioMaterial;

		//Create the ground
		RANGED_FOR(10, Ground.push_back(prtc_EntityRegistry->create()); );
		
		for (int i = 0; i < 10; i++)
		{
			prtc_EntityRegistry->assign<Triton::Components::Transform>(Ground[i]).Position = Triton::Vector3(-5.0f + (2.0f * i), -2.5f, 1.0f);
			prtc_EntityRegistry->assign<Triton::Components::MeshFilter>(Ground[i]).Mesh = CubeMesh;
			prtc_EntityRegistry->assign<Triton::Components::MeshRenderer>(Ground[i]).Material = TestMaterial;
		}
	}

	void CheckCollision()
	{
		auto& marioTransform = prtc_EntityRegistry->get<Triton::Components::Transform>(Mario);

		//if (marioTransform.Position.y < -0.5f)
		//{
		//	marioTransform.Position.y = -0.5f;
		//}

		//Check collision with ground
		gv_NotFalling = false;
		for (Triton::ECS::Entity groundObject : Ground)
		{
			auto& groundTransform = prtc_EntityRegistry->get<Triton::Components::Transform>(groundObject);
			if (marioTransform.Position.y < groundTransform.Position.y + 2)
			{
				Triton::Vector2 marioRightPoint = Triton::Vector2(marioTransform.Position.x + 1.0f, marioTransform.Position.y - 1.0f);
				Triton::Vector2 marioLeftPoint = Triton::Vector2(marioTransform.Position.x - 1.0f, marioTransform.Position.y - 1.0f);

				Triton::Vector2 groundRightPoint = Triton::Vector2(groundTransform.Position.x + 1.0f, groundTransform.Position.y + 0.9f);
				Triton::Vector2 groundLeftPoint = Triton::Vector2(groundTransform.Position.x - 1.0f, groundTransform.Position.y + 0.9f);

				if(!(marioRightPoint.x < groundLeftPoint.x || marioLeftPoint.x > groundRightPoint.x) 
					&& !(marioLeftPoint.y <= groundRightPoint.y))
				{
					marioTransform.Position.y = groundTransform.Position.y + 2;
					gv_NotFalling = true;
				}
			}
		}
	}

	void FocusCamera()
	{
		auto& marioTransform = prtc_EntityRegistry->get<Triton::Components::Transform>(Mario);

		if(marioTransform.Position.y > -10.0f)
		{
			m_Camera->Position.x = marioTransform.Position.x;
			m_Camera->Position.y = marioTransform.Position.y;
		}
	}

	void HandleMovement()
	{
		auto& marioTransform = prtc_EntityRegistry->get<Triton::Components::Transform>(Mario);

		if (gv_Jump)
		{
			gv_JumpPower = 0.5f;
			gv_NotFalling = true;
			gv_Jump = false;
		}

		if (gv_MoveLeft)
		{
			marioTransform.Position.x += -0.2f * prtc_Delta;
		}

		if (gv_MoveRight)
		{
			marioTransform.Position.x += 0.2f * prtc_Delta;
		}
	}
public:
	MarioTest()
	{
		this->Listen<Triton::KeyPressedEvent>(Triton::EventBehavior(std::bind(&MarioTest::keyPressed, this, std::placeholders::_1)));
		this->Listen<Triton::KeyReleasedEvent>(Triton::EventBehavior(std::bind(&MarioTest::keyReleased, this, std::placeholders::_1)));
	}

	~MarioTest()
	{
		delete m_Camera;
	}

	void PreExecutionSetup() override
	{
		m_Camera = new Triton::Camera(Triton::Vector3(0.0f, 0.0f, 45.0f));
		m_Camera->Yaw = -90;
		m_Camera->Pitch = 0;

		CreateObjects();
	}

	void OnUpdate() override
	{
		TR_STANDARD_SYSTEMS_UPDATE(*prtc_EntityRegistry.get(), 0);
		
		FocusCamera();

		m_Camera->OnUpdate();
		prtc_Shader->SetUniform("viewMatrix", m_Camera->ViewMatrix());
		prtc_v_RenderBatch = prtc_BatchSystem->GetBatches();

		HandleMovement();
		Jump();
		CheckCollision();
	}

private:
	bool keyPressed(const Triton::Event& event)
	{
		const Triton::KeyPressedEvent& kpe = dynamic_cast<const Triton::KeyPressedEvent&>(event);
		int keycode = kpe.GetKeyCode();

		if (keycode == (int)'W' && gv_NotFalling)
		{
			gv_Jump = true;
		}
		if (keycode == (int)'A')
		{
			gv_MoveLeft = true;
		}
		if (keycode == (int)'D')
		{
			gv_MoveRight = true;
		}

		return true;
	}
	bool keyReleased(const Triton::Event& event)
	{
		const Triton::KeyReleasedEvent& kre = dynamic_cast<const Triton::KeyReleasedEvent&>(event);
		int keycode = kre.GetKeyCode();

		if (keycode == (int)'A')
		{
			gv_MoveLeft = false;
		}
		if (keycode == (int)'D')
		{
			gv_MoveRight = false;
		}

		return true;
	}

private: //DATA
	Triton::Camera* m_Camera;
	unsigned int ActiveCamera = 0;

	std::vector<float> CubeVertices = {
		-1.000000, -1.000000,  1.000000,
		-1.000000,  1.000000,  1.000000,
		 1.000000, -1.000000,  1.000000,
		 1.000000, -1.000000,  1.000000,
		 1.000000,  1.000000,  1.000000,
		 -1.000000, 1.000000,  1.000000
	};
	std::vector<float> CubeColors = {
		0.000000, 0.000000,
		0.000000, 1.000000,
		1.000000, 0.000000,
		1.000000, 0.000000,
		1.000000, 1.000000,
		0.000000, 1.000000
	};

	std::shared_ptr<Triton::Data::Mesh> CubeMesh;

	std::shared_ptr<Triton::Data::Texture> TestTexture;
	std::shared_ptr<Triton::Data::Material> TestMaterial;

	std::shared_ptr<Triton::Data::Texture> MarioTexture;
	std::shared_ptr<Triton::Data::Material> MarioMaterial;
};

Triton::Application* Triton::CreateApplication()
{
	return new MarioTest();
}