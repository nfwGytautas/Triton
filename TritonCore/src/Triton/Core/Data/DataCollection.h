#pragma once

namespace Triton
{
	namespace Core
	{
		template <class T>
		class DataCollection
		{
		public:
			size_t Add(std::shared_ptr<T> aResource)
			{
				m_Resources.push_back(aResource);
				return m_Resources.size() - 1;
			}
			std::shared_ptr<T> Take(size_t aResourceIndex)
			{
				if (!m_Resources.empty())
					return m_Resources[aResourceIndex];
				else
				{
					TR_CORE_ERROR("Resource with id: {0} doesn't exist!", aResourceIndex);
					TR_CORE_ASSERT(0, aResourceIndex);
					return nullptr;
				}
			}
		private:
			std::vector<std::shared_ptr<T>> m_Resources;
		};
	}
}