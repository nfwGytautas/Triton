#pragma once

#include <imgui.h>

void showMetrics(bool* p_open, float RPS, float UPS)
{
	if (*p_open == false)
	{
		return;
	}

	ImGui::Begin("Metrics", p_open);

	ImGuiIO& io = ImGui::GetIO();
	ImGui::Text("ImGui version: [%s]", ImGui::GetVersion());
	ImGui::Text(" FPS: %.0f", io.Framerate);
	ImGui::Text(" RPF: %.2f ms/frame", RPS * 1000);
	ImGui::Text(" UPF: %.2f ms/frame", UPS * 1000);
	ImGui::Separator();

	ImGui::End();
}