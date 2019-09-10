#pragma once

#include "TritonTypes/relay_ptr.h"

namespace Triton
{
	/**
	 * A pointer wrapper similar to std::shared_ptr, in that it counts reference count,
	 * but it also provides functionality for polymorphism with as() function
	 */
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
		/**
		 * Empty constructor
		 */
		reference()
			: m_Object{ nullptr }
			, m_ReferenceCount{ nullptr }
		{
		}

		/**
		 * Constructor with pointer
		 *
		 * @param object The pointer to be stored
		 */
		reference(T* object)
			: m_Object{ object }
			, m_ReferenceCount{ new int(0) }
		{
			increment();
		}

		/**
		 * Destructor
		 */
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

		/**
		 * Copy constructor
		 *
		 * @param other Another reference object
		 */
		reference(const reference<T>& other)
			: m_Object{ other.m_Object }
			, m_ReferenceCount{ other.m_ReferenceCount }
		{
			if (m_ReferenceCount)
			{
				increment();
			}
		}

		/**
		 * Assignment operator used to reassign the value of the reference object
		 *
		 * @param other New value for this reference object
		 * @return This, but with new values
		 */
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

		/**
		 * Compares two references of type T by checking 
		 * if the underlying pointers point to the same memory
		 *
		 * @param other The other reference to compare to
		 * @return True if both reference point to the same memory
		 */
		bool operator==(const reference<T>& other) const
		{
			return m_Object == other.m_Object;
		}

		/**
		 * Dereference operator used to get the object instance
		 *
		 * @return Underlying object c++ reference
		 */
		T& operator*()
		{
			return *m_Object;
		}

		/**
		 * Member access operator
		 *
		 * @return Underlying object const pointer
		 */
		T* operator->() const
		{
			return m_Object;
		}

		/**
		 * Creates a reference of specified type from current reference
		 * used in polymorphic types
		 *
		 * @tparam AsType The type of the new reference
		 * @return Reference with the specified AsType type
		 */
		template <typename AsType>
		reference<AsType> as()
		{
			return reference<AsType>(dynamic_cast<AsType*>(m_Object), m_ReferenceCount);
		}

		/**
		 * Checks if the underlying pointer is valid
		 *
		 * @return True if the pointer is not pointing to nullptr
		 */
		bool valid() const
		{
			return m_Object != nullptr;
		}

		
		/**
		 * Returns the underlying pointer as a relay_ptr which guarantees that the pointer won't be deleted
		 * but also there won't be any reference counting which means that if the reference is deleted the
		 * relay_ptr will be invalid
		 *
		 * @return Relay pointer created from the underlying pointer
		 */
		relay_ptr<T> asRelay() const
		{
			return relay_ptr<T>(m_Object);
		}

		/**
		 * Current reference count of the object
		 *
		 * @return Number of references
		 */
		int refCount() const
		{
			return *m_ReferenceCount;
		}
	};
}