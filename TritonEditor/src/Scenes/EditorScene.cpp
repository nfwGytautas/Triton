#include "EditorScene.h"
#include <string>

#include <imgui.h>
#include "Impl/imgui_impl_win32.h"
#include "Impl/imgui_impl_dx11.h"

#include "TritonPlatform/DirectX/DXSpecific.h"

#include "Widgets/DockSpace.h"
#include "Widgets/Viewport.h"
#include "Widgets/LogWindow.h"
#include "Widgets/Metrics.h"

#include "Triton/Utility/ClassGetters.h"

#include "Triton/Managers/AssetManager.h"

IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Triton::EditorScene::EditorScene()
	: SceneBase(), EventInterface()
{
	edtr_Camera = std::make_unique<Triton::Camera>(Vector3(0.0f, 0.0f, 0.0f));
}

Triton::EditorScene::~EditorScene()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void Triton::EditorScene::onMessage(size_t message, void * payload)
{
	switch (message)
	{
	case (size_t)Triton::Core::TritonMessageType::ClassRegistered:
	{
		TR_GET_CLASS(InputManager);
		TR_GET_CLASS(AssetManager);

		m_input = InputManager.as<Triton::Core::InputManager>();
		m_input->AddListener(*this);

		m_assetManager = AssetManager.as<Manager::AssetManager>();

		m_mainRenderBuffer = this->getClassByID((size_t)Core::TritonClasses::MainRenderBuffer).as<Triton::Core::RenderBuffer>();
		m_graphicsContext = this->getClassByID((size_t)Core::TritonClasses::Context).as<PType::Context>();

		onRegistered();

		loadResources();
		createEntities();

		return;
	}
	case (size_t)Triton::Core::TritonMessageType::Render:
	{
		onRender();

		return;
	}
	case (size_t)Triton::Core::TritonMessageType::Update:
	{
		onUpdate();

		return;
	}
	}
}

bool Triton::EditorScene::OnKeyPressed(int aKeycode, int aRepeatCount, int aScancode, int aMods)
{
	ImGuiIO& io = ImGui::GetIO();
	io.KeysDown[aKeycode] = true;

	io.KeyCtrl = (::GetKeyState(VK_CONTROL) & 0x8000) != 0;
	io.KeyShift = (::GetKeyState(VK_SHIFT) & 0x8000) != 0;
	io.KeyAlt = (::GetKeyState(VK_MENU) & 0x8000) != 0;
	io.KeySuper = false;
	return false;
}

bool Triton::EditorScene::OnKeyReleased(int aKeycode, int aScancode, int aMods)
{
	ImGuiIO& io = ImGui::GetIO();
	io.KeysDown[aKeycode] = false;
	return false;
}

bool Triton::EditorScene::OnKeyInput(unsigned int aCharInput)
{
	ImGuiIO& io = ImGui::GetIO();
	int keycode = aCharInput;
	if (keycode > 0 && keycode < 0x10000)
		io.AddInputCharacter((unsigned short)keycode);

	return false;
}

bool Triton::EditorScene::OnMouseMoved(double aX, double aY)
{
	ImGuiIO& io = ImGui::GetIO();
	io.MousePos = ImVec2(m_input->getMouse()->X, m_input->getMouse()->Y);

	return false;
}

bool Triton::EditorScene::OnMouseButtonPressed(int aKeycode)
{
	ImGuiIO& io = ImGui::GetIO();
	io.MouseDown[aKeycode] = true;

	return false;
}

bool Triton::EditorScene::OnMouseButtonReleased(int aKeycode)
{
	ImGuiIO& io = ImGui::GetIO();
	io.MouseDown[aKeycode] = false;

	return false;
}

bool Triton::EditorScene::OnMouseScrolled(double aXOffset, double aYOffset)
{
	ImGuiIO& io = ImGui::GetIO();
	io.MouseWheel += aXOffset;
	io.MouseWheelH += aYOffset;

	return true;
}

bool Triton::EditorScene::OnWindowResized(int aWidth, int aHeight)
{
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(aWidth, aHeight);
	io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);

	return false;
}

void Triton::EditorScene::onRegistered()
{
	ImGui_ImplWin32_EnableDpiAwareness();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	m_imguiIO = &ImGui::GetIO();
	ImGuiIO& io = *m_imguiIO;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Game pad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-View port / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;
	//io.ConfigViewportsNoDefaultParent = true;
	//io.ConfigDockingTabBarOnSingleWindows = true;
	io.ConfigDockingWithShift = false;
	io.ConfigDockingTransparentPayload = true;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// When view ports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	PType::DXPacket* packet = new PType::DXPacket();

	TR_GET_CLASS(Context);

	Context.as<PType::Context>()->fillPacket(packet);

	ImGui_ImplWin32_Init(packet->hwnd);
	ImGui_ImplDX11_Init(packet->device, packet->deviceContext);

	delete packet;

	m_gameWindow = this->getClassByName("gameWindow").as<GameWindow>();

	m_timer = this->getClassByName("timer").as<Utility::Timer>();
}

