#include "TRpch.h"
#include "TritonHost.h"

#include "Triton/Core/TritonClassParams.h"
#include "Triton/Logger/Log.h"

namespace Triton
{

	TritonHost::TritonHost()
	{

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
	}

	void TritonHost::registerClass(reference<Core::TritonClass>& classToRegister, Core::ClassRegisterParams classRegisterParams)
	{
		if (!m_initialized)
		{
			TR_SYSTEM_ERROR("Trying to register class with uninitialized host");
			TR_ERROR("Trying to register class with uninitialized host");
			return;
		}

		classToRegister->m_HostInstance = this;
		classToRegister->m_ID = m_nextClassId++;
		classToRegister->takeParams(classRegisterParams);
		m_IDObjectPairs[classToRegister->m_ID] = classToRegister;

		classToRegister->onRegistered();
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

#if TR_STRING_REPRESENTATIONS == 1

	reference<Core::TritonClass> TritonHost::getClassByName(std::string name)
	{
		auto& it = std::find_if(m_registeredClasses.begin(), m_registeredClasses.end(), [&](auto& TClass) {return TClass->m_Name == name; });

		if (it != m_registeredClasses.end())
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
		auto& it = std::find_if(m_registeredClasses.begin(), m_registeredClasses.end(), [&](auto& TClass) {return TClass->m_Name == name; });

		if (it != m_registeredClasses.end())
		{
			TR_ERROR("Object '{0}' does not exist!", name);
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

	std::string TritonHost::getClassName(size_t id)
	{
		if (checkReturnability(id, -1))
		{
			return m_IDObjectPairs[id]->m_Name;
		}
		else
		{
			return false;
		}
	}

#endif

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

}