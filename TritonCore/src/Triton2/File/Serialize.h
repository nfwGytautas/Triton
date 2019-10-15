#pragma once

#include "TritonTypes/IO2.h"
#include "Triton2/Scene/Lighting.h"
#include "TritonPlatform2/mathematical.h"
#include "TritonTypes/AssetDictionary.h"
#include "Triton2/Scene/ECS.h"

namespace Triton
{
	template<class Archive>
	void serialize(Archive& archive,
		Vector2& m)
	{
		archive(m.x, m.y);
	}

	template<class Archive>
	void serialize(Archive& archive,
		Vector3& m)
	{
		archive(m.x, m.y, m.z);
	}

	template<class Archive>
	void serialize(Archive& archive,
		Vector4& m)
	{
		archive(m.x, m.y, m.z, m.w);
	}

	template<class Archive>
	void serialize(Archive& archive,
		PointLight& m)
	{
		archive(m.Position, m.Color, m.Range, m.Constant, m.Linear, m.Quadratic);
	}

	template<class Archive>
	void serialize(Archive& archive,
		DirectionalLight& m)
	{
		archive(m.Position, m.Color, m.Direction);
	}

	template<class Archive>
	void serialize(Archive& archive,
		SpotLight& m)
	{
		archive(m.Position, m.Color, m.Range, m.Angle, m.Direction, m.Constant, m.Linear, m.Quadratic);
	}

	namespace Core
	{
		template<class Archive>
		void serialize(Archive& archive,
			AssetDictionary::AssetMeta& m)
		{
			int& type = (int&)m.Type;
			archive(cereal::make_nvp("Path", m.Path), cereal::make_nvp("IsPacket", m.PacketFile), cereal::make_nvp("Type", type));
		}
	}

	namespace Components
	{
		template<class Archive>
		void serialize(Archive& archive,
			MetaComponent& m)
		{
			archive(m.Name);
		}

		template<class Archive>
		void serialize(Archive& archive,
			Transform& m)
		{
			archive(m.Position, m.Rotation, m.Scale);
		}

		template<class Archive>
		void serialize(Archive& archive,
			Visual& m)
		{
			archive(m.Mesh, m.Material);
		}
	}
}

#include <cereal/archives/binary.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/bitset.hpp>
#include <cereal/types/unordered_map.hpp>