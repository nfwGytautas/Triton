#pragma once
#include "Triton\Core\Math\Math.h"
#include "Triton\Core\Data\Mesh.h"
#include "Triton\Core\Data\Material.h"

#include <cereal\archives\binary.hpp>

namespace Triton
{
	namespace Components
	{
		//A component which specifies entity transformation inside the world
		struct Transform
		{
			Vector3 Position;
			Vector3 Rotation;
			Vector3 Scale;

			Transform()
				: Position(0.0f, 0.0f, 0.0f), Rotation(0.0f, 0.0f, 0.0f), Scale(1.0f, 1.0f, 1.0f)
			{}
			Transform(const Transform& aOther)
				: Position(aOther.Position), Rotation(aOther.Rotation), Scale(aOther.Scale)
			{ }
			Transform(Vector3& aPosition, Vector3& aRotation, Vector3& aScale)
				: Position(aPosition), Rotation(aRotation), Scale(aScale)
			{}
		};

		struct Renderable
		{
			size_t ID;
		};
	}
}