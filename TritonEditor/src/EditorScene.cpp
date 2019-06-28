#include "EditorScene.h"
#include <string>

#include <imgui.h>
#include "Impl/imgui_impl_win32.h"
#include "Impl/imgui_impl_dx11.h"

#include "TritonPlatform/DirectX/DXSpecific.h"

#include "Widgets/DockSpace.h"
#include "Widgets/Viewport.h"

IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Triton::EditorScene::EditorScene(PType::Context* context, Core::EventManager* manager)
	: SceneBase(context), EventInterface(manager)
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

	context->fillPacket(packet);

	ImGui_ImplWin32_Init(packet->hwnd);
	ImGui_ImplDX11_Init(packet->device, packet->deviceContext);

	delete packet;
}

void Triton::EditorScene::update(float delta)
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

void Triton::EditorScene::render()
{
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	
	static bool test = true;

	ShowDockSpace(&m_dockspace);

	// Viewport
	ShowViewport(&m_viewport, ViewPort);

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

void Triton::EditorScene::destroy()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
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
	io.MousePos = ImVec2(aX, aY);

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