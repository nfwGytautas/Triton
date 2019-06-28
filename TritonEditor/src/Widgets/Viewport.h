#pragma once

#include <imgui.h>

#include "TritonPlatform/PlatformAbstraction.h"
#include "TritonPlatform/CrossTypes/Texture.h"
#include "TritonPlatform/DirectX/DXTypes.h"
#include "Triton/Core/Wrapers/Viewport.h"

void ShowViewport(bool* p_open, Triton::reference<Triton::Data::Viewport>& viewport)
{
	ImGui::SetNextWindowSize(ImVec2(viewport->Width, viewport->Height), ImGuiCond_FirstUseEver);
	ImGui::Begin("Viewport");

	ImVec2 im_size = ImGui::GetWindowSize();

	viewport->Width = im_size.x;
	viewport->Height = im_size.y;

	// Get the texture associated to the FBO
	auto tex = viewport->Framebuffer.as<Triton::PType::DXFramebuffer>()->getShaderResourceView();

	ImGui::Image((void *)tex, 
		ImVec2(im_size.x, im_size.y),
		ImVec2(0, 0), ImVec2(1, 1));

	ImGui::End();

}