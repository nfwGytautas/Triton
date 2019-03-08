#include "GUIComponentVisualizer.h"

#include "TritonShell\Entity\Components\TritonComponents.h"

Triton::Tools::GUIComponentVisualizer::GUIComponentVisualizer(std::shared_ptr<ECS::Registry> aRegistry)
	: mRegistry(aRegistry)
{
	IsOpen = true;

	mEntity = 0;
}


Triton::Tools::GUIComponentVisualizer::~GUIComponentVisualizer()
{
}

void Triton::Tools::GUIComponentVisualizer::Visualize()
{
	//ImGui::ShowDemoWindow(&IsOpen);

	//Setup

	if (!IsOpen)
	{
		return;
	}

	ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("Entity component visualizer", &IsOpen))
	{
		ImGui::End();
		return;
	}

	

	//Transformation
	if (mRegistry->has<Components::Transform>(mEntity))
	{
		ImGui::Spacing();
		if (ImGui::CollapsingHeader("Transform"))
		{
			Components::Transform& transform = mRegistry->get<Components::Transform>(mEntity);

			//Transform -> Position
			ImGui::Text("Position:"); ImGui::SameLine();

			Vector3& pos = transform.Position;
			ImGui::Text("X:"); ImGui::SameLine(); ImGui::PushItemWidth(cFloatInputWidth); ImGui::DragFloat("##posX", &pos.x, cFloatInputSensitivity);
			ImGui::SameLine();
			ImGui::Text("Y:"); ImGui::SameLine(); ImGui::PushItemWidth(cFloatInputWidth); ImGui::DragFloat("##posY", &pos.y, cFloatInputSensitivity);
			ImGui::SameLine();
			ImGui::Text("Z:"); ImGui::SameLine(); ImGui::PushItemWidth(cFloatInputWidth); ImGui::DragFloat("##posZ", &pos.z, cFloatInputSensitivity);
			ImGui::Spacing();


			//Transform -> Rotation
			ImGui::Text("Rotation:"); ImGui::SameLine();

			Vector3& rot = transform.Rotation;
			ImGui::Text("X:"); ImGui::SameLine(); ImGui::PushItemWidth(cFloatInputWidth); ImGui::DragFloat("##rotX", &rot.x, cFloatInputRotationSensitivity);
			ImGui::SameLine();
			ImGui::Text("Y:"); ImGui::SameLine(); ImGui::PushItemWidth(cFloatInputWidth); ImGui::DragFloat("##rotY", &rot.y, cFloatInputRotationSensitivity);
			ImGui::SameLine();
			ImGui::Text("Z:"); ImGui::SameLine(); ImGui::PushItemWidth(cFloatInputWidth); ImGui::DragFloat("##rotZ", &rot.z, cFloatInputRotationSensitivity);
			ImGui::Spacing();


			//Transform -> Scale
			ImGui::Text("   Scale:"); ImGui::SameLine();

			Vector3& scal = transform.Scale;
			ImGui::Text("X:"); ImGui::SameLine(); ImGui::PushItemWidth(cFloatInputWidth); ImGui::DragFloat("##scalX", &scal.x, cFloatInputSensitivity);
			ImGui::SameLine();
			ImGui::Text("Y:"); ImGui::SameLine(); ImGui::PushItemWidth(cFloatInputWidth); ImGui::DragFloat("##scalY", &scal.y, cFloatInputSensitivity);
			ImGui::SameLine();
			ImGui::Text("Z:"); ImGui::SameLine(); ImGui::PushItemWidth(cFloatInputWidth); ImGui::DragFloat("##scalZ", &scal.z, cFloatInputSensitivity);
			ImGui::Spacing();
		}

		ImGui::Spacing();
		ImGui::Separator();
	}

	//MeshFilter
	if (mRegistry->has<Components::MeshFilter>(mEntity))
	{
		ImGui::Spacing();
		if (ImGui::CollapsingHeader("Mesh filter"))
		{
			Components::MeshFilter& mFilter = mRegistry->get<Components::MeshFilter>(mEntity);

			//Transform -> Position
			ImGui::Text("Mesh:"); ImGui::SameLine(); ImGui::Text(mFilter.Mesh->GetPath().c_str());
		}

		ImGui::Spacing();
		ImGui::Separator();
	}

	//MeshRenderer
	if (mRegistry->has<Components::MeshRenderer>(mEntity))
	{
		ImGui::Spacing();
		if (ImGui::CollapsingHeader("Mesh renderer"))
		{
			Components::MeshRenderer& mRenderer = mRegistry->get<Components::MeshRenderer>(mEntity);

			//Transform -> Position
			ImGui::Text("Material:"); ImGui::SameLine(); ImGui::Text(mRenderer.Material->GetTexture()->GetPath().c_str());
		}

		ImGui::Spacing();
		ImGui::Separator();
	}

	ImGui::End();
}

void Triton::Tools::GUIComponentVisualizer::Update(float aDelta)
{
}

void Triton::Tools::GUIComponentVisualizer::ShowEntity(ECS::Entity aEntity)
{
	mEntity = aEntity;
}
