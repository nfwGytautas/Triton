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

		void Asset::destroyAsset(relay_ptr<PType::Context> context, PType::FactoryDestroyParams* params)
		{
			if (m_destroyOnUnload)
			{
				destroyFunc(context, params);
				m_isDestroyed = true;
			}
		}

		bool Asset::valid() const
		{
			return m_isDestroyed;
		}



	}
}