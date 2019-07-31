#pragma once

#include <imgui.h>

#include "TritonPlatform/PlatformAbstraction.h"
#include "TritonPlatform/CrossTypes/Texture.h"
#include "TritonPlatform/DirectX/DXTypes.h"
#include "Triton/Core/Wrapers/Viewport.h"

void showSceneView(bool* p_open, std::string sceneName, Triton::reference<Triton::EditorState>& state)
{
	static char name[64] = "";
	static std::string* entity_name = nullptr;

	bool contextDefined = false;

	if (*p_open == false)
	{
		return;
	}

	ImGui::SetNextWindowSize(ImVec2(300, 700), ImGuiCond_FirstUseEver);
	ImGui::Begin("Scene view", p_open);

	ImVec2 im_size = ImGui::GetWindowSize();

	if (ImGui::TreeNode(sceneName.c_str()))
	{
		if (ImGui::BeginPopupContextWindow("scene view context menu"))
		{
			if (ImGui::Button("New entity"))
			{
				auto id = state->CurrentScene->Entities->create();
				state->NameMap->EntityNames[id] = "newEntity";
			}

			ImGui::EndPopup();
		}

		static int selection_mask = (1 << 2);
		int node_clicked = -1;
		ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 3);

		state->CurrentScene->Entities->each(
			[&](Triton::ECS::Entity entity) 
			{
				ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_DefaultOpen;

				if (state->CurrentEntity == entity)
				{
					node_flags |= ImGuiTreeNodeFlags_Selected;
				}
				node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
				
				if (ImGui::TreeNodeEx((void*)(intptr_t)entity, node_flags, "%s", state->NameMap->EntityNames[entity].c_str()))
				{
					if (!contextDefined)
					{
						if (ImGui::BeginPopupContextItem("scene view entity context menu"))
						{
							ImGui::Text("Edit name:"); ImGui::SameLine();
							ImGui::InputText("##editName", name, 64, ImGuiInputTextFlags_CharsNoBlank);

							if (entity_name != nullptr)
							{
								*entity_name = name;
							}

							if (ImGui::Button("Close"))
								ImGui::CloseCurrentPopup();
							ImGui::EndPopup();
						}

						contextDefined = true;
					}

					if (ImGui::OpenPopupOnItemClick("scene view entity context menu", 1))
					{
						entity_name = &state->NameMap->EntityNames[entity];
						std::strcpy(name, entity_name->c_str());
					}

					if (ImGui::IsItemClicked())
						state->CurrentEntity = entity;
				}
			});

		ImGui::PopStyleVar();

		ImGui::TreePop();
	}

	ImGui::End();
}