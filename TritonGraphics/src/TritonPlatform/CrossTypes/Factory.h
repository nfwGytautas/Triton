#pragma once

#include "Base.h"

namespace Triton
{
	namespace PType
	{

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
			virtual void destroy(FactoryDestroyParams* destroyParams) = 0;
		};

		class Factory
		{
		public:
			virtual ~Factory() { }

			virtual FactoryObject* createShader(FactoryCreateParams* createParams) = 0;
			virtual FactoryObject* createVAO(FactoryCreateParams* createParams) = 0;
			virtual FactoryObject* createTexture(FactoryCreateParams* createParams) = 0;
			virtual FactoryObject* createFramebuffer(FactoryCreateParams* createParams) = 0;

			virtual void destroyObject(FactoryObject* object, FactoryDestroyParams* destroyParams) = 0;
		};

	}
}