#pragma once

#include "TritonTypes/IO.h"
#include "Triton2/Scene/Lighting.h"
#include "TritonPlatform2/mathematical.h"
#include "TritonTypes/AssetDictionary.h"

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
			archive(cereal::make_nvp("Path", m.Path), cereal::make_nvp("IsPacket", m.PacketFile));
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