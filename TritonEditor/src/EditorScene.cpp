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

		m_input = InputManager.as<Triton::Core::InputManager>();
		m_input->AddListener(*this);

		onRegistered();

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
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;
	//io.ConfigViewportsNoDefaultParent = true;
	//io.ConfigDockingTabBarOnSingleWindows = true;
	io.ConfigDockingWithShift = false;
	io.ConfigDockingTransparentPayload = true;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
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

	// Asset creation description
	Triton::Resource::AssetCreateParams asset_desc;
	asset_desc.Type = Triton::Resource::AssetCreateParams::AssetType::VIEWPORT;
	asset_desc.Width = 1280;
	asset_desc.Height = 720;

	TR_GET_CLASS(AssetManager);
	m_viewPortObject = AssetManager.as<Manager::AssetManager>()->createAsset(asset_desc).as<Triton::Data::Viewport>();

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
}

void Triton::EditorScene::onRender()
{
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	static bool test = true;

	ShowDockSpace(&m_dockspace);

	// Viewport
	ShowViewport(&m_viewport, m_viewPortObject);

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
