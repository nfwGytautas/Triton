#pragma once

#include <string>
#include <vector>

#include "Triton/Config.h"
#include "TritonTypes/reference.h"
#include "Triton/Core/Graphics/Layers.h"

namespace Triton
{
	class TritonHost;

	namespace Core
	{
		// Enum containing all ids of Triton classes already defined by the engine
		// using this enum they can be accessed anywhere without use of names
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
			PreRender = 2,
			Update = 3
		};

		// Flags that specify what messages should be sent to a Triton class
		enum class ReceivedMessages : int 
		{
			None = 0,
			ClassRegistered = 1 << (int)TritonMessageType::ClassRegistered,
			Render			= 1 << (int)TritonMessageType::Render,
			PreRender		= 1 << (int)TritonMessageType::PreRender,
			Update			= 1 << (int)TritonMessageType::Update,
			All				= 1 << (int)TritonMessageType::Update + 1,
			Custom			= 1 << (int)TritonMessageType::Update + 2,
		};

		inline constexpr ReceivedMessages operator|(ReceivedMessages a, ReceivedMessages b) {
			return static_cast<ReceivedMessages> ((int)a | (int)b);
		}
		inline constexpr int operator&(ReceivedMessages a, ReceivedMessages b) {
			return (int)a & (int)b;
		}
		inline constexpr int operator&(ReceivedMessages a, int b) {
			return (int)a & b;
		}
		inline constexpr int operator&(int a, ReceivedMessages b) {
			return a & (int)b;
		}
		inline constexpr size_t operator&(ReceivedMessages a, size_t b) {
			return (size_t)a & b;
		}

		// The mask of all built in messages in Triton
		const ReceivedMessages c_BuiltInMessageMask = (ReceivedMessages::ClassRegistered |
			ReceivedMessages::Update |
			ReceivedMessages::Render |
			ReceivedMessages::PreRender);

		// The params that are passed to Triton host when creating a specific
		// Triton class, this struct specifies how the host should create the class
		struct ClassRegisterParams
		{
			// Marks the created class as protected meaning no other class can receive the instance of it
			// unless the id of the receiver is the on specified in the Receivers vector
			bool IsProtected = false;

			// Vector containing ids of the Triton classes that can get the instance of this class if 
			// IsProtected = true
			std::vector<size_t> Receivers;

			// Name of the created class
			std::string Name;

			// Specifies what priority does this class have over others when sending render messages
			// Messages are sent from the lowest number to the highest number if a class with
			// the same priority already exists then it will be inserted AFTER said class
			Layers::Layer UpdatePriority = Layers::c_lastLayer;

			// Same as update priority except for pre render messages
			Layers::Layer PreRenderPriority = Layers::c_lastLayer;

			// Same as update priority except for render messages, for example a scene can be rendered
			// in 0 layer and GUI in 1 meaning that the GUI class will receive the render message after
			// the scene has been rendered
			Layers::Layer RenderLayer = Layers::c_lastLayer;

			// Messages that the class is receiving, by default all messages will be sent to the class
			// this is mainly used for minimizing the amount of onMessage calls per update
			ReceivedMessages MessagesToListenTo = ReceivedMessages::All;
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

			// The messages the class receives
			ReceivedMessages m_messages;

			// Struct containing the priorities/positions for messages
			struct LayerSettings
			{
				Layers::Layer Update;
				Layers::Layer PreRender;
				Layers::Layer Render;
			} m_layerSettings;

			// Instance of the host
			TritonHost* m_HostInstance;
		private:
			// Sets private members from create params this method is called after the id and 
			// host instance is set
			void takeParams(ClassRegisterParams& params);

			friend TritonHost;
		};
	}
}