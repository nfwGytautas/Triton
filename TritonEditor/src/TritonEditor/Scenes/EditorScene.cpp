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
	edtr_materialPreviewCamera = std::make_unique<Triton::Camera>(Vector3(0.0f, 0.0f, 0.0f));
	m_shaderBalls = std::make_unique<ECS::Registry>();
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
		TR_GET_EDTR_STATE(m_edtr_state);

		TR_GET_CLASS(InputManager);
		TR_GET_CLASS(AssetManager);

		m_input = InputManager.as<Triton::Core::InputManager>();
		m_input->AddListener(*this);

		m_assetManager = AssetManager.as<Manager::AssetManager>();

		m_mainRenderBuffer = this->getClassByID((size_t)Core::TritonClasses::MainRenderBuffer).as<Triton::Core::RenderBuffer>();
		m_graphicsContext = this->getClassByID((size_t)Core::TritonClasses::Context).as<PType::Context>();
		m_graphicsContext->renderer->setVsync(true);

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
	ECS::Entity selectedEntity = m_edtr_state->CurrentEntity;
	if(m_edtr_state->CurrentScene->Entities->valid(selectedEntity))
	{
		if(m_edtr_state->CurrentScene->Entities->has<Components::Transform>(selectedEntity))
		{
			auto& selectedTransform = m_edtr_state->CurrentScene->Entities->get<Components::Transform>(selectedEntity);

			auto& transform = Entities->get<Triton::Components::Transform>(edtr_pointer_id);
			auto& visual = Entities->get<Triton::Components::Visual>(edtr_pointer_id);

			transform.Position = selectedTransform.Position;
			transform.Rotation = selectedTransform.Rotation;
			visual.Visible = true;
		}
		else
		{
			auto& visual = Entities->get<Triton::Components::Visual>(edtr_pointer_id);
			visual.Visible = false;
		}
	}

	// Check if there are any changes to the loaded materials
	if (m_prevMatSize < m_edtr_state->AllMaterials.size())
	{
		createShaderballs();

		m_prevMatSize = m_edtr_state->AllMaterials.size();
	}
}

void Triton::EditorScene::onRender()
{
	renderEntities();

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

	// Shader ball mesh
	asset_desc.Type = Triton::Resource::AssetCreateParams::AssetType::MESH;
	asset_desc.Paths[0] = "C:/dev/Triton/Models/shaderBall.obj";

	edtr_shaderBall = m_assetManager->createAsset(asset_desc).as<Triton::Data::Mesh>();

	asset_desc.Type = Triton::Resource::AssetCreateParams::AssetType::VIEWPORT;
	asset_desc.Width = 1000;
	asset_desc.Height = 1000;
	edtr_materialViewport = m_assetManager->createAsset(asset_desc).as<Triton::Data::Viewport>();

	// Set up the main directional light
	auto dlight = new Triton::Graphics::DirectionalLight(Triton::Vector3(1.0f, 0.0f, 1.0f));
	dlight->Ambient = Triton::Vector3(0.15f, 0.15f, 0.15f);
	dlight->Specular = Triton::Vector3(1.0f, 1.0f, 1.0f);
	m_mainMaterialPreviewLight = dlight;
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

void Triton::EditorScene::createShaderballs()
{
	m_shaderBalls->reset();
	
	for (int i = 0; i < m_edtr_state->AllMaterials.size(); i++)
	{
		ECS::Entity entity = m_shaderBalls->create();

		auto& transform = m_shaderBalls->assign<Triton::Components::Transform>(entity);

		transform.Position = Triton::Vector3(-10.0, 5.0, 250.0);

		m_shaderBalls->assign<Triton::Components::Visual>(entity, edtr_shaderBall->getAssetID(), m_edtr_state->AllMaterials[i]->getAssetID());
	}

	m_renderShaderballs = true;
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

void Triton::EditorScene::renderEntities()
{
	// Bind/enable the view port
	m_mainRenderBuffer->addCommand<RCommands::PushViewport>(m_gameWindow->getViewport());

	// Change context settings
	m_mainRenderBuffer->addCommand<RCommands::ChangeContextSetting>
		([&](reference<PType::Context> context)
	{
		context->depthBufferState(false);
	});

	Entities->view<Components::Transform, Components::Visual>().each([&](auto& transform, auto& visual) {

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
	});

	// Change context settings
	m_mainRenderBuffer->addCommand<RCommands::ChangeContextSetting>
		([&](reference<PType::Context> context)
	{
		context->depthBufferState(true);
		context->renderer->default();
	});

	// If needed render the shader balls
	if (m_renderShaderballs)
	{
		// Push shader ball view port
		m_mainRenderBuffer->addCommand<RCommands::PushViewport>(edtr_materialViewport);

		// Clear the view port
		m_mainRenderBuffer->addCommand<RCommands::ClearCurrentViewport>();

		// Push the shader ball mesh
		m_mainRenderBuffer->addCommand<RCommands::PushMesh>(edtr_shaderBall);

		m_shaderBalls->view<Components::Transform, Components::Visual>().each([&](auto& transform, auto& visual) {

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
				shader->setBufferValue("frame_PerFrame", "viewMatrix", &edtr_materialPreviewCamera->ViewMatrix());
				shader->setBufferValue("CameraBuffer", "cameraPosition", &edtr_materialPreviewCamera->Position);

				m_mainMaterialPreviewLight->bind(shader);

				shader->updateBuffers(PType::BufferUpdateType::FRAME);
				shader->updateBuffers(PType::BufferUpdateType::PERSISTANT);
			}
			);

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

		m_renderShaderballs = true;
	}
}
