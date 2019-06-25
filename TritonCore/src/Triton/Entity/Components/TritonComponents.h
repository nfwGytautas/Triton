#pragma once
#include "TritonPlatform/mathematical.h"
#include <cereal\archives\binary.hpp>

#define TR_SERIALIZABLE_COMPONENTS Triton::Components::Transform, Triton::Components::Visual, Triton::Components::LightEmitter

namespace Triton
{
	namespace Components
	{
		// A component which specifies entity transformation inside the world
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

		// Component that represents a model
		struct Visual
		{
			//Mesh of entity, -1 is illegal
			size_t Mesh;
			//Material of entity, -1 is illegal
			size_t Material;

			Visual()
				: Mesh(-1), Material(-1)
			{}

			Visual(size_t aMesh)
				: Mesh(aMesh), Material(-1)
			{}

			Visual(size_t aMesh, size_t aMaterial)
				: Mesh(aMesh), Material(aMaterial)
			{}
		};

		// Component specifying that the entity is a light emitter
		struct LightEmitter
		{
			//Id of the light, -1 is illegal
			size_t Light;

			LightEmitter()
				: Light(-1)
			{}

			LightEmitter(size_t aLight)
				: Light(aLight)
			{}
		};

		// Component that reprsents an image not to be confused with Visual
		struct Image
		{
			//Bitmap of entity, -1 is illegal
			size_t Bitmap;

			Image()
				: Bitmap(-1)
			{}

			Image(size_t aBitmap)
				: Bitmap(aBitmap)
			{}
		};
	}
}

ENTT_NAMED_TYPE(Triton::Components::Transform)
ENTT_NAMED_TYPE(Triton::Components::Visual)
ENTT_NAMED_TYPE(Triton::Components::LightEmitter)