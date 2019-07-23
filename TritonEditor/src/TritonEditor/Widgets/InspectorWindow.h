#pragma once

#include <string>

#include <imgui.h>

#include "TritonPlatform/PlatformAbstraction.h"
#include "TritonPlatform/CrossTypes/Texture.h"
#include "TritonPlatform/DirectX/DXTypes.h"
#include "Triton/Core/Wrapers/Viewport.h"

void showInspector(bool* p_open, 
	Triton::reference<Triton::EditorState>& edtr_state, 
	Triton::reference<Triton::Manager::AssetManager>& asset_manager)
{
	static unsigned int prev_count = 0;

	const float cFloatInputWidth = 75;
	const float cFloatInputSensitivity = 0.01f;
	const float cFloatInputRotationSensitivity = cFloatInputSensitivity * 10;

	if (*p_open == true)
	{
		ImGui::SetNextWindowSize(ImVec2(800, 200), ImGuiCond_FirstUseEver);
		ImGui::Begin("Inspector", p_open);

		ImVec2 im_size = ImGui::GetWindowSize();

		auto& registry = edtr_state->CurrentScene->Entities;
		auto& entity = edtr_state->CurrentEntity;

		// Transformation
		if (registry->has<Triton::Components::Transform>(entity))
		{
			ImGui::Spacing();
			if (ImGui::CollapsingHeader("Transform"))
			{
				Triton::Components::Transform& transform = registry->get<Triton::Components::Transform>(entity);

				ImGui::Separator();

				//Transform -> Position
				ImGui::Text("Position:"); 

				ImGui::Separator();
				Triton::Vector3& pos = transform.Position;
				ImGui::Text("X:"); ImGui::SameLine(); ImGui::PushItemWidth(cFloatInputWidth); ImGui::DragFloat("##posX", &pos.x, cFloatInputSensitivity);
				ImGui::SameLine();
				ImGui::Text("Y:"); ImGui::SameLine(); ImGui::PushItemWidth(cFloatInputWidth); ImGui::DragFloat("##posY", &pos.y, cFloatInputSensitivity);
				ImGui::SameLine();
				ImGui::Text("Z:"); ImGui::SameLine(); ImGui::PushItemWidth(cFloatInputWidth); ImGui::DragFloat("##posZ", &pos.z, cFloatInputSensitivity);
				ImGui::Separator();


				//Transform -> Rotation
				ImGui::Text("Rotation:");

				ImGui::Separator();
				Triton::Vector3& rot = transform.Rotation;
				ImGui::Text("X:"); ImGui::SameLine(); ImGui::PushItemWidth(cFloatInputWidth); ImGui::DragFloat("##rotX", &rot.x, cFloatInputRotationSensitivity);
				ImGui::SameLine();
				ImGui::Text("Y:"); ImGui::SameLine(); ImGui::PushItemWidth(cFloatInputWidth); ImGui::DragFloat("##rotY", &rot.y, cFloatInputRotationSensitivity);
				ImGui::SameLine();
				ImGui::Text("Z:"); ImGui::SameLine(); ImGui::PushItemWidth(cFloatInputWidth); ImGui::DragFloat("##rotZ", &rot.z, cFloatInputRotationSensitivity);
				ImGui::Separator();


				//Transform -> Scale
				ImGui::Text("Scale:");

				ImGui::Separator();
				Triton::Vector3& scal = transform.Scale;
				ImGui::Text("X:"); ImGui::SameLine(); ImGui::PushItemWidth(cFloatInputWidth); ImGui::DragFloat("##scalX", &scal.x, cFloatInputSensitivity);
				ImGui::SameLine();
				ImGui::Text("Y:"); ImGui::SameLine(); ImGui::PushItemWidth(cFloatInputWidth); ImGui::DragFloat("##scalY", &scal.y, cFloatInputSensitivity);
				ImGui::SameLine();
				ImGui::Text("Z:"); ImGui::SameLine(); ImGui::PushItemWidth(cFloatInputWidth); ImGui::DragFloat("##scalZ", &scal.z, cFloatInputSensitivity);
				ImGui::Separator();
			}
		}

		// Visual
		if (registry->has<Triton::Components::Visual>(entity))
		{
			ImGui::Spacing();
			if (ImGui::CollapsingHeader("Visual"))
			{
				Triton::Components::Visual& visual = registry->get<Triton::Components::Visual>(entity);

				ImGui::Separator();

				auto& mesh = asset_manager->getAssetByID(visual.Mesh).as<Triton::Data::Mesh>();
				auto& material = asset_manager->getAssetByID(visual.Material).as<Triton::Data::Material>();

				ImGui::Text("Mesh:"); ImGui::SameLine();

				ImGui::Text(mesh->getName().c_str());
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_MESH"))
					{
						int payload_n = *(const int*)payload->Data;
						visual.Mesh = edtr_state->AllMeshes[payload_n]->getAssetID();
					}

					ImGui::EndDragDropTarget();
				}

				ImGui::Separator();
				ImGui::Spacing();
				ImGui::Spacing();

				ImGui::Text("Material:");  ImGui::SameLine();

				ImGui::Text(material->getName().c_str());
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_MATERIAL"))
					{
						int payload_n = *(const int*)payload->Data;
						visual.Material = edtr_state->AllMaterials[payload_n]->getAssetID();
					}

					ImGui::EndDragDropTarget();
				}

				ImGui::Separator();
			}
		}

		ImGui::End();
	}
}