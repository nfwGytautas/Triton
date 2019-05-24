#pragma once

#include "TritonTypes/relay_ptr.h"

namespace Triton
{
	namespace PType
	{
		// Base class for most platform objects
		class PlatformObject
		{
		public:
			virtual ~PlatformObject() { }
		};

		// Base wraper object
		class WraperBase
		{
		public:
			WraperBase(PlatformObject* obj) : _object(obj) { }
			virtual ~WraperBase() { }

			virtual void enable() = 0;
		protected:
			PlatformObject* _object;
		};

		// Wraper around the platform object
		template <class T>
		class PlatformWraper : public WraperBase
		{
		public:
			PlatformWraper(T* obj) : WraperBase(obj) { }
			virtual ~PlatformWraper() { }

			// Get the wraped object
			T* object() const
			{
				return static_cast<T*>(this->_object);
			}

			virtual void enable() override = 0;
		};
	}
}