#pragma once

namespace Triton
{

	// Simple pointer wraper that doesn't allow the deletion of the underlying pointer.
	// Note 1: This wraper doesn't count any references or enforce uniqueness,
	// if such functionality is needed use the standard smart pointers.
	// Note 2: This wraper could be compared to a reference except for the fact
	// that it doesn't require to be set during construction of object
	template <typename T>
	class relay_ptr
	{
	public:
		// Create relay_ptr from a normal pointer
		relay_ptr(T* aptr)
			: ptr(aptr)
		{ }

		// Default constructor
		relay_ptr()
			: ptr(nullptr)
		{ }

		// Copy constructor 
		relay_ptr(const relay_ptr<T> &relay)
		{
			ptr = relay.ptr;
		}

#ifdef _MEMORY_
		// Create relay_ptr from unique_ptr
		relay_ptr(std::unique_ptr<T>& aptr)
			: ptr(aptr.get())
		{ }

		// Create relay_ptr from shared_ptr
		relay_ptr(std::shared_ptr<T>& aptr)
			: ptr(aptr.get())
		{ }
#endif

		// Destructor
		~relay_ptr()
		{

		}

		// Assign to another relay_ptr, the value of the original ptr is not changed
		relay_ptr<T>& operator=(const relay_ptr<T>& rhs)
		{
			ptr = rhs.ptr;
			return *this;
		}

		// Set relay_ptr value to different ptr, the value of the original ptr is not changed
		relay_ptr<T>& operator=(T* rhs)
		{
			ptr = rhs;
			return *this;
		}

		// Check if the relay_ptr underlying ptr is valid or not(nullptr)
		bool valid() const
		{
			return ptr != nullptr;
		}

		// Access the pointer
		T *operator->() const
		{
			return ptr;
		}
	private:
		T* ptr;
	};

}