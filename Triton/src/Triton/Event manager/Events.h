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

		virtual ~Event() { }

		inline bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}

		inline bool IsHandled() const
		{
			return m_Handled;
		}
	protected:
		bool m_Handled = false;
	};

	using EventBehavior = std::function<bool(const Event& e)>;

	class TRITON_API EventListener
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventListener();

		void Listen(const EventType& event, EventBehavior& behavior);
		void StopListening(const EventType& event);

		void React(Event& event);
	private:
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