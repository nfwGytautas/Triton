#pragma once

#include "TritonTypes/IO.h"
#include "TritonPlatform2/mathematical.h"

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
}

#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/string.hpp>