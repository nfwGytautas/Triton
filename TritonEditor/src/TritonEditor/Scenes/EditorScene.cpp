#include "EditorScene.h"
#include <string>

#include <imgui.h>

#include <TritonPlatform/DirectX/DXSpecific.h>
#include <Triton/Utility/ClassGetters.h>
#include <Triton/Managers/AssetManager.h>
#include <Triton/Core/Graphics/Lighting/DirectionalLight.h>

#include "TritonEditor/Impl/imgui_impl_win32.h"
#include "TritonEditor/Impl/imgui_impl_dx11.h"

#include "TritonEditor/Widgets/MainMenuBar.h"
#include "TritonEditor/Widgets/DockSpace.h"
#include "TritonEditor/Widgets/Viewport.h"
#include "TritonEditor/Widgets/LogWindow.h"
#include "TritonEditor/Widgets/Metrics.h"
#include "TritonEditor/Widgets/SceneView.h"
#include "TritonEditor/Widgets/AssetWindow.h"
#include "TritonEditor/Widgets/InspectorWindow.h"
#include "TritonEditor/Widgets/MaterialEditor.h"

#include "Triton/File/File.h"

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

void Triton::EditorScene::relayMessage(size_t message, void * payload)
{
	switch (message)
	{
	case (size_t)Triton::Core::TritonMessageType::ClassRegistered:
	{
		TR_GET_EDTR_STATE(m_edtr_state);

		TR_GET_CLASS(InputManager);
		TR_GET_CLASS(AssetManager);

		m_input = InputManager.as<Triton::Core::InputManager>();
		m_input->AddListener(*this);

		m_assetManager = AssetManager.as<Manager::AssetManager>();

		m_mainRenderBuffer = this->getClassByID((size_t)Core::TritonClasses::MainRenderBuffer).as<Triton::Core::RenderBuffer>();
		m_graphicsContext = this->getClassByID((size_t)Core::TritonClasses::Context).as<PType::Context>();
		m_graphicsContext->renderer->setVsync(false);

		m_scriptEngine = this->getClassByID((size_t)Core::TritonClasses::ScriptEngine).as<Script::ScriptEngine>();

		onRegistered();

		loadResources();
		createEntities();

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

	DragAcceptFiles(packet->hwnd, true);
	ImGui_ImplWin32_Init(packet->hwnd);
	ImGui_ImplDX11_Init(packet->device, packet->deviceContext);

	delete packet;

	m_gameWindow = this->getClassByName("gameWindow").as<GameWindow>();

	m_timer = this->getClassByName("timer").as<Utility::Timer>();
}

void Triton::EditorScene::onUpdate()
{
	// Transform the pointer to be on the selected entity
	auto object = m_edtr_state->CurrentGameObject;

	if(object.valid() && object->hasComponent<Components::Transform>())
	{
		auto& selectedTransform = object->getComponent<Components::Transform>();

		auto& transform = edtr_pointer->getComponent<Components::Transform>();
		auto& visual = edtr_pointer->getComponent<Triton::Components::Visual>();

		transform.Position = selectedTransform.Position;
		transform.Rotation = selectedTransform.Rotation;
		visual.Visible = true;
	}
	else
	{
		auto& visual = edtr_pointer->getComponent<Triton::Components::Visual>();
		visual.Visible = false;
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


	m_scriptEngine->loadAssembly("../bin/MonoDLL.dll");
}

void Triton::EditorScene::createEntities()
{
	edtr_pointer = newObject();

	auto& transform = edtr_pointer->addComponent<Triton::Components::Transform>();

	transform.Position = Triton::Vector3(0.0, 10.0, 25.0);
	transform.Scale = Triton::Vector3(5, 5, 5);
	transform.Rotation = Triton::Vector3(0.0, 0.0, 0.0);

	edtr_pointer->addComponent<Triton::Components::Visual>(edtr_3DPOINTER->getAssetID(), edtr_mat_3DPOINTER->getAssetID());
}

void Triton::EditorScene::setupForRendering()
{
	// Bind/enable the view port
	m_mainRenderBuffer->addCommand<RCommands::PushViewport>(m_gameWindow->getViewport());
}

void Triton::EditorScene::setupForObjects()
{
	// Change context settings
	m_mainRenderBuffer->addCommand<RCommands::ChangeContextSetting>
		([&](reference<PType::Context> context)
	{
		context->depthBufferState(false);
	});
}

void Triton::EditorScene::onRenderObject(reference<GameObject>& object, Components::Transform& transform, Components::Visual& visual)
{
	if (!visual.Visible)
	{
		return;
	}

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
		shader->setBufferValue("frame_PerFrame", "viewMatrix", &m_edtr_state->CurrentScene.as<Triton::Scene>()->getViewMatrix());

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
}

void Triton::EditorScene::onRenderDone()
{
	// Change context settings
	m_mainRenderBuffer->addCommand<RCommands::ChangeContextSetting>
		([&](reference<PType::Context> context)
	{
		context->depthBufferState(true);
		context->renderer->default();
	});

	// Render the editor

	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// Dock space enable
	showDockSpace(&m_isOpen.dockspace);

	// Main menu bar
	showMainMenuBar(m_isOpen);

	// View port
	showViewport(&m_isOpen.viewport, m_gameWindow);

	// Log window
	showLogWindow(&m_isOpen.logWindow);

	// Show asset window
	showAssetWindow(&m_isOpen.assetWindow, m_edtr_state);

	// Show inspector
	showInspector(&m_isOpen.inspectorWindow, m_edtr_state, m_assetManager);

	// Metrics window
	showMetrics(&m_isOpen.metrics, m_timer->renderDelta(), m_timer->updateDelta());

	// Scene view
	showSceneView(&m_isOpen.sceneView, "test scene", m_edtr_state);

	// Material editor
	showMaterialEditor(&m_isOpen.materialViewport, m_edtr_state, m_assetManager);

	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	ImGui::ShowDemoWindow();

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

bool Triton::EditorScene::onAppDrop(std::vector<std::string> files)
{
	Triton::Resource::AssetCreateParams asset_desc;

	for (std::string& path : files)
	{
		auto texParams = Triton::Data::File::tryLoadTexture(path);

		if (texParams != nullptr)
		{
			asset_desc.Type = Triton::Resource::AssetCreateParams::AssetType::TEXTURE;
			asset_desc.Name = Triton::Data::File::fileNameFromPath(path);
			asset_desc.CreateParams = texParams;
			auto texture = m_assetManager->createAsset(asset_desc);
			m_edtr_state->AllTextures.push_back(texture.as<Data::PlainTexture>());
			delete texParams;
		}
		else
		{
			auto vaoParams = Triton::Data::File::tryLoadMesh(path);

			if (vaoParams != nullptr)
			{
				asset_desc.Type = Triton::Resource::AssetCreateParams::AssetType::MESH;
				asset_desc.Name = Triton::Data::File::fileNameFromPath(path);
				asset_desc.CreateParams = vaoParams;
				auto vao = m_assetManager->createAsset(asset_desc);
				m_edtr_state->AllMeshes.push_back(vao.as<Data::Mesh>());
				delete vaoParams;
			}
		}
	}

	return true;
}