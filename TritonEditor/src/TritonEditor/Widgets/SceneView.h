#pragma once

#include <imgui.h>

#include "TritonPlatform/PlatformAbstraction.h"
#include "TritonPlatform/CrossTypes/Texture.h"
#include "TritonPlatform/DirectX/DXTypes.h"
#include "Triton/Core/Wrapers/Viewport.h"

void showSceneView(bool* p_open, std::string sceneName, Triton::reference<Triton::EditorState>& state)
{
	if (*p_open == false)
	{
		return;
	}

	ImGui::SetNextWindowSize(ImVec2(300, 700), ImGuiCond_FirstUseEver);
	ImGui::Begin("Scene view", NULL);

	ImVec2 im_size = ImGui::GetWindowSize();

	if (ImGui::BeginPopupContextItem("scene view context menu"))
	{
		ImGui::Text("Edit name:");
		if (ImGui::Button("Close"))
			ImGui::CloseCurrentPopup();
		ImGui::EndPopup();
	}

	if (ImGui::TreeNode(sceneName.c_str()))
	{
		static int selection_mask = (1 << 2); // Dumb representation of what may be user-side selection state. You may carry selection state inside or outside your objects in whatever format you see fit.
		int node_clicked = -1;                // Temporary storage of what node we have clicked to process selection at the end of the loop. May be a pointer to your own node type, etc.
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
				ImGui::TreeNodeEx((void*)(intptr_t)entity, node_flags, "%s", state->NameMap->EntityNames[entity].c_str());
				if (ImGui::IsItemClicked())
					state->CurrentEntity = entity;
			});

		ImGui::PopStyleVar();

		ImGui::TreePop();
	}
	ImGui::OpenPopupOnItemClick("scene view context menu", 1);

	ImGui::End();

}