#define TR_SCRIPTING_ENABLED
#define TR_SCRIPTING_LANG_PYTHON
#include <TritonScript.h>
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
	TR_INCLUDE_SCRIPTING
private: //GAME STRUCTURES

	struct Pipe
	{
		float Height;
		std::vector<Triton::ECS::Entity> Entities;
	};

private: //GAME VARIABLE

	Triton::PythonModule pModule = prtc_PyHandler->CreateModule("testScript");

	TR_GAME_VARIABLE(Float, JumpPower);
	TR_GAME_VARIABLE(Bool, NotFalling);
	TR_GAME_VARIABLE(Bool, Jump);
	TR_GAME_VARIABLE(Bool, MoveLeft);
	TR_GAME_VARIABLE(Bool, MoveRight);

	TR_GAME_VARIABLE(Mesh, Mesh);

	Triton::ECS::Entity Mario;
	std::vector<Triton::ECS::Entity> Ground;
	std::vector<Triton::ECS::Entity> Enviroment;
	std::vector<Pipe> Pipes;
private: //GAME FUNCTIONS
	void CreateMesh()
	{
		Triton::Data::MeshData mData;
		mData.Vertices = CubeVertices;
		mData.UVs = CubeColors;
		CubeMesh = Triton::Data::Mesh::Create(mData);
		gv_Mesh->value = CubeMesh;
		prtc_AppState->Store("CubeMesh", gv_Mesh);
	}
	void CreateObjects()
	{
		CreateMesh();
	}

	void CheckCollision()
	{
		auto& culledEntities = prtc_FrustumCullingSystem->GetCulled();

		auto& marioTransform = prtc_EntityRegistry->get<Triton::Components::Transform>(Mario);

		//Check collision with ground
		gv_NotFalling->value = false;
		for (Triton::ECS::Entity groundObject : Ground)
		{
			if (std::find(culledEntities.begin(), culledEntities.end(), groundObject) != culledEntities.end())
			{
				auto& groundTransform = prtc_EntityRegistry->get<Triton::Components::Transform>(groundObject);
				if (marioTransform.Position.y < groundTransform.Position.y + 2)
				{
					Triton::Vector2 marioRightPoint = Triton::Vector2(marioTransform.Position.x + 1.0f, marioTransform.Position.y - 1.0f);
					Triton::Vector2 marioLeftPoint = Triton::Vector2(marioTransform.Position.x - 1.0f, marioTransform.Position.y + 1.0f);

					Triton::Vector2 groundRightPoint = Triton::Vector2(groundTransform.Position.x + 1.0f, groundTransform.Position.y - 1.0f);
					Triton::Vector2 groundLeftPoint = Triton::Vector2(groundTransform.Position.x - 1.0f, groundTransform.Position.y + 1.0f);

					if (marioLeftPoint.x < groundRightPoint.x && marioRightPoint.x > groundLeftPoint.x &&
						marioLeftPoint.y > groundRightPoint.y && marioRightPoint.y < groundLeftPoint.y)
					{
						marioTransform.Position.y = groundTransform.Position.y + 2;
						gv_NotFalling->value = true;
						break;
					}
				}
			}
		}

		//Check collision with enviroment
		for (Triton::ECS::Entity enviromentObject : Enviroment)
		{
			if (std::find(culledEntities.begin(), culledEntities.end(), enviromentObject) != culledEntities.end())
			{
				auto& enviromentTransform = prtc_EntityRegistry->get<Triton::Components::Transform>(enviromentObject);
				if (marioTransform.Position.y < enviromentTransform.Position.y + 2)
				{
					Triton::Vector2 marioRightPoint = Triton::Vector2(marioTransform.Position.x + 1.0f, marioTransform.Position.y - 1.0f);
					Triton::Vector2 marioLeftPoint = Triton::Vector2(marioTransform.Position.x - 1.0f, marioTransform.Position.y + 1.0f);

					Triton::Vector2 enviromentRightPoint = Triton::Vector2(enviromentTransform.Position.x + 1.0f, enviromentTransform.Position.y - 1.0f);
					Triton::Vector2 enviromentLeftPoint = Triton::Vector2(enviromentTransform.Position.x - 1.0f, enviromentTransform.Position.y + 1.0f);

					if (marioLeftPoint.x < enviromentRightPoint.x && marioRightPoint.x > enviromentLeftPoint.x &&
						marioLeftPoint.y > enviromentRightPoint.y && marioRightPoint.y < enviromentLeftPoint.y)
					{
						if (glm::abs(enviromentLeftPoint.y - marioRightPoint.y) < glm::abs(enviromentRightPoint.y - marioLeftPoint.y))
						{
							marioTransform.Position.y = enviromentTransform.Position.y + 2;
							gv_NotFalling->value = true;
						}
						else
						{
							marioTransform.Position.y = enviromentRightPoint.y - 1;
							gv_JumpPower->value = 0;
						}
						break;
					}
				}
			}
		}

		//Check collision with pipes
		for (Pipe pipe : Pipes)
		{
			for (Triton::ECS::Entity pipeObject : pipe.Entities)
			{
				if (std::find(culledEntities.begin(), culledEntities.end(), pipeObject) != culledEntities.end())
				{
					auto& pipeTransform = prtc_EntityRegistry->get<Triton::Components::Transform>(pipeObject);
					if (marioTransform.Position.y < pipeTransform.Position.y + 2)
					{
						Triton::Vector2 marioRightPoint = Triton::Vector2(marioTransform.Position.x + 1.0f, marioTransform.Position.y - 1.0f);
						Triton::Vector2 marioLeftPoint = Triton::Vector2(marioTransform.Position.x - 1.0f, marioTransform.Position.y + 1.0f);

						Triton::Vector2 pipeRightPoint = Triton::Vector2(pipeTransform.Position.x + 1.0f, pipeTransform.Position.y - 1.0f);
						Triton::Vector2 pipeLeftPoint = Triton::Vector2(pipeTransform.Position.x - 1.0f, pipeTransform.Position.y + 1.0f);

						if (marioLeftPoint.x < pipeRightPoint.x && marioRightPoint.x > pipeLeftPoint.x &&
							marioLeftPoint.y > pipeRightPoint.y && marioRightPoint.y < pipeLeftPoint.y)
						{
							if (glm::abs(pipeLeftPoint.y - marioRightPoint.y) < glm::abs(pipeRightPoint.y - marioLeftPoint.y))
							{
								marioTransform.Position.y = pipe.Height + 2;
								gv_NotFalling->value = true;
							}

							if (gv_MoveRight && marioRightPoint.y < pipe.Height)
							{
								if (marioRightPoint.x > pipeLeftPoint.x)
								{
									marioTransform.Position.x = pipeLeftPoint.x - 1;
								}
							}

							if (gv_MoveLeft && marioRightPoint.y < pipe.Height)
							{
								if (marioLeftPoint.x < pipeRightPoint.x)
								{
									marioTransform.Position.x = pipeRightPoint.x + 1;
								}
							}

							break;
						}
					}
				}
			}
		}
	}

	void FocusCamera()
	{
		auto& marioTransform = prtc_EntityRegistry->get<Triton::Components::Transform>(Mario);

		if(marioTransform.Position.y > -10.0f && marioTransform.Position.x > -0.5f)
		{
			prtc_Camera->Position.x = marioTransform.Position.x;
		}
	}

	void HandleMovement()
	{
		auto& marioTransform = prtc_EntityRegistry->get<Triton::Components::Transform>(Mario);

		if (gv_Jump->value)
		{
			gv_JumpPower->value = 0.73f;
			gv_NotFalling->value = true;
			gv_Jump->value = false;
		}

		if (gv_MoveLeft->value && marioTransform.Position.x > -20.0f)
		{
			marioTransform.Position.x += -0.2f * prtc_Delta;
		}

		if (gv_MoveRight->value)
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
	}

	void PreExecutionSetup() override
	{
		CreateObjects();
		gv_JumpPower->value = -0.5f;
		prtc_AppState->Store("JumpPower", gv_JumpPower);
		prtc_AppState->Store("NotFalling", gv_NotFalling);
		prtc_AppState->Store("Jump", gv_Jump);
		prtc_AppState->Store("MoveLeft", gv_MoveLeft);
		prtc_AppState->Store("MoveRight", gv_MoveRight);

		TR_PYTHON_SCRIPT_GUARD(TR_SCRIPTING_SETUP(prtc_EntityRegistry.get(), prtc_AppState.get()));

		TR_APPSTATE_TAKE_ASSIGN(prtc_AppState, "Ground", Ground, Triton::Storage::List_Storable, std::vector<unsigned int>);
		TR_APPSTATE_TAKE_ASSIGN(prtc_AppState, "Enviroment", Enviroment, Triton::Storage::List_Storable, std::vector<unsigned int>);
		TR_APPSTATE_TAKE_ASSIGN(prtc_AppState, "Mario", Mario, Triton::Storage::Entity_Storable, Triton::ECS::Entity);

		prtc_Camera = std::make_shared<Triton::Camera>(Triton::Vector3(0.0f, 16.0f, 45.0f));
		prtc_Camera->Yaw = -90;
		prtc_Camera->Pitch = 0;

		prtc_FrustumCullingSystem->SetCamera(prtc_Camera);

		prtc_PyHandler->ImportTritonModule(
			pModule, 
			Triton::Scripting::TritonImport::Math | 
			Triton::Scripting::TritonImport::Data |
			Triton::Scripting::TritonImport::Components |
			Triton::Scripting::TritonImport::Registry |
			Triton::Scripting::TritonImport::TritonCore);
	}

	void OnUpdate() override
	{
		TR_STANDARD_SYSTEMS_UPDATE(*prtc_EntityRegistry.get(), prtc_Delta);
		TR_PYTHON_SCRIPT_GUARD(TR_SCRIPTING_UPDATE(prtc_EntityRegistry.get(), prtc_AppState.get(), prtc_Delta));
		
		FocusCamera();

		HandleMovement();
		CheckCollision();
	}

