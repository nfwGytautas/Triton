#include "GUIComponentVisualizer.h"

#include "TritonShell\Entity\Components\TritonComponents.h"

Triton::Tools::GUIComponentVisualizer::GUIComponentVisualizer(relay_ptr<ECS::Registry> aRegistry)
	: m_Registry(aRegistry)
{
	IsOpen = true;

	m_Entity = 0;
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

	if (ImGui::CollapsingHeader("Entity selection"))
	{
		m_Registry->each([&](auto entity) {
			char buf[32];
			sprintf(buf, "ID: %d", entity);
			if (ImGui::Selectable(buf, m_Entity == entity))
				m_Entity = entity;
		});	
	}

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Text("Components:");

	if (!m_Registry->valid(m_Entity))
	{
		ImGui::End();
		return;
	}

	//Transformation
	VisualizeTransform();

	//Visual
	VisualizeVisual();

	//Light emitter
	VisualizeLightEmitter();


	ImGui::End();
}

void Triton::Tools::GUIComponentVisualizer::Update(float aDelta)
{
}

void Triton::Tools::GUIComponentVisualizer::ShowEntity(ECS::Entity aEntity)
{
	m_Entity = aEntity;
}

void Triton::Tools::GUIComponentVisualizer::VisualizeTransform()
{
	if (m_Registry->has<Components::Transform>(m_Entity))
	{
		ImGui::Spacing();
		if (ImGui::CollapsingHeader("Transform"))
		{
			Components::Transform& transform = m_Registry->get<Components::Transform>(m_Entity);

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
}

void Triton::Tools::GUIComponentVisualizer::VisualizeVisual()
{
	if (m_Registry->has<Components::Visual>(m_Entity))
	{
		ImGui::Spacing();
		if (ImGui::CollapsingHeader("Visual"))
		{
			Components::Visual& visual = m_Registry->get<Components::Visual>(m_Entity);

			//Visual -> Mesh
			ImGui::Text("Mesh:"); ImGui::SameLine();

			size_t& mesh = visual.Mesh;
			ImGui::Text("ID:"); ImGui::SameLine(); ImGui::PushItemWidth(cFloatInputWidth); ImGui::InputInt("##mesh", (int*)&mesh, cFloatInputSensitivity);
			ImGui::Spacing();


			//Visual -> Material
			ImGui::Text("Material:"); ImGui::SameLine();

			size_t& material = visual.Material;
			ImGui::Text("ID:"); ImGui::SameLine(); ImGui::PushItemWidth(cFloatInputWidth); ImGui::InputInt("##material", (int*)&material, cFloatInputSensitivity);
			ImGui::Spacing();
		}

		ImGui::Spacing();
		ImGui::Separator();
	}
}

void Triton::Tools::GUIComponentVisualizer::VisualizeLightEmitter()
{
	if (m_Registry->has<Components::LightEmitter>(m_Entity))
	{
		ImGui::Spacing();
		if (ImGui::CollapsingHeader("Light emitter"))
		{
			Components::LightEmitter& emitter = m_Registry->get<Components::LightEmitter>(m_Entity);

			//LightEmitter -> Light
			ImGui::Text("Light:"); ImGui::SameLine();

			size_t& light = emitter.Light;
			ImGui::Text("ID:"); ImGui::SameLine(); ImGui::PushItemWidth(cFloatInputWidth); ImGui::InputInt("##mesh", (int*)&light, cFloatInputSensitivity);
			ImGui::Spacing();
		}

		ImGui::Spacing();
		ImGui::Separator();
	}
}
