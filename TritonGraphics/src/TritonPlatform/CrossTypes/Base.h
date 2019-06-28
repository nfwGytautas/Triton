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
	}
}