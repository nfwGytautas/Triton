#pragma once
#include "Triton\Core\Data\Structures\RenderBatch.h"
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

			std::vector<Data::RenderBatch>& GetBatches()
			{
				return m_Batches;
			}

			~BatchSystem()
			{ }
		private:
			std::vector<Data::RenderBatch> m_Batches;
		};		
	}
}