#include "TRpch.h"
#include "GUI.h"

#ifndef TR_DISABLE_GUI

#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include "Platform\Windows\vendorIMPL\imgui_impl_opengl3.h"

Triton::UI::GUI::GUI()
{
	mActive = true;
}

Triton::UI::GUI::~GUI()
{
	
}

void Triton::UI::GUI::Refresh()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();
}

void Triton::UI::GUI::Draw()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

ImGuiIO& Triton::UI::GUI::IO()
{
	return ImGui::GetIO();
}

bool Triton::UI::GUI::IsActive()
{
	return mActive;
}

void Triton::UI::GUI::Disable()
{
	mActive = false;
}

void Triton::UI::GUI::Enable()
{
	mActive = true;
}


#endif