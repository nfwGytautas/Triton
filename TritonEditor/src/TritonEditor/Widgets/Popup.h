#pragma once

void popup_proj(Triton::reference<Triton::EditorState> edtrState)
{
	static char name[64] = "";

	if (edtrState->ShowTextModal)
	{
		ImGui::OpenPopup("New name");

		ImGui::SetNextWindowSize(ImVec2(265, 0));
		if (ImGui::BeginPopupModal("New name", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Enter the name of the project:\n\n");

			ImGui::PushItemWidth(248);
			ImGui::InputText("##editProjectName", name, 64, ImGuiInputTextFlags_CharsNoBlank);

			if (ImGui::Button("OK", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
				edtrState->ShowTextModal = false;
				edtrState->TextInputCallback(name);
			}

			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
				edtrState->ShowTextModal = false;
			}
			ImGui::EndPopup();
		}
	}
}

void popup_mBox(Triton::reference<Triton::EditorState> edtrState)
{
	if (edtrState->ShowBox)
	{
		ImGui::OpenPopup("MBox");

		if (ImGui::BeginPopupModal("MBox", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text(edtrState->BoxContent.c_str());
			ImGui::Separator();

			if (ImGui::Button("OK", ImVec2(-1, 0))) 
			{ 
				ImGui::CloseCurrentPopup();
				edtrState->ShowBox = false;
			}
			ImGui::EndPopup();
		}
	}
}

void showPopup(Triton::reference<Triton::EditorState> edtrState)
{
	popup_proj(edtrState);
	popup_mBox(edtrState);
}