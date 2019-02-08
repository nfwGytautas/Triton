#pragma once
#include "Triton\TRMacros.h"
#include "Storables.h"

#define TR_APPSTATE_TAKE(appStateObject, storableName, takeObjectName, storableType, returnType) \
		returnType takeObjectName = std::dynamic_pointer_cast<storableType>(appStateObject->Take(storableName))->value.cast<returnType>()\

#define TR_APPSTATE_TAKE_ASSIGN(appStateObject, storableName, takeObjectName, storableType, returnType) \
		takeObjectName = std::dynamic_pointer_cast<storableType>(appStateObject->Take(storableName))->value.cast<returnType>()\

#define TR_GAME_VARIABLE(type, name) \
		std::shared_ptr<Triton::Storage::TR_STRINGIZE(type,_Storable)> gv_##name = std::make_shared<Triton::Storage::TR_STRINGIZE(type,_Storable)>()\

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