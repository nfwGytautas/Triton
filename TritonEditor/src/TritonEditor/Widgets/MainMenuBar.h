#pragma once


#include <string>

#include <imgui.h>

#include "TritonPlatform/PlatformAbstraction.h"
#include "TritonPlatform/CrossTypes/Texture.h"
#include "TritonPlatform/DirectX/DXTypes.h"
#include "Triton/Core/Wrapers/Viewport.h"

void showMainMenuBar(Triton::EditorScene::WidgetIsOpen& isOpen)
{
	if (ImGui::BeginMainMenuBar())
	{
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

		ImGui::EndMainMenuBar();
	}
}