void Triton::EditorScene::onUpdate()
{
	if (m_imguiIO->KeyShift)
	{
		m_dockspace = true;
	}
	else
	{
		m_dockspace = false;
	}

	static int movingDir = 1;

	auto& transform = Entities->get<Triton::Components::Transform>(edtr_pointer_id);
	transform.Position.y += movingDir * m_timer->updateDelta() * 5;

	if (transform.Position.y > 5)
	{
		movingDir = -1;
	}
	else if (transform.Position.y < -5)
	{
		movingDir = 1;
	}

	//auto& transform_pointer = Entities->get<Triton::Components::Transform>(edtr_pointer_id);
	//transform_pointer.Position.y += 1 * m_timer->updateDelta();
}

void Triton::EditorScene::onRender()
{
	renderEntities();

	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	static bool test = true;

	ShowDockSpace(&m_dockspace);

	// View port
	ShowViewport(&m_viewport, m_gameWindow);

	// Log window
	showLogWindow(&m_logWindow);

	// Metrics window
	showMetrics(&m_metrics, m_timer->renderDelta(), m_timer->updateDelta());

	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	ImGui::ShowDemoWindow(&test);

	// Rendering
	ImGui::Render();

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	if (m_imguiIO->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}

void Triton::EditorScene::loadResources()
{
	// 3D pointer mesh
	Triton::Resource::AssetCreateParams asset_desc;
	asset_desc.Type = Triton::Resource::AssetCreateParams::AssetType::MESH;
	asset_desc.Paths[0] = "C:/dev/Triton/Models/3dpointer.obj";

	edtr_3DPOINTER = m_assetManager->createAsset(asset_desc).as<Triton::Data::Mesh>();

	// 3D pointer material
	asset_desc.Type = Triton::Resource::AssetCreateParams::AssetType::MATERIAL;
	asset_desc.Paths[0] = "C:/dev/Triton/Models/3dpointer.png";

	edtr_mat_3DPOINTER = m_assetManager->createAsset(asset_desc).as<Triton::Data::Material>();

	// 3D pointer shader
	asset_desc.Type = Triton::Resource::AssetCreateParams::AssetType::SHADER;
	asset_desc.Paths[0] = "C:/dev/Triton/Shaders/v_selector.hlsl";
	asset_desc.Paths[1] = "C:/dev/Triton/Shaders/f_selector.hlsl";

	asset_desc.Arguments[0] = "SelectorVertexShader";
	asset_desc.Arguments[1] = "SelectorPixelShader";

	edtr_mat_3DPOINTER->Shader = m_assetManager->createAsset(asset_desc).as<Triton::Data::ShaderProgram>();
}

void Triton::EditorScene::createEntities()
{
	edtr_pointer_id = Entities->create();

	auto& transform = Entities->assign<Triton::Components::Transform>(edtr_pointer_id);

	transform.Position = Triton::Vector3(0.0, 10.0, 25.0);
	transform.Scale = Triton::Vector3(5, 5, 5);
	transform.Rotation = Triton::Vector3(0.0, 0.0, 0.0);

	Entities->assign<Triton::Components::Visual>(edtr_pointer_id, edtr_3DPOINTER->getAssetID(), edtr_mat_3DPOINTER->getAssetID());
}

void Triton::EditorScene::renderEntities()
{
	// Bind/enable the view port
	m_mainRenderBuffer->addCommand<RCommands::PushViewport>(m_gameWindow->getViewport());

	Entities->view<Components::Transform, Components::Visual>().each([&](auto& transform, auto& visual) {

		// Get material used by the entity
		auto material = m_assetManager->getAssetByID(visual.Material).as<Triton::Data::Material>();

		// Push material
		m_mainRenderBuffer->addCommand<RCommands::PushMaterial>(material);

		// Update uniforms
		m_mainRenderBuffer->addCommand<RCommands::UpdateUniformValues>
			(
				[&](reference<PType::Shader> shader)
		{
			shader->setBufferValue("persistant_Persistant", "projectionMatrix", &m_graphicsContext->renderer->projection());
			shader->setBufferValue("frame_PerFrame", "viewMatrix", &edtr_Camera->ViewMatrix());
			shader->setBufferValue("CameraBuffer", "cameraPosition", &edtr_Camera->Position);

			shader->updateBuffers(PType::BufferUpdateType::FRAME);
			shader->updateBuffers(PType::BufferUpdateType::PERSISTANT);
		}
		);

		// Get mesh object
		reference<Data::Mesh>& mesh = m_assetManager->getAssetByID(visual.Mesh).as<Data::Mesh>();
		reference<PType::VAO>& vao = mesh->VAO;

		// Push mesh
		m_mainRenderBuffer->addCommand<RCommands::PushMesh>(mesh);

		// Update uniforms
		m_mainRenderBuffer->addCommand<RCommands::UpdateUniformValues>
			(
				[&](reference<PType::Shader> shader)
		{
			auto trans_mat = Triton::Core::CreateTransformationMatrix(transform.Position, transform.Rotation, transform.Scale);
			shader->setBufferValue("object_PerObject", "transformationMatrix", &trans_mat);

			shader->updateBuffers(PType::BufferUpdateType::OBJECT);
		}
		);

		// Push render command
		m_mainRenderBuffer->addCommand<RCommands::Draw>();
	});
}
