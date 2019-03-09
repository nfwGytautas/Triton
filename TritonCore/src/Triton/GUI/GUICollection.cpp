#include "TRpch.h"
#include "GUICollection.h"

#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include "Platform\Windows\vendorIMPL\imgui_impl_opengl3.h"

Triton::UI::GUICollection::GUICollection(Core::EventManager* mManager)
	:EventListener(mManager)
{
	this->Listen<Triton::MouseButtonPressedEvent>(Triton::EventBehavior(TR_BIND_FUNC(GUICollection::OnMouseButtonPressedEvent, this, std::placeholders::_1)));
	this->Listen<Triton::MouseButtonReleasedEvent>(Triton::EventBehavior(TR_BIND_FUNC(GUICollection::OnMouseButtonReleasedEvent, this, std::placeholders::_1)));
	this->Listen<Triton::MouseMovedEvent>(Triton::EventBehavior(TR_BIND_FUNC(GUICollection::OnMouseMovedEvent, this, std::placeholders::_1)));
	this->Listen<Triton::MouseScrolledEvent>(Triton::EventBehavior(TR_BIND_FUNC(GUICollection::OnMouseScrolledEvent, this, std::placeholders::_1)));
	this->Listen<Triton::KeyPressedEvent>(Triton::EventBehavior(TR_BIND_FUNC(GUICollection::OnKeyPressedEvent, this, std::placeholders::_1)));
	this->Listen<Triton::KeyReleasedEvent>(Triton::EventBehavior(TR_BIND_FUNC(GUICollection::OnKeyReleasedEvent, this, std::placeholders::_1)));
	this->Listen<Triton::KeyInputEvent>(Triton::EventBehavior(TR_BIND_FUNC(GUICollection::OnKeyInputEvent, this, std::placeholders::_1)));
	this->Listen<Triton::WindowResizeEvent>(Triton::EventBehavior(TR_BIND_FUNC(GUICollection::OnWindowResizeEvent, this, std::placeholders::_1)));
}

Triton::UI::GUICollection::GUICollection(Core::EventManager* mManager, size_t aSize)
	: EventListener(mManager), mGUIS(aSize)
{
	this->Listen<Triton::MouseButtonPressedEvent>(Triton::EventBehavior(TR_BIND_FUNC(GUICollection::OnMouseButtonPressedEvent, this, std::placeholders::_1)));
	this->Listen<Triton::MouseButtonReleasedEvent>(Triton::EventBehavior(TR_BIND_FUNC(GUICollection::OnMouseButtonReleasedEvent, this, std::placeholders::_1)));
	this->Listen<Triton::MouseMovedEvent>(Triton::EventBehavior(TR_BIND_FUNC(GUICollection::OnMouseMovedEvent, this, std::placeholders::_1)));
	this->Listen<Triton::MouseScrolledEvent>(Triton::EventBehavior(TR_BIND_FUNC(GUICollection::OnMouseScrolledEvent, this, std::placeholders::_1)));
	this->Listen<Triton::KeyPressedEvent>(Triton::EventBehavior(TR_BIND_FUNC(GUICollection::OnKeyPressedEvent, this, std::placeholders::_1)));
	this->Listen<Triton::KeyReleasedEvent>(Triton::EventBehavior(TR_BIND_FUNC(GUICollection::OnKeyReleasedEvent, this, std::placeholders::_1)));
	this->Listen<Triton::KeyInputEvent>(Triton::EventBehavior(TR_BIND_FUNC(GUICollection::OnKeyInputEvent, this, std::placeholders::_1)));
	this->Listen<Triton::WindowResizeEvent>(Triton::EventBehavior(TR_BIND_FUNC(GUICollection::OnWindowResizeEvent, this, std::placeholders::_1)));
}

Triton::UI::GUICollection::~GUICollection()
{
}

