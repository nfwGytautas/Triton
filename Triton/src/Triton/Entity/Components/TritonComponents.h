#pragma once
#include "Triton\TRMacros.h"
#include "Triton\Core\Math\Math.h"

namespace Triton
{
	namespace Components
	{
		//A component which specifies entity transformation inside the world
		struct TRITON_API Transform
		{
			Vector3 Position;
			Vector3 Rotation;
			Vector3 Scale;

			Transform()
				: Position(0.0f, 0.0f, 0.0f), Rotation(0.0f, 0.0f, 0.0f), Scale(1.0f, 1.0f, 1.0f)
			{}
			Transform(Transform& aOther)
				: Position(aOther.Position), Rotation(aOther.Rotation), Scale(aOther.Scale)
			{ }
			Transform(Vector3& aPosition, Vector3& aRotation, Vector3& aScale)
				: Position(aPosition), Rotation(aRotation), Scale(aScale)
			{}
		};
	
		//A component which specifies what meshes an entity uses
		struct TRITON_API MeshFilter
		{
			MeshList Meshes;

			MeshFilter()
			{ }
			MeshFilter(MeshFilter& aOther)
				: Meshes(aOther.Meshes)
			{

			}
		};
	}
}