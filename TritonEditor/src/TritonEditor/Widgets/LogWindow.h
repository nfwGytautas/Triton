#pragma once

#include <string>

#include <imgui.h>

#include "TritonPlatform/PlatformAbstraction.h"
#include "TritonPlatform/CrossTypes/Texture.h"
#include "TritonPlatform/DirectX/DXTypes.h"
#include "Triton/Core/Wrapers/Viewport.h"

void showLogWindow(bool* p_open)
{
	static unsigned int prev_count = 0;

	if (*p_open == true)
	{
		ImGui::SetNextWindowSize(ImVec2(800, 200), ImGuiCond_FirstUseEver);
		ImGui::Begin("Log", p_open);

		ImVec2 im_size = ImGui::GetWindowSize();

		for (unsigned int i = 0; i < Triton::Log::History.size(); i++)
		{
			ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + im_size.x - 10);

			ImGui::Text(Triton::Log::History[i].Format.c_str());

			switch (Triton::Log::History[i].Level)
			{
			case spdlog::level::trace:
				ImGui::TextDisabled(Triton::Log::History[i].Text.c_str());
				break;

			case spdlog::level::info:
				ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), Triton::Log::History[i].Text.c_str());
				break;

			case spdlog::level::warn:
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), Triton::Log::History[i].Text.c_str());
				break;

			case spdlog::level::err:
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), Triton::Log::History[i].Text.c_str());
				break;
			}

			ImGui::PopTextWrapPos();
		}


		if (prev_count < Triton::Log::History.size())
		{
			ImGui::SetScrollHere(1.0f);
			prev_count = Triton::Log::History.size();
		}

		ImGui::End();
	}
}