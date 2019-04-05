#pragma once
#include "TritonShell\Entity\Components\TritonComponents.h"
#include "TritonShell\Entity\Registry.h"

namespace Triton
{
	namespace Systems
	{
		//Base system class
		class System
		{
		public:
			virtual void OnUpdate(ECS::Registry& aRegistry, float aDelta) = 0;
			virtual ~System() { }
		};

		//Batching system for optimized mesh rendering
		class BatchSystem : public System
		{
		public:
			// Inherited via System
			virtual void OnUpdate(ECS::Registry& aRegistry, float aDelta) override;

			~BatchSystem()
			{ }
		};		
	}
}