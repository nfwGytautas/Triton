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
		
		float sensitivity = 0.1f; // change this Get<>() to your liking
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

	std::shared_ptr<Triton::Data::Mesh> gv_Mesh = std::make_shared<Triton::Data::Mesh>();

	Triton::Vector3 Position;

	bool jumped = false;
private: //GAME FUNCTIONS
	bool AABB(const Triton::Vector4& recA, const Triton::Vector4& recB)
	{
		if (
			recA.x + recA.w >= recB.x &&
			recB.x + recB.w >= recA.x &&
			recA.y + recA.z >= recB.y &&
			recB.y + recB.z >= recA.y
			)
		{
			return true;
		}

		return false;
	}

	void Collision()
	{
		Triton::ECS::Entity Mario = 0;

		auto& culledEntities = prtc_FrustumCullingSystem->GetCulled();

		auto& marioTransform = prtc_EntityRegistry->get<Triton::Components::Transform>(Mario);

		Triton::Vector4 marioAABB = Triton::Vector4(marioTransform.Position.x, marioTransform.Position.y, 2.0f, 2.0f);

		bool coll = false;
		//Check collision with enviroment
		for(Triton::ECS::Entity entity : culledEntities)
		{
			if (entity == 0)
				continue;

			auto& enviromentTransform = prtc_EntityRegistry->get<Triton::Components::Transform>(entity);

			Triton::Vector4 entityAABB = Triton::Vector4(enviromentTransform.Position.x, enviromentTransform.Position.y, 2.0f, 2.0f);

			if (AABB(marioAABB, entityAABB))
			{
				coll = true;
				jumped = false;
				if (prtc_PyVariableHandler->GetVariable("JumpPower").cast<float>() > 0.0f)
				{
					prtc_PyVariableHandler->ChangeVariable("JumpPower", py::cast(0.0f));
				}
				marioTransform.Position = Position;
				break;
			}
		}
	}

	void CreateMesh()
	{
		Triton::Data::MeshData mData;
		mData.Vertices = CubeVertices;
		mData.UVs = CubeColors;
		gv_Mesh = Triton::Data::Mesh::Create(mData);
		//STORE MESH
		prtc_PyVariableHandler->ChangeResource<std::shared_ptr<Triton::Data::Mesh>>("CubeMesh", gv_Mesh);
	}
	void CreateObjects()
	{
		CreateMesh();
	}

	void FocusCamera()
	{
		Triton::ECS::Entity Mario = 0;
		auto& marioTransform = prtc_EntityRegistry->get<Triton::Components::Transform>(Mario);

		if(marioTransform.Position.y > -10.0f && marioTransform.Position.x > -0.5f)
		{
			prtc_Camera->Position.x = marioTransform.Position.x;
		}
	}

	void HandleMovement()
	{
		Triton::ECS::Entity Mario = 0;
		auto& marioTransform = prtc_EntityRegistry->get<Triton::Components::Transform>(Mario);

		if (prtc_PyVariableHandler->GetVariable("Jump").cast<bool>())
		{
			prtc_PyVariableHandler->ChangeVariable("JumpPower", py::cast(3.6f));
			prtc_PyVariableHandler->ChangeVariable("Jump", py::cast(false));
			jumped = true;
		}

		if (prtc_PyVariableHandler->GetVariable("MoveLeft").cast<bool>() && marioTransform.Position.x > -20.0f)
		{
			marioTransform.Position.x += -8.0f * prtc_Delta;
		}

		if (prtc_PyVariableHandler->GetVariable("MoveRight").cast<bool>())
		{
			marioTransform.Position.x += 8.0f * prtc_Delta;
		}
	}
public:
	MarioTest()
	{
		this->Listen<Triton::KeyPressedEvent>(Triton::EventBehavior(std::bind(&MarioTest::keyPressed, this, std::placeholders::_1)));
		this->Listen<Triton::KeyReleasedEvent>(Triton::EventBehavior(std::bind(&MarioTest::keyReleased, this, std::placeholders::_1)));
		prtc_Display->SetVSync(true);
	}

	~MarioTest()
	{
	}

	void PreExecutionSetup() override
	{
		CreateObjects();

		TR_PYTHON_SCRIPT_GUARD(TR_SCRIPTING_SETUP(prtc_EntityRegistry.get()));

		prtc_Camera = std::make_shared<Triton::Camera>(Triton::Vector3(0.0f, 16.0f, 45.0f));
		prtc_Camera->Yaw = -90;
		prtc_Camera->Pitch = 0;

		prtc_FrustumCullingSystem->SetCamera(prtc_Camera);

		prtc_PyHandler->ImportTritonModule(
			pModule, 
			Triton::Scripting::TritonImport::Math | 
			Triton::Scripting::TritonImport::Data |
			Triton::Scripting::TritonImport::Components |
			Triton::Scripting::TritonImport::TritonCore);
	}

	void OnUpdate() override
	{
		TR_STANDARD_SYSTEMS_UPDATE(*prtc_EntityRegistry.get(), prtc_Delta);

		Position = Triton::Vector3(prtc_EntityRegistry->get<Triton::Components::Transform>(0).Position);
		TR_PYTHON_SCRIPT_GUARD(TR_SCRIPTING_UPDATE(prtc_EntityRegistry.get(), prtc_Delta));
		Collision();

		FocusCamera();
		HandleMovement();
		Collision();
	}

private:
	bool keyPressed(const Triton::Event& event)
	{
		const Triton::KeyPressedEvent& kpe = dynamic_cast<const Triton::KeyPressedEvent&>(event);
		int keycode = kpe.GetKeyCode();

		if (keycode == (int)'W' && !jumped)
		{
			prtc_PyVariableHandler->ChangeVariable("Jump", py::cast(true));
		}
		if (keycode == (int)'A')
		{
			prtc_PyVariableHandler->ChangeVariable("MoveLeft", py::cast(true));
		}
		if (keycode == (int)'D')
		{
			prtc_PyVariableHandler->ChangeVariable("MoveRight", py::cast(true));
		}
		if (keycode == (int)'`')
		{
			try
			{
				this->Restart();
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
			prtc_PyVariableHandler->ChangeVariable("MoveLeft", py::cast(false));
		}
		if (keycode == (int)'D')
		{
			prtc_PyVariableHandler->ChangeVariable("MoveRight", py::cast(false));
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