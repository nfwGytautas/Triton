#include <TRpch.h>
#include "Asset.h"

namespace Triton
{
	namespace Resource
	{

		void Asset::dontDestroyOnUnload()
		{
			m_destroyOnUnload = false;
		}

		bool Asset::valid() const
		{
			return m_isDestroyed;
		}
	}
}