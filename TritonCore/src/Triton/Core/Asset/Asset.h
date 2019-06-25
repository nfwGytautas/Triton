#pragma once

#include "TritonPlatform/PlatformAbstraction.h"

namespace Triton
{
	namespace Resource
	{
		class Asset
		{
		public:
			virtual ~Asset() { }

			void dontDestroyOnUnload();
			bool valid() const;
		private:
			bool m_destroyOnUnload = true;
			bool m_isDestroyed = false;
		};
	}
}