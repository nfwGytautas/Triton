#include "TRpch.h"
#include "SceneManager.h"

#include "TritonPlatform/PlatformAbstraction.h"

namespace Triton
{
	namespace Manager
	{

		SceneManager::SceneManager(PType::Context* context)
			: m_Context(context)
		{
		}

		SceneManager::~SceneManager()
		{
			cleanUp();
		}

		reference<Scene> SceneManager::createScene()
		{
			reference<Scene> scene(new Scene(m_Context));
			m_Scenes.push_back(scene.as<SceneBase>());
			return scene;
		}

		void SceneManager::cleanUp()
		{
			for (unsigned int i = 0; i < m_Scenes.size(); i++)
			{
				m_Scenes[i]->destroy();
			}
		}

	}
}