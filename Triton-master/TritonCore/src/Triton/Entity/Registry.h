#pragma once

//ENTT https://github.com/skypjack/entt
#include <entt\entity\registry.hpp>
#include <entt\entity\group.hpp>

#include "Triton\Entity\Components\TritonComponents.h"

namespace Triton
{
	namespace ECS
	{		
		using Registry = entt::registry;
		using Entity = entt::registry::entity_type;
	}
}