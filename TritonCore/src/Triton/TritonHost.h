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
#include "Triton\Managers\ThreadManager.h"
#include "Triton\Core\RenderBuffer.h"
#include "Triton\Utility\Utility.h"
#include "Triton\Core\GameWindow\GameWindow.h"

namespace Triton 
{
	// Specifies how the Triton host should be initialized
	struct HostInitializationParams
	{
		AppSettings AppSettings;
		
		// Notify the host that you will be sending render messages yourself
		bool CustomRenderFunction = false;

		// Notify the host that you will be sending update messages yourself
		bool CustomUpdateFunction = false;
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
		void registerClass(reference<Core::TritonClass> classToRegister, Core::ClassRegisterParams classRegisterParams);

		// Returns a reference to class object with specified id
		reference<Core::TritonClass> getClassByID(size_t id);

		// Returns a reference to protected class object with specified id with requester id
		reference<Core::TritonClass> getClassByID(size_t id, size_t requester);

		// Posts a message that is sent to all classes
		void postMessage(int message, void* payload);

		// Posts a message that is sent to classes in the specified layer
		void postMessage(int message, std::vector<reference<Core::TritonClass>>& classesToSendTo, void* payload);

#if TR_STRING_REPRESENTATIONS == 1
		// Returns a reference to class object with specified name
		reference<Core::TritonClass> getClassByName(std::string name);

		// Returns a reference to protected class object with specified id with requester id
		reference<Core::TritonClass> getClassByName(std::string name, size_t requester);
#endif

		// Enables/disables rendering, in MT configuration the host creates a thread that
		// immediately starts sending render messages this function is used to stop rendering
		// in cases where the host is not the one that controls the flow of the program such
		// as being embedded in QApplication or any other program that has specific render cycles
		// the rendering can be achieved by hand by posting a render message
		void isRendering(bool state);

		// Same as isRendering but for updating, once updating is disabled the host stops all 
		// updating which means the host no longer receives messages from the system and stops posting
		// events however this can be achieved by hand using getClass methods and getting reference to 
		// input manager
		void isUpdating(bool state);

		// Starts the updating and rendering loops if custom loops are not provided
		void start();
	private:
		// Checks if the Triton class with specified id can be accessed
		bool checkReturnability(size_t id, size_t requester);

		// Update function for the engine when a custom function is not given
		void updateMT();

		// Render function for the engine when a custom function is not given
		void renderMT();
	private:
		// Struct containing the current host settings
		struct HostSettings
		{
			// Host is rendering state
			bool rendering = true;
			// Host is updating state
			bool updating = true;

			// Should host handle rendering
			bool render = true;
			// Should host handle updating
			bool update = true;
		} m_settings;

		// Struct containing the order of sending messages for
		// Rendering, Updating, PreRendering
		struct ClassPriorities
		{
			std::vector<std::vector<reference<Core::TritonClass>>> update;
			std::vector<std::vector<reference<Core::TritonClass>>> render;
			std::vector<std::vector<reference<Core::TritonClass>>> preRender;
		} m_priorities;

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