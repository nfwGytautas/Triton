#include "TRpch.h"
#include "GUICollection.h"

#ifndef TR_DISABLE_GUI

Triton::UI::GUICollection::GUICollection()
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

Triton::UI::GUICollection::GUICollection(size_t aSize)
	: mGUIS(aSize)
{
}

Triton::UI::GUICollection::~GUICollection()
{
}

void Triton::UI::GUICollection::RefreshCollection()
{
	for (auto& gui : mGUIS)
	{
		gui->Refresh();
	}
}

void Triton::UI::GUICollection::DrawCollection()
{
	for (auto& gui : mGUIS)
	{
		gui->Draw();
	}
}

void Triton::UI::GUICollection::VisualizeCollection()
{
	for (auto& gui : mGUIS)
	{
		gui->Visualize();
	}
}

void Triton::UI::GUICollection::UpdateCollection(float aDelta)
{
	for (auto& gui : mGUIS)
	{
		gui->Update(aDelta);
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

#endif