#pragma once


#include <string>

#include <imgui.h>

#include "TritonPlatform/PlatformAbstraction.h"
#include "TritonPlatform/CrossTypes/Texture.h"
#include "TritonPlatform/DirectX/DXTypes.h"
#include "Triton/Core/Wrapers/Viewport.h"

void showMainMenuBar(Triton::EditorScene::WidgetIsOpen& isOpen, Triton::EditorScene::_Callbacks& callbacks, Triton::reference<Triton::EditorState> state)
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::Button("Open project"))
			{
				callbacks.OpenProject();
			}

			if (ImGui::Button("Save project"))
			{
				callbacks.SaveProject();
			}

			if (ImGui::Button("New project"))
			{
				state->TextInputCallback = callbacks.NewProject;
				state->ShowTextModal = true;
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			ImGui::Checkbox("Asset window", &isOpen.assetWindow);
			ImGui::Checkbox("Inspector", &isOpen.inspectorWindow);
			ImGui::Checkbox("Log", &isOpen.logWindow);
			ImGui::Checkbox("Metrics", &isOpen.metrics);
			ImGui::Checkbox("Scene explorer", &isOpen.sceneView);
			ImGui::Checkbox("Viewport", &isOpen.viewport);
			ImGui::Checkbox("Material editor", &isOpen.materialViewport);

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Operation"))
		{
			if(ImGui::Button("Compile"))
			{
				callbacks.CompileScripts();
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}