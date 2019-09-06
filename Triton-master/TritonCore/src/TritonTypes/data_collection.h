#pragma once

namespace Triton
{
	template <class T>
	class data_collection
	{
	public:
		size_t Add(std::shared_ptr<T> aResource)
		{
			m_Resources.push_back(aResource);
			return m_Resources.size() - 1;
		}

		relay_ptr<T> Take(size_t aResourceIndex)
		{
			if (!m_Resources.empty() && m_Resources.size() > aResourceIndex && aResourceIndex >= 0)
				return m_Resources[aResourceIndex];
			else
			{
				TR_CORE_ERROR("Resource with id: {0} doesn't exist!", aResourceIndex);
				TR_CORE_ASSERT(0, aResourceIndex);
				return nullptr;
			}
		}

		void ForEach(std::function<void(relay_ptr<T>)> aFunc)
		{
			for (std::shared_ptr<T> obj : m_Resources)
			{
				aFunc(obj);
			}
		}

		template <class Archive>
		void save(Archive& ar) const
		{
			ar(m_Resources);
		}

		template <class Archive>
		void load(Archive& ar)
		{
			ar(m_Resources);
		}
	private:
		std::vector<std::shared_ptr<T>> m_Resources;
	};
}