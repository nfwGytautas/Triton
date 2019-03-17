#pragma once

#include "Triton\GUI\GUI.h"
#include "TritonShell\Entity\Registry.h"
#include "TritonShell\Tools\GUIDataMap.h"


namespace Triton
{
	namespace Tools
	{
		class GUIComponentVisualizer : public UI::GUI
		{
		public:
			GUIComponentVisualizer(std::shared_ptr<ECS::Registry> aRegistry, std::shared_ptr<GUIDataMap> aGUIDmap);
			~GUIComponentVisualizer();

			virtual void Visualize() override;

			virtual void Update(float aDelta) override;
			
			virtual void ShowEntity(ECS::Entity aEntity);
		private:
			std::shared_ptr<ECS::Registry> m_Registry;
			std::shared_ptr<GUIDataMap> m_GUIDmap;
			ECS::Entity m_Entity;

		private:
			const float cFloatInputWidth = 75;
			const float cFloatInputSensitivity = 0.01f;
			const float cFloatInputRotationSensitivity = cFloatInputSensitivity * 10;
		};

	}
}