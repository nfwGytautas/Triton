#pragma once

#include "ManagerBase.h"

#include "TritonPlatform/CrossTypes/Types.h"
#include "Triton/Core/Asset/Asset.h"

namespace Triton
{
	namespace Manager
	{
		using ObjectManager = Manager<PType::PlatformObject>;
		using AssetManager = Manager<Resource::Asset>;
	}
}