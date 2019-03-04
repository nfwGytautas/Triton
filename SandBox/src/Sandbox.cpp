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
	Sandbox(const Triton::AppSettings& aSettings)
		: ShellApplication(aSettings)
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

class TestGUI : public Triton::UI::GUI, public Triton::EventListener
{
public:
	TestGUI(unsigned int aDisplayWidth, unsigned int aDisplayHeight)
		: GUI(aDisplayWidth, aDisplayHeight)
	{
		this->Listen<Triton::MouseButtonPressedEvent>(Triton::EventBehavior(TR_BIND_FUNC(TestGUI::OnMouseButtonPressedEvent, this, std::placeholders::_1)));
		this->Listen<Triton::MouseButtonReleasedEvent>(Triton::EventBehavior(TR_BIND_FUNC(TestGUI::OnMouseButtonReleasedEvent, this, std::placeholders::_1)));
		this->Listen<Triton::MouseMovedEvent>(Triton::EventBehavior(TR_BIND_FUNC(TestGUI::OnMouseMovedEvent, this, std::placeholders::_1)));
		this->Listen<Triton::MouseScrolledEvent>(Triton::EventBehavior(TR_BIND_FUNC(TestGUI::OnMouseScrolledEvent, this, std::placeholders::_1)));
		this->Listen<Triton::KeyPressedEvent>(Triton::EventBehavior(TR_BIND_FUNC(TestGUI::OnKeyPressedEvent, this, std::placeholders::_1)));
		this->Listen<Triton::KeyReleasedEvent>(Triton::EventBehavior(TR_BIND_FUNC(TestGUI::OnKeyReleasedEvent, this, std::placeholders::_1)));
		this->Listen<Triton::KeyInputEvent>(Triton::EventBehavior(TR_BIND_FUNC(TestGUI::OnKeyInputEvent, this, std::placeholders::_1)));
		this->Listen<Triton::WindowResizeEvent>(Triton::EventBehavior(TR_BIND_FUNC(TestGUI::OnWindowResizeEvent, this, std::placeholders::_1)));
	}

	void Visualize() override
	{
		bool show = true;
		ImGui::ShowDemoWindow(&show);

		//bool my_tool_active = true;
		//
		//ImGui::Begin("My First Tool", &my_tool_active, ImGuiWindowFlags_MenuBar);
		//if (ImGui::BeginMenuBar())
		//{
		//	if (ImGui::BeginMenu("File"))
		//	{
		//		if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
		//		if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
		//		if (ImGui::MenuItem("Close", "Ctrl+W")) { my_tool_active = false; }
		//		ImGui::EndMenu();
		//	}
		//	ImGui::EndMenuBar();
		//}
		//
		//// Plot some values
		//const float my_values[] = { 0.2f, 0.1f, 1.0f, 0.5f, 0.9f, 2.2f };
		//ImGui::PlotLines("Frame Times", my_values, IM_ARRAYSIZE(my_values));
		//
		//// Display contents in a scrolling region
		//ImGui::TextColored(ImVec4(1, 1, 0, 1), "Important Stuff");
		//ImGui::BeginChild("Scrolling");
		//for (int n = 0; n < 50; n++)
		//	ImGui::Text("%04d: Some text", n);
		//ImGui::EndChild();
		//ImGui::End();
	}

	void Update(float aDelta) override
	{
		IO().DeltaTime = aDelta;
	}

	bool OnMouseButtonPressedEvent(const Triton::Event& e)
	{
		TR_EVENT_CAST(mbpe, e, Triton::MouseButtonPressedEvent);

		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[mbpe.GetMouseButton()] = true;

		return false;
	}

	bool OnMouseButtonReleasedEvent(const Triton::Event& e)
	{
		TR_EVENT_CAST(mbre, e, Triton::MouseButtonReleasedEvent);

		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[mbre.GetMouseButton()] = false;

		return false;
	}

	bool OnMouseMovedEvent(const Triton::Event& e)
	{
		TR_EVENT_CAST(mme, e, Triton::MouseMovedEvent);

		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(mme.GetX(), mme.GetY());

		return false;
	}

	bool OnMouseScrolledEvent(const Triton::Event& e)
	{
		TR_EVENT_CAST(mse, e, Triton::MouseScrolledEvent);

		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += mse.GetXOffset();
		io.MouseWheel += mse.GetYOffset();

		return true;
	}

	bool OnKeyPressedEvent(const Triton::Event& e)
	{
		TR_EVENT_CAST(kpe, e, Triton::KeyPressedEvent);

		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[kpe.GetKeyCode()] = true;

		io.KeyCtrl = io.KeysDown[341] || io.KeysDown[345];
		io.KeyShift = io.KeysDown[340] || io.KeysDown[344];
		io.KeyAlt = io.KeysDown[342] || io.KeysDown[346];
		io.KeySuper = io.KeysDown[343] || io.KeysDown[347];
		return false;
	}

	bool OnKeyReleasedEvent(const Triton::Event& e)
	{
		TR_EVENT_CAST(kre, e, Triton::KeyReleasedEvent);

		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[kre.GetKeyCode()] = false;

		return false;
	}

	bool OnKeyInputEvent(const Triton::Event& e)
	{
		TR_EVENT_CAST(kie, e, Triton::KeyInputEvent);

		ImGuiIO& io = ImGui::GetIO();
		int keycode = kie.GetKeyCode();
		if (keycode > 0 && keycode < 0x10000)
			io.AddInputCharacter((unsigned short)keycode);

		return false;
	}

	bool OnWindowResizeEvent(const Triton::Event& e)
	{
		TR_EVENT_CAST(wre, e, Triton::WindowResizeEvent);

		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(wre.GetWidth(), wre.GetHeight());
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);

		return false;
	}

};

class UnitTest1 : public Triton::ShellApplication
{
public:
	UnitTest1(const Triton::AppSettings& aSettings)
		: ShellApplication(aSettings)
	{
		
	}

	void PreExecutionSetup() override
	{
		prtc_Display->ShowCursor(true);

		prtc_GUIS->AddGUI(std::make_shared<TestGUI>(prtc_Display->GetWidth(), prtc_Display->GetHeight()));
	}
};

Triton::Application* Triton::CreateApplication(const Triton::AppSettings& aSettings)
{
	return new UnitTest1(aSettings);
}