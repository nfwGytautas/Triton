#include "TRpch.h"
#include "Registry.h"

Triton::ECS::Internal::ComponentID Triton::ECS::Internal::GetUniqueComponentID() noexcept
{
	static Triton::ECS::Internal::ComponentID lastIDC{ 0u };
	return lastIDC++;
}

Triton::ECS::Internal::SystemID Triton::ECS::Internal::GetUniqueSystemID() noexcept
{
	static Triton::ECS::Internal::SystemID lastIDS{ 0u };
	return lastIDS++;
}
