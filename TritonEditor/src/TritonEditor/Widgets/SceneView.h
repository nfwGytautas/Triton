#pragma once

#include <imgui.h>

#include "TritonPlatform/PlatformAbstraction.h"
#include "TritonPlatform/CrossTypes/Texture.h"
#include "TritonPlatform/DirectX/DXTypes.h"
#include "Triton/Core/Wrapers/Viewport.h"

#include "Triton/Entity/GameObject.h"

void showSceneView(bool* p_open, std::string sceneName, Triton::reference<Triton::EditorState>& state)
{
	static char name[64] = "";
	static std::string entity_name_copy = "";

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
				state->CurrentScene->newObject();
			}

			ImGui::EndPopup();
		}

		static int selection_mask = (1 << 2);
		int node_clicked = -1;
		ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 3);\

		auto it = state->CurrentScene->begin();
		auto end = state->CurrentScene->end();

		for (it; it != end; it++)
		{
			Triton::reference<Triton::GameObject> gameObject = *it;

			ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_DefaultOpen;

			if (state->CurrentGameObject == gameObject)
			{
				node_flags |= ImGuiTreeNodeFlags_Selected;
			}
			node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet

			if (ImGui::TreeNodeEx(gameObject->getName().c_str(), node_flags))
			{
				if (!contextDefined)
				{
					if (ImGui::BeginPopupContextItem("scene view entity context menu"))
					{
						ImGui::Text("Edit name:"); ImGui::SameLine();
						ImGui::InputText("##editName", name, 64, ImGuiInputTextFlags_CharsNoBlank);

						state->CurrentGameObject->setName(std::string(name));

						if (ImGui::Button("Close"))
							ImGui::CloseCurrentPopup();
						ImGui::EndPopup();
					}

					contextDefined = true;
				}

				if (ImGui::OpenPopupOnItemClick("scene view entity context menu", 1))
				{
					state->CurrentGameObject = gameObject;
					entity_name_copy = state->CurrentGameObject->getName();
					std::strcpy(name, entity_name_copy.c_str());
				}

				if (ImGui::IsItemClicked())
					state->CurrentGameObject = gameObject;
			}
		}

		ImGui::PopStyleVar();

		ImGui::TreePop();
	}

	ImGui::End();
}