#pragma once

#include <imgui.h>

#include "TritonPlatform/PlatformAbstraction.h"
#include "TritonPlatform/CrossTypes/Texture.h"
#include "TritonPlatform/DirectX/DXTypes.h"

void ShowViewport(bool* p_open, Triton::reference<Triton::PType::DXFramebuffer> viewport, Triton::Vector2& size)
{
	ImGui::SetNextWindowSize(ImVec2(size.x, size.y), ImGuiCond_FirstUseEver);
	ImGui::Begin("Viewport");

	ImVec2 im_size = ImGui::GetWindowSize();

	size.x = im_size.x;
	size.y = im_size.y;

	// Get the texture associated to the FBO
	auto tex = viewport->getShaderResourceView();

	ImGui::Image((void *)tex, 
		ImVec2(im_size.x, im_size.y),
		ImVec2(0, 0), ImVec2(1, 1));

	ImGui::End();

}