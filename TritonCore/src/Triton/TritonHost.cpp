#include "TRpch.h"
#include "TritonHost.h"

#include "Triton/Core/TritonClassParams.h"
#include "Triton/Logger/Log.h"

namespace Triton
{
	TritonHost::TritonHost()
	{
		// Fills priority vectors with empty data
		m_priorities.update.resize(Layers::c_maxLayers);
		m_priorities.render.resize(Layers::c_maxLayers);
		m_priorities.preRender.resize(Layers::c_maxLayers);
	}

	void TritonHost::init(HostInitializationParams initParams)
	{
		Triton::Log::Init();

		m_initialized = true;

		reference<Core::InputManager> iManager = new Core::InputManager();
		registerClass(iManager.as<Core::TritonClass>(), Core::InputRegisterParams);

		reference<PType::Context> context = Impl::createContext(initParams.AppSettings);
		registerClass(context.as<Core::TritonClass>(), Core::ContextRegisterParams);

		reference<Core::TritonCache> cache = new Core::TritonCache();
		registerClass(cache.as<Core::TritonClass>(), Core::CacheRegisterParams);

		reference<Manager::AssetManager> assetManager = new Manager::AssetManager();
		registerClass(assetManager.as<Core::TritonClass>(), Core::AssetManagerRegisterParams);

		reference<Manager::ThreadManager> threadManager = new Manager::ThreadManager();
		registerClass(threadManager.as<Core::TritonClass>(), Core::ThreadManagerRegisterParams);

		reference<Core::RenderBuffer> mainRenderbuffer = new Core::RenderBuffer();
		registerClass(mainRenderbuffer.as<Core::TritonClass>(), Core::MainRenderBufferRegisterParams);

		reference<Utility::Timer> timer = new Utility::Timer();
		registerClass(timer.as<Core::TritonClass>(), Core::TimerRegisterParams);

		reference<GameWindow> gWindow = new GameWindow();
		registerClass(gWindow.as<Core::TritonClass>(), Core::GameWindowRegisterParams);

		reference<Script::ScriptEngine> scriptEngine = new Script::ScriptEngine();
		registerClass(scriptEngine.as<Core::TritonClass>(), Script::ScriptEngineRegisterParams);

		m_settings.render = !initParams.CustomRenderFunction;
		m_settings.updating = !initParams.CustomUpdateFunction;
	}

	void TritonHost::registerClass(reference<Core::TritonClass> classToRegister, Core::ClassRegisterParams classRegisterParams)
	{
		if (!m_initialized)
		{
			TR_SYSTEM_ERROR("Trying to register class with uninitialized host");
			TR_ERROR("Trying to register class with uninitialized host");
			return;
		}

		// Fill class with data
		classToRegister->m_HostInstance = this;
		classToRegister->m_ID = m_nextClassId++;
		classToRegister->takeParams(classRegisterParams);

		// Add class to trackers
		m_IDObjectPairs[classToRegister->m_ID] = classToRegister;
		m_registeredClasses.push_back(classToRegister);

		// Add class to layers
		if (classToRegister->m_messages & Core::ReceivedMessages::PreRender 
			&& classToRegister->m_layerSettings.PreRender < Layers::c_maxLayers)
		{
			m_priorities.preRender[classToRegister->m_layerSettings.PreRender].push_back(classToRegister);
		}
		if (classToRegister->m_messages & Core::ReceivedMessages::Update
			&& classToRegister->m_layerSettings.Update < Layers::c_maxLayers)
		{
			m_priorities.update[classToRegister->m_layerSettings.Update].push_back(classToRegister);
		}
		if (classToRegister->m_messages & Core::ReceivedMessages::Render
			&& classToRegister->m_layerSettings.Render < Layers::c_maxLayers)
		{
			m_priorities.render[classToRegister->m_layerSettings.Render].push_back(classToRegister);
		}

		// Send a registered class message
		if (classToRegister->m_messages & Core::ReceivedMessages::ClassRegistered)
		{
			classToRegister->onMessage((size_t)Triton::Core::TritonMessageType::ClassRegistered, nullptr);
		}
	}

	reference<Core::TritonClass> TritonHost::getClassByID(size_t id)
	{
		if (checkReturnability(id, -1))
		{
			return m_IDObjectPairs[id];
		}
		else
		{
			return nullptr;
		}
	}

	reference<Core::TritonClass> TritonHost::getClassByID(size_t id, size_t requester)
	{
		if (checkReturnability(id, requester))
		{
			return m_IDObjectPairs[id];
		}
		else
		{
			return nullptr;
		}
	}

	void TritonHost::postMessage(int message, void* payload)
	{
		// Check if the message is custom or not
		bool customMsg = (message & Core::c_BuiltInMessageMask) == 0;

		for (auto& TClass : m_registeredClasses)
		{
			if (customMsg)
			{
				if (TClass->m_messages & Core::ReceivedMessages::Custom)
				{
					TClass->onMessage(message, payload);
				}
			}
			else
			{
				if (TClass->m_messages & message)
				{
					TClass->onMessage(message, payload);
				}
			}
		}
	}

