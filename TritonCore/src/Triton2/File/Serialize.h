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

	namespace IO
	{
		template<class Archive>
		void serialize(Archive& archive,
			MeshData& m)
		{
			archive(m.meshes);
		}

		template<class Archive>
		void serialize(Archive& archive,
			Mesh& m)
		{
			archive(m.vertices, m.indices);
		}

		template<class Archive>
		void serialize(Archive& archive,
			Vertex& m)
		{
			archive(m.Vertice, m.UV, m.Normal);
		}

		template<class Archive>
		void serialize(Archive& archive,
			ImageData& m)
		{
			archive(
				m.width,
				m.height,
				m.BPP,
				m.buffer);
		}
	}
}

#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/string.hpp>