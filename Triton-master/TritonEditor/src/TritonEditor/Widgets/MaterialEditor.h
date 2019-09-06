#pragma once

#include <string>

#include <imgui.h>

#include "TritonPlatform/PlatformAbstraction.h"
#include "TritonPlatform/CrossTypes/Texture.h"
#include "TritonPlatform/DirectX/DXTypes.h"
#include "Triton/Core/Wrapers/Viewport.h"

void showMaterialEditor(bool* p_open,
	Triton::reference<Triton::EditorState>& edtr_state,
	Triton::reference<Triton::Manager::AssetManager>& asset_manager)
{
	static char name[64] = "";
	static std::string* material_name = nullptr;

	material_name = &edtr_state->CurrentEditMaterial.Name;
	std::strcpy(name, material_name->c_str());

	auto& material = edtr_state->CurrentMaterial;

	bool valid = material.valid();

	bool hasTexture = false;
	Triton::reference<Triton::PType::Texture> texture = nullptr;

	if (valid)
	{
		if (material->Texture.valid())
		{
			hasTexture = true;
			texture = material->Texture;
		}

		if (material->Shader.valid())
		{
			edtr_state->CurrentEditMaterial.Shader = material->Shader;
		}

		edtr_state->CurrentEditMaterial.Ambient = material->Ambient;
		edtr_state->CurrentEditMaterial.Diffuse = material->Diffuse;
		edtr_state->CurrentEditMaterial.Specular = material->Specular;
		edtr_state->CurrentEditMaterial.Shininess = material->Shininess;

		material_name = &material->getNameRef();
		std::strcpy(name, material_name->c_str());
	}
	else
	{
		if(edtr_state->CurrentEditMaterial.Texture.valid())
		{
			texture = edtr_state->CurrentEditMaterial.Texture;
			hasTexture = texture.valid();
		}

		material_name = &edtr_state->CurrentEditMaterial.Name;
	}

	if (*p_open == true)
	{
		ImGui::SetNextWindowSize(ImVec2(800, 200), ImGuiCond_FirstUseEver);
		ImGui::Begin("Material editor", p_open);

		ImVec2 im_size = ImGui::GetWindowSize();
		
		if (ImGui::Button("New material", ImVec2(im_size.x, 0)))
		{
			edtr_state->CurrentMaterial = nullptr;

			edtr_state->CurrentEditMaterial.Texture = nullptr;
			edtr_state->CurrentEditMaterial.Name = "new material";

			edtr_state->CurrentEditMaterial.Ambient = Triton::Vector3();
			edtr_state->CurrentEditMaterial.Diffuse = Triton::Vector3();
			edtr_state->CurrentEditMaterial.Specular = Triton::Vector3();
			edtr_state->CurrentEditMaterial.Shininess = 0;

			material_name = &edtr_state->CurrentEditMaterial.Name;
			std::strcpy(name, material_name->c_str());
		}

		if (ImGui::Button("Save material", ImVec2(im_size.x, 0)))
		{
			// Check if all fields are filled

			if (edtr_state->CurrentEditMaterial.Texture.valid() &&
				edtr_state->CurrentEditMaterial.Name != "" && 
				edtr_state->CurrentEditMaterial.Shader.valid() &&
				edtr_state->CurrentEditMaterial.Shader->Program.valid())
			{
				Triton::Resource::AssetCreateParams asset_desc;

				asset_desc.Type = Triton::Resource::AssetCreateParams::AssetType::MATERIAL;
				asset_desc.PType = edtr_state->CurrentEditMaterial.Texture.as<Triton::PType::PlatformObject>();
				asset_desc.Name = *material_name;

				auto mat = asset_manager->createAsset(asset_desc).as<Triton::Data::Material>();

				mat->Shader = edtr_state->CurrentEditMaterial.Shader;

				mat->Ambient = edtr_state->CurrentEditMaterial.Ambient;
				mat->Diffuse = edtr_state->CurrentEditMaterial.Diffuse;
				mat->Specular = edtr_state->CurrentEditMaterial.Specular;
				mat->Shininess = edtr_state->CurrentEditMaterial.Shininess;

				edtr_state->AllMaterials.push_back(mat);
			}
		}

		ImGui::Separator();

		ImGui::Text("Name:"); ImGui::SameLine();

		ImGui::InputText("##editName", name, 64, ImGuiInputTextFlags_CharsNoBlank);

		if (material_name != nullptr)
		{
			*material_name = name;
		}

		ImGui::Spacing();

		ImGui::Columns(2, "mycolumns");

		if (hasTexture)
		{
			if (ImGui::ImageButton(texture->raw_texture(),
				ImVec2(200, 200),
				ImVec2(1, 1),
				ImVec2(0, 0),
				5,
				ImVec4(0.0f, 0.0f, 0.0f, 1.0f)))
			{

			}

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_TEXTURE"))
				{
					int payload_n = *(const int*)payload->Data;
					edtr_state->CurrentEditMaterial.Texture = edtr_state->AllTextures[payload_n]->Texture;
				}

				ImGui::EndDragDropTarget();
			}
		}
		else
		{
			if (ImGui::Button("##EmptyTexture",
				ImVec2(205, 205)))
			{

			}

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_TEXTURE"))
				{
					int payload_n = *(const int*)payload->Data;
					edtr_state->CurrentEditMaterial.Texture = edtr_state->AllTextures[payload_n]->Texture;
				}

				ImGui::EndDragDropTarget();
			}
		}

		//ImGui::SameLine();
		ImGui::NextColumn();
		
		if (ImGui::Button("Shader",
			ImVec2(ImGui::GetContentRegionAvail().x, 0)))
		{

		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_SHADER"))
			{
				int payload_n = *(const int*)payload->Data;
				edtr_state->CurrentEditMaterial.Shader = edtr_state->AllShaders[payload_n];
			}

			ImGui::EndDragDropTarget();
		}

		if(edtr_state->CurrentEditMaterial.Shader.valid() &&
			edtr_state->CurrentEditMaterial.Shader->getName() != "")
		{
			ImGui::Text(edtr_state->CurrentEditMaterial.Shader->getName().c_str());
		}
		else
		{
			ImGui::Text("not set");
		}

		ImGui::Spacing();
		
		ImGuiColorEditFlags misc_flags = 0;
		
		if (ImGui::ColorEdit3("Ambient", (float*)&edtr_state->CurrentEditMaterial.Ambient, misc_flags) && valid)
		{
			edtr_state->CurrentMaterial->Ambient = edtr_state->CurrentEditMaterial.Ambient;
		}

		if (ImGui::ColorEdit3("Diffuse", (float*)&edtr_state->CurrentEditMaterial.Diffuse, misc_flags) && valid)
		{
			edtr_state->CurrentMaterial->Diffuse = edtr_state->CurrentEditMaterial.Diffuse;
		}

		if (ImGui::ColorEdit3("Specular", (float*)&edtr_state->CurrentEditMaterial.Specular, misc_flags) && valid)
		{
			edtr_state->CurrentMaterial->Specular = edtr_state->CurrentEditMaterial.Specular;
		}

		if (ImGui::SliderFloat("Shininess: ", &edtr_state->CurrentEditMaterial.Shininess, 0.0, 100) && valid)
		{
			edtr_state->CurrentMaterial->Shininess = edtr_state->CurrentEditMaterial.Shininess;
		}

		ImGui::Columns(1);

		ImGui::Separator();

		ImGui::End();
	}
}