	void TritonHost::postMessage(int message, std::vector<reference<Core::TritonClass>>& classesToSendTo, void* payload)
	{
		// Check if the message is custom or not
		bool customMsg = (message & Core::c_BuiltInMessageMask) == 0;

		for (auto& TClass : classesToSendTo)
		{
			if (customMsg)
			{
				if (TClass->m_messages & Core::ReceivedMessages::Custom)
				{
					TClass->onMessage(message, payload);
				}
			}
			else
			{
				if (TClass->m_messages & message)
				{
					TClass->onMessage(message, payload);
				}
			}
		}
	}

#if TR_STRING_REPRESENTATIONS == 1

	reference<Core::TritonClass> TritonHost::getClassByName(std::string name)
	{
		auto& it = std::find_if(m_registeredClasses.begin(), m_registeredClasses.end(), [&](auto& TClass) {return TClass->m_Name == name; });

		if (it == m_registeredClasses.end())
		{
			TR_ERROR("Object '{0}' does not exist!", name);
			return nullptr;
		}

		if (checkReturnability((*it)->m_ID, -1))
		{
			return m_IDObjectPairs[(*it)->m_ID];
		}
		else
		{
			return nullptr;
		}
	}

	reference<Core::TritonClass> TritonHost::getClassByName(std::string name, size_t requester)
	{
		auto& it = std::find_if(m_registeredClasses.begin(), m_registeredClasses.end(), [&](auto& TClass)
		{
			return TClass->m_Name == name;
		});

		if (it == m_registeredClasses.end())
		{
			TR_ERROR("Object '{0}' does not exist!", name);
			return nullptr;
		}

		if (!it->valid())
		{
			TR_ERROR("Object '{0}' is invalid!", name);
			return nullptr;
		}

		if (checkReturnability((*it)->m_ID, requester))
		{
			return m_IDObjectPairs[(*it)->m_ID];
		}
		else
		{
			return nullptr;
		}
	}

	void TritonHost::isRendering(bool state)
	{
		m_settings.rendering = state;
	}

#endif

	void TritonHost::isUpdating(bool state)
	{
		m_settings.updating = state;
	}

	void TritonHost::start()
	{
		if (m_settings.render)
		{
			this->getClassByID((size_t)Core::TritonClasses::ThreadManager).as<Manager::ThreadManager>()->
				newRenderFunc(std::bind(&TritonHost::renderMT, this));
		}

		if (m_settings.update)
		{
			updateMT();
		}
	}

	void TritonHost::startST()
	{
		auto& context = this->getClassByID((size_t)Core::TritonClasses::Context).as<PType::Context>();
		while(!context->window->windowClosed())
		{
			updateST();
			renderST();
		}
	}

	bool TritonHost::checkReturnability(size_t id, size_t requester)
	{
		auto& TClass = m_IDObjectPairs[id];

		if (m_IDObjectPairs.find(id) == m_IDObjectPairs.end())
		{
			TR_ERROR("Object with id: '{0}' doesn't exist, make sure the class was registered", id);
			return false;
		}

		if (requester != -1 && m_IDObjectPairs.find(requester) == m_IDObjectPairs.end())
		{
			TR_ERROR("Requester with id: '{0}' doesn't exist, make sure the class was registered", requester);
			return false;
		}

		if (!TClass->m_Protected)
		{
			return true;
		}
		else
		{
			if (std::find(TClass->m_Receivers.begin(), TClass->m_Receivers.end(), requester) != TClass->m_Receivers.end())
			{
				return true;
			}
			else
			{
#if TR_STRING_REPRESENTATIONS == 1
				TR_ERROR("Object '{0}' is not allowed to access protected class '{1}'", m_IDObjectPairs[requester]->m_Name, m_IDObjectPairs[id]->m_Name);
#else
				TR_ERROR("Object '{0}' is not allowed to access protected class '{1}'", requester, id);
#endif
				return false;
			}
		}
	}

	void TritonHost::updateMT()
	{
		auto& context = this->getClassByID((size_t)Core::TritonClasses::Context).as<PType::Context>();

		while (m_settings.updating)
		{
			for (int i = 0; i < Layers::c_maxLayers; i++)
			{
				postMessage((size_t)Core::TritonMessageType::Update, m_priorities.update[i], nullptr);
			}

			if (context->window->windowClosed())
			{
				m_settings.updating = false;
				m_settings.rendering = false;
			}
		}
	}

	void TritonHost::renderMT()
	{
		while (m_settings.rendering)
		{
			for (int i = 0; i < Layers::c_maxLayers; i++)
			{
				postMessage((size_t)Core::TritonMessageType::PreRender, m_priorities.preRender[i], nullptr);
			}

			for (int i = 0; i < Layers::c_maxLayers; i++)
			{
				postMessage((size_t)Core::TritonMessageType::Render, m_priorities.render[i], nullptr);
			}
		}
	}

	void TritonHost::updateST()
	{
		for (int i = 0; i < Layers::c_maxLayers; i++)
		{
			postMessage((size_t)Core::TritonMessageType::Update, m_priorities.update[i], nullptr);
		}
	}

	void TritonHost::renderST()
	{
		for (int i = 0; i < Layers::c_maxLayers; i++)
		{
			postMessage((size_t)Core::TritonMessageType::PreRender, m_priorities.preRender[i], nullptr);
			postMessage((size_t)Core::TritonMessageType::Render, m_priorities.render[i], nullptr);
		}
	}
}