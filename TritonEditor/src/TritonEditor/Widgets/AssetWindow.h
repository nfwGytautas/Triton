#pragma once

#include <string>

#include <imgui.h>

#include "TritonPlatform/PlatformAbstraction.h"
#include "TritonPlatform/CrossTypes/Texture.h"
#include "TritonPlatform/DirectX/DXTypes.h"
#include "Triton/Core/Wrapers/Viewport.h"

void showAssetWindow(bool* p_open, Triton::reference<Triton::EditorState>& edtr_state)
{
	static unsigned int prev_count = 0;

	if (*p_open == true)
	{
		ImGui::SetNextWindowSize(ImVec2(800, 200), ImGuiCond_FirstUseEver);
		ImGui::Begin("Assets", p_open);

		ImVec2 im_size = ImGui::GetWindowSize();

		ImGuiStyle& style = ImGui::GetStyle();

		int material_count = edtr_state->AllMaterials.size();
		int mesh_count = edtr_state->AllMeshes.size();
		int texture_count = edtr_state->AllTextures.size();
		int shader_count = edtr_state->AllShaders.size();

		float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

		ImVec2 button_sz(80, 80);

		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
		if (ImGui::BeginTabBar("Assets", tab_bar_flags))
		{
			if (ImGui::BeginTabItem("Materials"))
			{
				for (int n = 0; n < material_count; n++)
				{
					ImGui::PushID(n);
					if (ImGui::Button(edtr_state->AllMaterials[n]->getName().c_str(), button_sz))
					{
						edtr_state->CurrentMaterial = edtr_state->AllMaterials[n];
					}
					float last_button_x2 = ImGui::GetItemRectMax().x;
					float next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x; // Expected position if next button was on same line
					if (n + 1 < material_count && next_button_x2 < window_visible_x2)
						ImGui::SameLine();

					if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
					{
						ImGui::SetDragDropPayload("ASSET_MATERIAL", &n, sizeof(int));
						ImGui::EndDragDropSource();
					}

					ImGui::PopID();
				}

				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Meshes"))
			{
				for (int n = 0; n < mesh_count; n++)
				{
					ImGui::PushID(n);
					ImGui::Button(edtr_state->AllMeshes[n]->getName().c_str(), button_sz);
					float last_button_x2 = ImGui::GetItemRectMax().x;
					float next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x; // Expected position if next button was on same line
					if (n + 1 < mesh_count && next_button_x2 < window_visible_x2)
						ImGui::SameLine();

					if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
					{
						ImGui::SetDragDropPayload("ASSET_MESH", &n, sizeof(int));
						ImGui::EndDragDropSource();
					}

					ImGui::PopID();
				}

				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Shaders"))
			{
				for (int n = 0; n < shader_count; n++)
				{
					ImGui::PushID(n);
					ImGui::Button(edtr_state->AllShaders[n]->getName().c_str(), button_sz);
					float last_button_x2 = ImGui::GetItemRectMax().x;
					float next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x; // Expected position if next button was on same line
					if (n + 1 < shader_count && next_button_x2 < window_visible_x2)
						ImGui::SameLine();

					if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
					{
						ImGui::SetDragDropPayload("ASSET_SHADER", &n, sizeof(int));
						ImGui::EndDragDropSource();
					}

					ImGui::PopID();
				}

				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Textures"))
			{
				for (int n = 0; n < texture_count; n++)
				{
					ImGui::PushID(n);
					ImGui::Button(edtr_state->AllTextures[n]->getName().c_str(), button_sz);
					float last_button_x2 = ImGui::GetItemRectMax().x;
					float next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x; // Expected position if next button was on same line
					if (n + 1 < texture_count && next_button_x2 < window_visible_x2)
						ImGui::SameLine();

					if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
					{
						ImGui::SetDragDropPayload("ASSET_TEXTURE", &n, sizeof(int));
						ImGui::EndDragDropSource();
					}

					ImGui::PopID();
				}

				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Scripts"))
			{
				const int size = edtr_state->AllScripts.size();
				for (int n = 0; n < size; n++)
				{
					ImGui::PushID(n);
					ImGui::Button(Triton::Data::File::fileNameFromPath(edtr_state->AllScripts[n]).c_str(), button_sz);
					float last_button_x2 = ImGui::GetItemRectMax().x;
					float next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x; // Expected position if next button was on same line
					if (n + 1 < texture_count && next_button_x2 < window_visible_x2)
						ImGui::SameLine();

					if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
					{
						ImGui::SetDragDropPayload("ASSET_SCRIPT", &n, sizeof(int));
						ImGui::EndDragDropSource();
					}

					ImGui::PopID();
				}

				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}

		ImGui::End();
	}
}