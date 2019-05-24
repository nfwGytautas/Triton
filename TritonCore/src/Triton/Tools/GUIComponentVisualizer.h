//#pragma once
//
//#include "TritonShell\Entity\Registry.h"
//
//#include "TritonTypes\relay_ptr.h"
//
//namespace Triton
//{
//	namespace Tools
//	{
//		class GUIComponentVisualizer : public UI::GUI
//		{
//		public:
//			GUIComponentVisualizer(relay_ptr<ECS::Registry> aRegistry);
//			~GUIComponentVisualizer();
//
//			virtual void Visualize() override;
//
//			virtual void Update(float aDelta) override;
//			
//			virtual void ShowEntity(ECS::Entity aEntity);
//
//		private:
//			void VisualizeTransform();
//			void VisualizeVisual();
//			void VisualizeLightEmitter();
//		private:
//			relay_ptr<ECS::Registry> m_Registry;
//			ECS::Entity m_Entity;
//
//		private:
//			const float cFloatInputWidth = 75;
//			const float cFloatInputSensitivity = 0.01f;
//			const float cFloatInputRotationSensitivity = cFloatInputSensitivity * 10;
//		};
//
//	}
//}