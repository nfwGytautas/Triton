#pragma once
#include "TRpch.h"
#include "Triton\TRMacros.h"

//ENTT https://github.com/skypjack/entt
#include <entt\entity\registry.hpp>

namespace Triton
{
	namespace ECS
	{		
		namespace Internal
		{
			using entt_ENTITY_type = std::uint32_t;
		}

		using Registry = entt::registry<Internal::entt_ENTITY_type>;
		using Entity = entt::registry<Internal::entt_ENTITY_type>::entity_type;
	}
}