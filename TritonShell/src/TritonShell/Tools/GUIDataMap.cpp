#include "GUIDataMap.h"



Triton::Tools::GUIDataMap::GUIDataMap(std::shared_ptr<Core::DataMap> aDMap)
	: m_DMap(aDMap)
{
	IsOpen = false;
}


Triton::Tools::GUIDataMap::~GUIDataMap()
{
}

void Triton::Tools::GUIDataMap::Visualize()
{
	if (!IsOpen)
	{
		return;
	}

	ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("Data map", &IsOpen))
	{
		ImGui::End();
		return;
	}

	//Meshes
	std::vector<std::string> meshKeys = m_DMap->GetMeshes();
	if (meshKeys.size() > 0)
	{
		ImGui::Spacing();
		if (ImGui::CollapsingHeader("Meshes"))
		{
			for (std::string key : meshKeys)
			{
				if (ImGui::Button(key.c_str()))
				{
					m_SelectedMesh = key;
				}
			}
		}

		ImGui::Spacing();
		ImGui::Separator();
	}

	ImGui::End();
}

void Triton::Tools::GUIDataMap::Update(float aDelta)
{
}

Triton::Data::Mesh& Triton::Tools::GUIDataMap::GetSelected()
{
	return m_DMap->GetMesh(m_SelectedMesh);
}
