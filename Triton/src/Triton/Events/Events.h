#pragma once
#include "TRpch.h"
#include "../TRMacros.h"

namespace Triton
{
	class TRITON_API Event;
	class TRITON_API EventListener;

	namespace Core
	{
		class TRITON_API EventManager;
	}

	enum class EventType : size_t
	{
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	const size_t MaxEvents = (size_t)EventType::MouseScrolled;

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};

	class TRITON_API Event
	{
		friend class EventListener;
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		virtual ~Event() {}

		bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}

		bool IsHandled() const
		{
			return m_Handled;
		}
	private:
		bool m_Handled = false;
	};

	using EventBehavior = std::function<bool(const Event& e)>;
	class TRITON_API EventListener
	{		
	public:
		EventListener();
		~EventListener();

		template<typename T>
		void Listen(EventBehavior& behavior)
		{
			TR_STATIC_ASSERT(std::is_base_of<Event, T>::value, "Can't listen to non Event!");
			m_ListeningTo[(size_t)T::GetStaticType()] = true;
			m_RegisteredEvents[(size_t)T::GetStaticType()] = behavior;
		}
		template<typename T>
		void StopListening()
		{
			TR_STATIC_ASSERT(std::is_base_of<Event, T>::value, "Can't stop listening to non Event!");
			m_ListeningTo[(size_t)T::GetStaticType()] = false;
		}

		bool operator==(const EventListener& rhs) const
		{
			return m_ID == rhs.m_ID;
		}
		bool operator!=(const EventListener& rhs) const
		{
			return !(*this == rhs);
		}

		void React(Event& event);
	private:
		size_t m_ID;
		std::bitset<MaxEvents> m_ListeningTo;
		std::array<EventBehavior, MaxEvents> m_RegisteredEvents;
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}