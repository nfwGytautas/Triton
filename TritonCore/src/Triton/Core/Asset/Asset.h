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
			void destroyAsset(relay_ptr<PType::Context> context, PType::FactoryDestroyParams* params);
			bool valid() const;
		protected:
			virtual void destroyFunc(relay_ptr<PType::Context> context, PType::FactoryDestroyParams* params) = 0;
		private:
			bool m_destroyOnUnload = true;
			bool m_isDestroyed = false;
		};
	}
}