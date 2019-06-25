#pragma once

#include "TritonTypes/reference.h"

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

		// Base class for storing factory object creation parameters
		class FactoryCreateParams
		{
		public:
			virtual ~FactoryCreateParams() { }
		};

		// Base class for storing factory object destruction parameters
		class FactoryDestroyParams
		{
		public:
			virtual ~FactoryDestroyParams() { }
		};

		// Base class for factory objects
		class FactoryObject : public PlatformObject
		{
		public:
			virtual ~FactoryObject() { }

			virtual void create(FactoryCreateParams* createParams) = 0;
		};

		// Base class for renderable objects
		class Renderable : public FactoryObject
		{
		public:
			virtual ~Renderable() { }

			virtual unsigned int getIndiceCount() = 0;
		};

		// Base wraper object
		class WraperBase
		{
		public:
			WraperBase() { }
			virtual ~WraperBase() { }

			virtual void enable() = 0;
		};

		// Wraper around the platform object
		template <class T>
		class PlatformWraper : public WraperBase
		{
		public:
			PlatformWraper(reference<T> obj) : WraperBase(), _object(obj) { }
			virtual ~PlatformWraper() { }

			// Get the wraped object
			reference<T> object() const
			{
				return _object;
			}

			virtual void enable() override = 0;
		protected:
			reference<T> _object;
		};
	}
}