private:
	bool keyPressed(const Triton::Event& event)
	{
		const Triton::KeyPressedEvent& kpe = dynamic_cast<const Triton::KeyPressedEvent&>(event);
		int keycode = kpe.GetKeyCode();

		if (keycode == (int)'W' && gv_NotFalling->value)
		{
			gv_Jump->value = true;
		}
		if (keycode == (int)'A')
		{
			gv_MoveLeft->value = true;
		}
		if (keycode == (int)'D')
		{
			gv_MoveRight->value = true;
		}
		if (keycode == (int)'`')
		{
			try
			{
				TR_PYTHON_SCRIPTING_RELOAD;
			}
			catch (const std::runtime_error &re) {
				OutputDebugStringA(re.what());
				::MessageBoxA(NULL, re.what(), "Error initializing sample", MB_OK);
			}
		}

		return true;
	}
	bool keyReleased(const Triton::Event& event)
	{
		const Triton::KeyReleasedEvent& kre = dynamic_cast<const Triton::KeyReleasedEvent&>(event);
		int keycode = kre.GetKeyCode();

		if (keycode == (int)'A')
		{
			gv_MoveLeft->value = false;
		}
		if (keycode == (int)'D')
		{
			gv_MoveRight->value = false;
		}

		return true;
	}

private: //DATA
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
};

Triton::Application* Triton::CreateApplication()
{
	return new MarioTest();
}