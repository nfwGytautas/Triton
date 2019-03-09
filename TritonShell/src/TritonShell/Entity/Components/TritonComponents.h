#pragma once
#include "Triton\Core\Math\Math.h"
#include "Triton\Core\Data\Structures\Mesh.h"
#include "Triton\Core\Data\Structures\Material.h"

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

		template<typename Archive>
		void serialize(Archive &ar, Transform &transform) {
			ar(transform.Position.x, transform.Position.y, transform.Position.z,
				transform.Rotation.x, transform.Rotation.y, transform.Rotation.z,
				transform.Scale.x, transform.Scale.y, transform.Scale.z);
		}
	
		//A component which specifies what meshes an entity uses
		struct MeshFilter
		{
			std::shared_ptr<Data::Mesh> Mesh;

			MeshFilter()
			{ }
			MeshFilter(std::shared_ptr<Data::Mesh> aMesh)
				:Mesh(aMesh)
			{ }
			MeshFilter(const MeshFilter& aOther)
				: Mesh(aOther.Mesh)
			{

			}
		};

		//A component which specifies how a game object should be rendered
		struct MeshRenderer
		{
			std::shared_ptr<Data::Material> Material;

			MeshRenderer()
			{ }
			MeshRenderer(std::shared_ptr<Data::Material> aMaterial)
				:Material(aMaterial)
			{ }
			MeshRenderer(const MeshRenderer& aOther)
				: Material(aOther.Material)
			{

			}
		};
	}
}