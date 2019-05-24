#pragma once

#include "Triton/Core/Scene.h"

namespace Triton
{
	namespace Manager
	{
		class SceneManager
		{
		public:
			SceneManager(relay_ptr<PType::Context> context);
			virtual ~SceneManager();

			relay_ptr<Scene> createScene();

			void cleanUp();
		private:
			relay_ptr<PType::Context> m_Context;

			std::vector<Scene*> m_Scenes;
		};
	}
}