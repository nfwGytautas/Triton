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

	const float popupWidth = 150;

	auto& entity = edtr_state->CurrentGameObject;

	if (*p_open == true)
	{
		ImGui::SetNextWindowSize(ImVec2(322, 498));
		ImGui::Begin("Inspector", p_open, ImGuiWindowFlags_NoResize);

		ImVec2 im_size = ImGui::GetWindowSize();

		if (ImGui::Button("Add component", ImVec2(im_size.x, 0)))
		{
			ImGui::OpenPopup("my_select_popup");
		}

		ImGui::SameLine();

		ImGui::PushItemWidth(popupWidth);
		if (ImGui::BeginPopup("my_select_popup"))
		{
			ImGui::Text("Component");
			ImGui::Separator();

			if (!entity->hasComponent<Triton::Components::Transform>() && ImGui::Button("Transform", ImVec2(popupWidth, 0)))
			{
				entity->addComponent<Triton::Components::Transform>();
				ImGui::CloseCurrentPopup();
			}

			if (!entity->hasComponent<Triton::Components::Visual>() && ImGui::Button("Visual", ImVec2(popupWidth, 0)))
			{
				entity->addComponent<Triton::Components::Visual>();
				ImGui::CloseCurrentPopup();
			}

			if (!entity->hasComponent<Triton::Components::LightEmitter>() && ImGui::Button("Light emitter", ImVec2(popupWidth, 0)))
			{
				entity->addComponent<Triton::Components::LightEmitter>();
				ImGui::CloseCurrentPopup();
			}

			if (!entity->hasComponent<Triton::Components::Image>() && ImGui::Button("Image", ImVec2(popupWidth, 0)))
			{
				entity->addComponent<Triton::Components::Image>();
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		ImGui::Spacing();

		// Transformation
		if (entity->hasComponent<Triton::Components::Transform>())
		{
			ImGui::Spacing();
			if (ImGui::CollapsingHeader("Transform"))
			{
				Triton::Components::Transform& transform = entity->getComponent<Triton::Components::Transform>();

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
				if (rot.x > 360)
				{
					rot.x = rot.x - 360;
				}
				if (rot.y > 360)
				{
					rot.y = rot.y - 360;
				}
				if (rot.z > 360)
				{
					rot.z = rot.z - 360;
				}
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
		if (entity->hasComponent<Triton::Components::Visual>())
		{
			ImGui::Spacing();
			if (ImGui::CollapsingHeader("Visual"))
			{
				Triton::Components::Visual& visual = entity->getComponent<Triton::Components::Visual>();

				ImGui::Separator();

				ImGui::Checkbox("Visible", &visual.Visible);

				ImGui::Separator();

				if (asset_manager->valid(visual.Mesh))
				{
					auto& mesh = asset_manager->getAssetByID(visual.Mesh).as<Triton::Data::Mesh>();

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
				}
				else
				{
					ImGui::Text("Mesh:"); ImGui::SameLine();

					ImGui::Text("invalid");
					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_MESH"))
						{
							int payload_n = *(const int*)payload->Data;
							visual.Mesh = edtr_state->AllMeshes[payload_n]->getAssetID();
						}

						ImGui::EndDragDropTarget();
					}
				}

				ImGui::Separator();
				ImGui::Spacing();
				ImGui::Spacing();

				if (asset_manager->valid(visual.Material))
				{
					auto& material = asset_manager->getAssetByID(visual.Material).as<Triton::Data::Material>();

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
				}
				else
				{
					ImGui::Text("Material:");  ImGui::SameLine();

					ImGui::Text("invalid");
					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_MATERIAL"))
						{
							int payload_n = *(const int*)payload->Data;
							visual.Material = edtr_state->AllMaterials[payload_n]->getAssetID();
						}

						ImGui::EndDragDropTarget();
					}
				}

				ImGui::Separator();
			}
		}

		ImGui::End();
	}
}