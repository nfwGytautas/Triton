#pragma once

//EnTT https://github.com/skypjack/entt
#include <entt\entity\registry.hpp>

#include "TritonPlatform2/mathematical.h"

namespace Triton
{
	namespace ECS
	{
		/// EnTT class, registry used to store entities and their components
		using Registry = entt::registry;

		/// EnTT class, entity identifier
		using Entity = entt::registry::entity_type;
	}
}

#define TR_SERIALIZABLE_COMPONENTS Triton::Components::MetaComponent, Triton::Components::Transform, Triton::Components::Visual

namespace Triton
{
	namespace Components
	{
		/**
		 * Component containing some meta data about entities
		 */
		struct MetaComponent
		{
			/// Name of the entity
			std::string Name;

			/**
			 * Default constructor
			 */
			MetaComponent()
				: Name("NULL")
			{}

			/**
			 * Copy constructor
			 *
			 * @param other The meta component to copy
			 */
			MetaComponent(const MetaComponent& other)
				: Name(other.Name)
			{}

			/**
			 * Constructor with given name
			 *
			 * @param name Name of the entity
			 */
			MetaComponent(std::string name)
				: Name(name)
			{}
		};

		/**
		 * Component containing the transformation data of a single entity
		 */
		struct Transform
		{
			/// 3 component vector that defines the 3D position
			Vector3 Position;

			/// 3 component vector that defines the rotation around each axis
			Vector3 Rotation;

			/// 3 component vector that defines the scale
			Vector3 Scale;

			/**
			 * Default constructor
			 */
			Transform()
				: Position(0.0f, 0.0f, 0.0f), Rotation(0.0f, 0.0f, 0.0f), Scale(1.0f, 1.0f, 1.0f)
			{}

			/**
			 * Copy constructor
			 *
			 * @param other The transform component to copy
			 */
			Transform(const Transform& other)
				: Position(other.Position), Rotation(other.Rotation), Scale(other.Scale)
			{ }

			/**
			 * Constructor with given position, rotation and scale
			 *
			 * @param position Position of the entity
			 * @param rotation Rotation of the entity
			 * @param scale Scale of the entity
			 */
			Transform(Vector3& position, Vector3& rotation, Vector3& scale)
				: Position(position), Rotation(rotation), Scale(scale)
			{}
		};

		/**
		 * Component containing the data needed to represent the entity
		 */
		struct Visual
		{
			/// Mesh used by the entity
			std::string Mesh;

			/// Material used by the entity
			std::string Material;

			/**
			 * Default constructor
			 */
			Visual()
				: Mesh("NULL"), Material("NULL")
			{}

			/**
			 * Copy constructor
			 *
			 * @param other The visual component to copy
			 */
			Visual(const Visual& other)
				: Mesh(other.Mesh), Material(other.Material)
			{ }

			/**
			 * Constructor with given mesh and material
			 *
			 * @param mesh Mesh object name
			 * @param material Material object name
			 */
			Visual(std::string mesh, std::string material)
				: Mesh(mesh), Material(material)
			{}
		};
	}
}