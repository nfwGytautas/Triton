#pragma once

#include <imgui.h>

#include "TritonPlatform/PlatformAbstraction.h"
#include "TritonPlatform/CrossTypes/Texture.h"
#include "TritonPlatform/DirectX/DXTypes.h"
#include "Triton/Core/Wrapers/Viewport.h"

void showViewport(bool* p_open, Triton::reference<Triton::GameWindow>& gameWindow)
{
	if (*p_open == true)
	{
		ImGui::SetNextWindowSize(ImVec2(gameWindow->getWidth(), gameWindow->getHeight()), ImGuiCond_FirstUseEver);
		ImGui::Begin("Viewport", p_open, ImGuiWindowFlags_NoTitleBar);

		ImVec2 im_size = ImGui::GetWindowSize();

		gameWindow->getViewport()->Width = im_size.x;
		gameWindow->getViewport()->Height = im_size.y;

		ImGui::Image(gameWindow->getTexture(),
			ImVec2(im_size.x - 20, im_size.y - 40),
			ImVec2(0, 0), ImVec2(1, 1));

		ImGui::End();
	}
}

void showViewport(bool* p_open, Triton::reference<Triton::Data::Viewport>& viewPort)
{
	if (*p_open == true)
	{
		ImGui::SetNextWindowSize(ImVec2(viewPort->Width, viewPort->Height), ImGuiCond_FirstUseEver);
		ImGui::Begin("Viewport", NULL, ImGuiWindowFlags_NoTitleBar);

		ImVec2 im_size = ImGui::GetWindowSize();

		viewPort->Width = im_size.x;
		viewPort->Height = im_size.y;

		ImGui::Image(viewPort->Framebuffer->getAsTexture(),
			ImVec2(im_size.x - 20, im_size.y - 40),
			ImVec2(0, 0), ImVec2(1, 1));

		ImGui::End();
	}
}