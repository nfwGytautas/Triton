#pragma once

#include "Triton/Config.h"

#include "TritonTypes\reference.h"
#include "TritonTypes\relay_ptr.h"

#include "Triton\Core\TritonClass.h"

// Triton classes included in the order they are registered in the host
#include "Triton\Core\Input\InputManager.h"
#include "TritonPlatform\PlatformAbstraction.h"
#include "Triton\Managers\TritonCache.h"
#include "Triton\Managers\AssetManager.h"

namespace Triton 
{
	// Specifies how the Triton host should be initialized
	struct HostInitializationParams
	{
		AppSettings AppSettings;
	};

	// One of the main classes of the engine the host is responsible for initializing
	// the graphics context creating windows, Triton classes and sinks
	class TritonHost
	{
	public:
		TritonHost();
		
		// Initializes the host and all the necessary classes for the engine
		void init(HostInitializationParams initParams);

		// Register a Triton class with the host with specified register params
		void registerClass(reference<Core::TritonClass>& classToRegister, Core::ClassRegisterParams classRegisterParams);

		// Returns a reference to class object with specified id
		reference<Core::TritonClass> getClassByID(size_t id);

		// Returns a reference to protected class object with specified id with requester id
		reference<Core::TritonClass> getClassByID(size_t id, size_t requester);

#if TR_STRING_REPRESENTATIONS == 1
		// Returns a reference to class object with specified name
		reference<Core::TritonClass> getClassByName(std::string name);

		// Returns a reference to protected class object with specified id with requester id
		reference<Core::TritonClass> getClassByName(std::string name, size_t requester);

		// Returns the string representation of the Triton class with specified id
		std::string getClassName(size_t id);
#endif

	private:
		// Checks if the Triton class with specified id can be accessed
		bool checkReturnability(size_t id, size_t requester);
	private:
		// All the classes that registered themselves with the host
		std::vector<reference<Core::TritonClass>> m_registeredClasses;
		// Map containing the ids of registered classes and their instances
		std::unordered_map<size_t, reference<Core::TritonClass>> m_IDObjectPairs;

		// The id that will be given to the next class that gets registered
		size_t m_nextClassId = 0;

		// Variable that is used to store the init state of the host
		bool m_initialized = false;
	};
}