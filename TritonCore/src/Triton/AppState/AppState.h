#pragma once
#include "Triton\TRMacros.h"
#include "Storables.h"

namespace Triton
{
	namespace Storage
	{
		class TRITON_API AppState
		{
		public:
			AppState();
			~AppState();

			void Store(std::string aName, std::shared_ptr<Storable> aStorable);
			std::shared_ptr<Storable> Take(std::string aName);

			void Clear();
		private:
			std::unordered_map<std::string, std::shared_ptr<Storable>> m_Variables;
		};
	}
}