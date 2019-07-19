#pragma once

#include <string>
#include <vector>

#include "Triton/Config.h"
#include "TritonTypes/reference.h"

namespace Triton
{
	class TritonHost;

	namespace Core
	{
		// Enum containing all ids of Triton classes already defined by the engine
		// using this enum they can be accesed anywhere without use of names
		enum class TritonClasses : size_t
		{
			InputManager = 0,
			Context = 1,
			Cache = 2,
			AssetManager = 3,
			ThreadManager = 4,
			MainRenderBuffer = 5,
			Timer = 6,
			GameWindow = 7
		};

		// Enum containing all the inbuilt messages used in the Triton engine
		enum class TritonMessageType : size_t
		{
			ClassRegistered = 0,
			Render = 1,
			Update = 2
		};

		// The params that are pased to Triton host when creating a specific
		// Triton class, this struct specifies how the host should create the class
		struct ClassRegisterParams
		{
			// Marks the created class as protected meaning no other class can recieve the instance of it
			// unless the id of the receiver is the on specified in the Receivers vector
			bool IsProtected = false;

			// Vector containing ids of the Triton classes that can get the instance of this class if 
			// IsProtected = true
			std::vector<size_t> Receivers;

			// Name of the created class
			std::string Name;
		};

		// The base class of all objects (User or Engine)
		// that want interact with the engine
		class TritonClass
		{
		public:
			virtual ~TritonClass() {}

			size_t ID() const
			{
				return m_ID;
			}

#if TR_STRING_REPRESENTATIONS == 1
			std::string getName() const
			{
				return m_Name;
			}
#endif

			// A Triton class method that allows for communication between classes
			virtual void onMessage(size_t message, void* payload) { }

		protected:
			// Returns a reference to class object with specified id
			reference<Core::TritonClass> getClassByID(size_t id);

#if TR_STRING_REPRESENTATIONS == 1
			// Returns a reference to class object with specified name
			reference<Core::TritonClass> getClassByName(std::string name);
#endif
		private:
			// ID used to keep track of the object in the host
			size_t m_ID;

			// Boolean that shows if the specified class is protected
			bool m_Protected;

			// Classes allowed access to this class
			std::vector<size_t> m_Receivers;

#if TR_STRING_REPRESENTATIONS == 1
			// The name of the class
			std::string m_Name;
#endif

			// Instance of the host
			TritonHost* m_HostInstance;
		private:
			// Sets private members from create params this method is called after the id and 
			// host instance is set
			void takeParams(ClassRegisterParams params);

			friend TritonHost;
		};
	}
}