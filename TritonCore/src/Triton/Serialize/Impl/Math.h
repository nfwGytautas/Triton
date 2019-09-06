#pragma once

#include "TritonPlatform/mathematical.h"

namespace Triton
{
	template<class Archive>
	void serialize(Archive& archive,
		Vector2& m)
	{
		archive(m.x);
		archive(m.y);
	}

	template<class Archive>
	void serialize(Archive& archive,
		Vector3& m)
	{
		archive(m.x);
		archive(m.y);
		archive(m.z);
	}

	template<class Archive>
	void serialize(Archive& archive,
		Vector4& m)
	{
		archive(m.x);
		archive(m.y);
		archive(m.z);
		archive(m.w);
	}
}