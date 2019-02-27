#define TR_SCRIPTING_ENABLED
#include <TritonShell.h>

#include <string>

//class Sandbox : public Triton::ShellApplication, private Triton::EventListener
class Sandbox : public Triton::ShellApplication
{
	std::shared_ptr<Triton::Data::Mesh> gv_Mesh = std::make_shared<Triton::Data::Mesh>();
private:
	void CreateMesh()
	{
		Triton::Data::MeshData mData;
		mData.Fill("D:/Programming/Test files/nfw/stall.obj");
		CubeMesh = Triton::Data::Mesh::Create(mData);
		//STORE MESH
		py_ChangeResource<std::shared_ptr<Triton::Data::Mesh>>("CubeMesh", gv_Mesh);
	}
public:
	Sandbox()
	{
		this->Listen<Triton::KeyPressedEvent>(Triton::EventBehavior(std::bind(&Sandbox::keyPressed, this, std::placeholders::_1)));
		this->Listen<Triton::KeyReleasedEvent>(Triton::EventBehavior(std::bind(&Sandbox::keyReleased, this, std::placeholders::_1)));
		this->Listen<Triton::MouseMovedEvent>(Triton::EventBehavior(std::bind(&Sandbox::mouseMoved, this, std::placeholders::_1)));
		prtc_Display->SetVSync(true);
	}

	~Sandbox()
	{

	}

	void PreExecutionSetup() override
	{
		CreateMesh();

		prtc_Camera = std::make_shared<Triton::Camera>(Triton::Vector3(0.0f, 16.0f, 45.0f));
		prtc_Camera->Yaw = -90;
		prtc_Camera->Pitch = 0;

		Triton::Data::TextureData mTData;
		//mTData.Fill("D:/Programming/Test files/nfw/missingTexture64.png");

		TestTexture = Triton::Data::Texture::Create(mTData);
		TestMaterial = std::make_shared<Triton::Data::Material>(TestTexture);
		TestMaterial->SetDiffuse(Triton::Vector3(1.0f, 1.0f, 1.0f));

		TestModel = prtc_EntityRegistry->create();
		prtc_EntityRegistry->assign<Triton::Components::Transform>(TestModel).Position = Triton::Vector3(0.0, 0.1, 1.0);
		prtc_EntityRegistry->assign<Triton::Components::MeshFilter>(TestModel).Mesh = CubeMesh;
		prtc_EntityRegistry->assign<Triton::Components::MeshRenderer>(TestModel).Material = TestMaterial;
	}

	void OnUpdate() override
	{
		TR_TRACE("YAW:{0} PITCH:{1}", prtc_Camera->Yaw, prtc_Camera->Pitch);
		TR_INFO("X:{0} Y:{1}", prtc_Camera->Position.x, prtc_Camera->Position.y);
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
			prtc_Camera->Position += cameraSpeed * prtc_Camera->GetViewDirection();
		if (keycode == (int)'S')
			prtc_Camera->Position -= cameraSpeed * prtc_Camera->GetViewDirection();
		if (keycode == (int)'A')
			prtc_Camera->Position -= glm::normalize(glm::cross(prtc_Camera->GetViewDirection(), Triton::Vector3(0.0f,1.0f,0.0f))) * cameraSpeed;
		if (keycode == (int)'D')
			prtc_Camera->Position += glm::normalize(glm::cross(prtc_Camera->GetViewDirection(), Triton::Vector3(0.0f, 1.0f, 0.0f))) * cameraSpeed;
		if (keycode == (int)'`')
		{
			try
			{
				this->RestartShell();
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
		
		prtc_Camera->Yaw += xoffset;
		prtc_Camera->Pitch += yoffset;

		return true;
	}
private:
	unsigned int ActiveCamera = 0;

	Triton::ECS::Entity TestModel;

	std::shared_ptr<Triton::Data::Mesh> CubeMesh;
	std::shared_ptr<Triton::Data::Texture> TestTexture;
	std::shared_ptr<Triton::Data::Material> TestMaterial;

	bool m_AlternateAction = false;

	bool m_firstMouse = true;

	double lastX;
	double lastY;
};

#include <iterator>
#include <vector>
class UnitTest1 : public Triton::ShellApplication
{
public:
	UnitTest1()
	{
		this->Listen<Triton::KeyPressedEvent>(Triton::EventBehavior(std::bind(&UnitTest1::KeyDown, this, std::placeholders::_1)));
	}

	bool KeyDown(const Triton::Event& event)
	{
		const Triton::KeyPressedEvent& kpe = dynamic_cast<const Triton::KeyPressedEvent&>(event);

		if (kpe.GetKeyCode() == (int)'`')
		{
			RestartShell();
		}

		return true;
	}

	void PreExecutionSetup() override
	{
		TestEntity = prtc_EntityRegistry->create();

		//float vertices[] = {
		//	0.5f,  0.5f, 0.0f,  // top right
		//	 0.5f, -0.5f, 0.0f,  // bottom right
		//	-0.5f, -0.5f, 0.0f,  // bottom left
		//	-0.5f,  0.5f, 0.0f   // top left 
		//};
		////float vertices[] = {
		////	-0.5f, -0.5f, 0.0f,
		////	 0.5f, -0.5f, 0.0f,
		////	 0.0f,  0.5f, 0.0f
		////};
		//
		//unsigned int indices[] = {  // note that we start from 0!
		//	0, 1, 3,   // first triangle
		//	1, 2, 3    // second triangle
		//};
		//
		//float uvs[] = {
		//	1.0f, 1.0f,   // top right
		//	1.0f, 0.0f,   // bottom right
		//	0.0f, 0.0f,   // bottom left
		//	0.0f, 1.0f
		//};
		

		
		//Triton::Data::MeshData mData;
		//mData.Fill("D:/Programming/Test files/nfw/stall.obj");
		//Mesh = Triton::Data::Mesh::Create(mData);
		//
		//Triton::Data::TextureData tData;
		//tData.Fill("D:/Programming/Test files/nfw/stallTexture.png");
		//Material = std::make_shared<Triton::Data::Material>(Triton::Data::Texture::Create(tData));
		//
		//prtc_EntityRegistry->assign<Triton::Components::Transform>(TestEntity).Position = glm::vec3(0.0f, 0.0f, -15.0f);
		//prtc_EntityRegistry->assign<Triton::Components::MeshFilter>(TestEntity).Mesh = Mesh;
		//prtc_EntityRegistry->assign<Triton::Components::MeshRenderer>(TestEntity).Material = Material;


	}

	void OnUpdate() override
	{
		//prtc_EntityRegistry->get<Triton::Components::Transform>(TestEntity).Rotation.z += 1.0f;
		//prtc_EntityRegistry->get<Triton::Components::Transform>(TestEntity).Rotation.y += 1.0f;
		//prtc_EntityRegistry->get<Triton::Components::Transform>(TestEntity).Rotation.x += 1.0f;
	}
private:
	Triton::ECS::Entity TestEntity;

	std::shared_ptr<Triton::Data::Mesh> Mesh;
	std::shared_ptr<Triton::Data::Material> Material;
};

Triton::Application* Triton::CreateApplication()
{
	return new UnitTest1();
}