void Triton::UI::GUICollection::InitGUI(unsigned int aWidth, unsigned int aHeight)
{
	ImGui::StyleColorsDark();

	ImGuiIO& io = ImGui::GetIO();
	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
	io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

	// TEMPORARY: should eventually use Hazel key codes
	io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
	io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
	io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
	io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
	io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
	io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
	io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
	io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
	io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
	io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
	io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
	io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
	io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
	io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
	io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
	io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
	io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
	io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
	io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

	io.DisplaySize.x = aWidth;
	io.DisplaySize.y = aHeight;

	ImGui_ImplOpenGL3_Init("#version 410");
}

void Triton::UI::GUICollection::DrawCollection()
{
	GUI::Refresh();
	for (auto& gui : mGUIS)
	{
		if(gui->IsActive())
		{
			gui->Visualize();
		}
	}
	GUI::Draw();
}

void Triton::UI::GUICollection::UpdateCollection(float aDelta)
{
	for (auto& gui : mGUIS)
	{
		if (gui->IsActive())
		{
			gui->Update(aDelta);
		}
	}
}

void Triton::UI::GUICollection::AddGUI(std::shared_ptr<GUI> aGUI)
{
	mGUIS.push_back(aGUI);
}

bool Triton::UI::GUICollection::OnMouseButtonPressedEvent(const Triton::Event& e)
{
	TR_EVENT_CAST(mbpe, e, Triton::MouseButtonPressedEvent);

	ImGuiIO& io = ImGui::GetIO();
	io.MouseDown[mbpe.GetMouseButton()] = true;

	return false;
}

bool Triton::UI::GUICollection::OnMouseButtonReleasedEvent(const Triton::Event& e)
{
	TR_EVENT_CAST(mbre, e, Triton::MouseButtonReleasedEvent);

	ImGuiIO& io = ImGui::GetIO();
	io.MouseDown[mbre.GetMouseButton()] = false;

	return false;
}

bool Triton::UI::GUICollection::OnMouseMovedEvent(const Triton::Event& e)
{
	TR_EVENT_CAST(mme, e, Triton::MouseMovedEvent);

	ImGuiIO& io = ImGui::GetIO();
	io.MousePos = ImVec2(mme.GetX(), mme.GetY());

	return false;
}

bool Triton::UI::GUICollection::OnMouseScrolledEvent(const Triton::Event& e)
{
	TR_EVENT_CAST(mse, e, Triton::MouseScrolledEvent);

	ImGuiIO& io = ImGui::GetIO();
	io.MouseWheelH += mse.GetXOffset();
	io.MouseWheel += mse.GetYOffset();

	return true;
}

bool Triton::UI::GUICollection::OnKeyPressedEvent(const Triton::Event& e)
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

bool Triton::UI::GUICollection::OnKeyReleasedEvent(const Triton::Event& e)
{
	TR_EVENT_CAST(kre, e, Triton::KeyReleasedEvent);

	ImGuiIO& io = ImGui::GetIO();
	io.KeysDown[kre.GetKeyCode()] = false;

	io.KeyCtrl = io.KeysDown[341] || io.KeysDown[345];
	io.KeyShift = io.KeysDown[340] || io.KeysDown[344];
	io.KeyAlt = io.KeysDown[342] || io.KeysDown[346];
	io.KeySuper = io.KeysDown[343] || io.KeysDown[347];
	return false;
}

bool Triton::UI::GUICollection::OnKeyInputEvent(const Triton::Event& e)
{
	TR_EVENT_CAST(kie, e, Triton::KeyInputEvent);

	ImGuiIO& io = ImGui::GetIO();
	int keycode = kie.GetKeyCode();
	if (keycode > 0 && keycode < 0x10000)
		io.AddInputCharacter((unsigned short)keycode);

	return false;
}

bool Triton::UI::GUICollection::OnWindowResizeEvent(const Triton::Event& e)
{
	TR_EVENT_CAST(wre, e, Triton::WindowResizeEvent);

	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(wre.GetWidth(), wre.GetHeight());
	io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);

	return false;
}