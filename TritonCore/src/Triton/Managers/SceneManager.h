#pragma once

#include "Triton/Core/Scene.h"

namespace Triton
{
	namespace Manager
	{
		class SceneManager
		{
		public:
			SceneManager(PType::Context* context);
			virtual ~SceneManager();

			relay_ptr<Scene> createScene();

			template <class T, typename... Args>
			relay_ptr<T> createSceneCustom(Args&&... args)
			{
				T* scene = new T(m_Context, std::forward<Args>(args)...);
				m_Scenes.push_back(scene);
				return scene;
			}

			void cleanUp();
		private:
			PType::Context* m_Context;

			std::vector<SceneBase*> m_Scenes;
		};
	}
}