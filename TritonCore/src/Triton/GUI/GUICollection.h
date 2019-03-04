#pragma once

#ifndef TR_DISABLE_GUI

#include "GUI.h"

namespace Triton
{
	namespace UI
	{

		class TRITON_API GUICollection
		{
		public:
			GUICollection();
			GUICollection(size_t aSize);
			~GUICollection();

			void RefreshCollection();
			void DrawCollection();

			void VisualizeCollection();
			void UpdateCollection(float aDelta);

			void AddGUI(std::shared_ptr<GUI> aGUI);
		private:
			std::vector<std::shared_ptr<GUI>> mGUIS;
		};

	}
}

#endif