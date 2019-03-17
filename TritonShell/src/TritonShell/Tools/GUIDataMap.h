#pragma once

#include "Triton\GUI\GUI.h"
#include "Triton\Core\Data\DataMap.h"

namespace Triton
{
	namespace Tools
	{

		class GUIDataMap : public UI::GUI
		{
		public:
			GUIDataMap(std::shared_ptr<Core::DataMap> aDMap);
			~GUIDataMap();

			virtual void Visualize() override;

			virtual void Update(float aDelta) override;

			Triton::Data::Mesh& GetSelected();

		private:
			std::shared_ptr<Core::DataMap> m_DMap;		

			std::string m_SelectedMesh;
		};

	}
}