#pragma once

#include "Triton/Core/Scene.h"

namespace Triton
{
	namespace Manager
	{
		class SceneManager
		{
		public:
			SceneManager(PType::Context* context, reference<Core::InputManager> iManager);
			virtual ~SceneManager();

			reference<Scene> createScene();

			template <class T, typename... Args>
			reference<T> createSceneCustom(Args&&... args)
			{
				reference<T> scene(new T(m_Context, m_iManager, std::forward<Args>(args)...));
				m_Scenes.push_back(scene.as<SceneBase>());
				return scene;
			}

			void cleanUp();
		private:
			PType::Context* m_Context;
			reference<Core::InputManager> m_iManager;

			std::vector<reference<SceneBase>> m_Scenes;
		};
	}
}