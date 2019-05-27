#pragma once

//ENTT https://github.com/skypjack/entt
#include <entt\entity\registry.hpp>

namespace Triton
{
	namespace ECS
	{		
		using Registry = entt::registry;
		using Entity = entt::registry::entity_type;
	}
}