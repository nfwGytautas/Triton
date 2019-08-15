#pragma once

#include "Triton/Core/Asset/Asset.h"

namespace Triton
{
	namespace Resource
	{
		// Serialize asset create params
		template<class Archive>
		void serialize(Archive& archive,
			AssetCreateParams& m)
		{
			archive(m.Type);
			archive(m.Paths);
			archive(m.Arguments);
			archive(m.Width);
			archive(m.Height);
			archive(m.Name);
			archive(m.CacheResult);
			archive(m.ID);
		}
	}
}