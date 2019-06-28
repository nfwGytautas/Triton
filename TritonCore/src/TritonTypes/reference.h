#pragma once

namespace Triton
{
	// A pointer wraper similiar to std::shared_ptr, in that it counts reference count,
// but it also provides functionality for polymorphism with as() function
	template <typename T>
	class reference
	{
	private:
		// ref count manipulation - increase by 1
		void increment()
		{
			get_count()++;
		}

		// ref count manipulation - decrease by 1
		int& decrement()
		{
			return --get_count();
		}

		// returns the current ref count
		int& get_count()
		{
			return *(m_ReferenceCount);
		}
	private:
		T* m_Object{ nullptr };
		int* m_ReferenceCount{ nullptr };

		template<class AsType>
		friend class reference;
	private:
		// Private constructor;
		reference(T* object, int* refcnt)
			: m_Object{ object }
			, m_ReferenceCount{ refcnt }
		{
			increment();
		}
	public:
		// Empty constructor
		reference()
			: m_Object{ nullptr }
			, m_ReferenceCount{ nullptr }
		{
		}

		// Constructor
		reference(T* object)
			: m_Object{ object }
			, m_ReferenceCount{ new int(0) }
		{
			increment();
		}

		// Destructor
		virtual ~reference()
		{
			if (m_ReferenceCount)
			{
				int decrementedCount = decrement();
				if (decrementedCount <= 0)
				{
					delete m_ReferenceCount;
					delete m_Object;
					m_ReferenceCount = nullptr;
					m_Object = nullptr;
				}
			}
		}

		// Copy Constructor
		reference(const reference<T>& other)
			: m_Object{ other.m_Object }
			, m_ReferenceCount{ other.m_ReferenceCount }
		{
			increment();
		}

		// Overloaded Assignment Operator
		reference<T>& operator=(const reference<T>& other)
		{
			if (this != &other)
			{
				if (m_ReferenceCount)
				{
					if (decrement() == 0)
					{
						delete m_ReferenceCount;
						delete m_Object;
					}
				}
				m_Object = other.m_Object;
				m_ReferenceCount = other.m_ReferenceCount;
				increment();
			}
			return *this;
		}

		// Compares two references of type T by checking 
		// if the underlying pointers point to the same memory
		bool operator==(const reference<T>& other) const
		{
			return m_Object == other.m_Object;
		}

		// Dereference operator
		T& operator*()
		{
			return *m_Object;
		}

		// Member Access operator
		T* operator->()
		{
			return m_Object;
		}

		// Creates a reference of specified type from current reference used in polymorphic types
		template <typename AsType>
		reference<AsType> as()
		{
			return reference<AsType>(dynamic_cast<AsType*>(m_Object), m_ReferenceCount);
		}

		// Checks if the underlying pointer is valid
		bool valid() const
		{
			return m_Object != nullptr;
		}
	};
}