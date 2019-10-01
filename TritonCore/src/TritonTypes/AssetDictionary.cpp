#include "TRpch.h"
#include "AssetDictionary.h"

#include "Triton2/Assert.h"

#include "Triton2/Utility/Log.h"

#include "Triton2/File/Serialize.h"

namespace Triton
{
	namespace Core
	{
		/// Latest version of asset dictionary
		const char* AssetDictionary::c_LatestVersion = AssetDictionary::c_v_00_00_00;

		/// 00.00.00 Version of the asset dictionary
		const char* AssetDictionary::c_v_00_00_00 = "00.00.00";

		AssetDictionary::AssetDictionary()
			: m_version(c_LatestVersion)
		{
		}

		AssetDictionary::AssetDictionary(const std::string& version)
			: m_version(version)
		{
			// Check if the specified version is even a valid version
			bool valid = false;
			if (m_version == c_v_00_00_00)
			{
				valid = true;
			}

			TR_CORE_ASSERT(valid, "Invalid version for asset dictionary");
		}

		bool AssetDictionary::has(const std::string& assetName) const
		{
			return m_dictionary.find(assetName) != m_dictionary.end();
		}

		const AssetDictionary::AssetMeta& AssetDictionary::getData(const std::string& assetName) const
		{
			TR_CORE_ASSERT(m_dictionary.find(assetName) != m_dictionary.end(), 
				"The asset dictionary does not have metadata for the specific asset");

			return m_dictionary.at(assetName);
		}

		void AssetDictionary::associate(const std::string& assetName, const AssetMeta& metaData)
		{
			TR_CORE_ASSERT(m_dictionary.find(assetName) == m_dictionary.end(),
				"Asset with the specified name already has an association");

			m_dictionary[assetName] = metaData;
		}

		size_t AssetDictionary::size() const
		{
			return m_dictionary.size();
		}

		AssetDictionary* AssetDictionary::loadFromFile(const std::string& path)
		{
			TR_SYSTEM_TRACE("Loading dictionary: '{0}'", path);

			AssetDictionary* dict = new AssetDictionary(c_LatestVersion);

			// Create a cereal xml output archive
			std::ifstream os(path);
			cereal::XMLInputArchive archive(os);

			archive(dict->m_version);

			if (dict->m_version != c_LatestVersion)
			{
				TR_SYSTEM_WARN("Asset dictionary version '{0}' is outdated, the newest version is '{1}'", dict->m_version, c_LatestVersion);
				TR_WARN("Asset dictionary version '{0}' is outdated, the newest version is '{1}'", dict->m_version, c_LatestVersion);
			}

			if (dict->m_version == c_v_00_00_00)
			{
				dict->load_v_00_00_00(archive);
			}

			return dict;
		}

		void AssetDictionary::saveToFile(const std::string& path)
		{
			TR_SYSTEM_TRACE("Saving dictionary: '{0}'", path);

			// Create a cereal xml output archive
			std::ofstream os(path);
			cereal::XMLOutputArchive archive(os);

			archive(cereal::make_nvp("Version", m_version));

			if (m_version != c_LatestVersion)
			{
				TR_SYSTEM_WARN("Asset dictionary version '{0}' is outdated, the newest version is '{1}'", m_version, c_LatestVersion);
				TR_WARN("Asset dictionary version '{0}' is outdated, the newest version is '{1}'", m_version, c_LatestVersion);
			}

			if (m_version == c_v_00_00_00)
			{
				save_v_00_00_00(archive);
			}
		}

		void AssetDictionary::load_v_00_00_00(cereal::XMLInputArchive& archive)
		{
			archive.setNextName("MetaObjects");
			archive.startNode();

			int size = 0;
			archive.loadSize(size);

			for (int i = 0; i < size; i++)
			{
				std::string key;
				AssetMeta meta;

				archive.setNextName("Object");
				archive.startNode();

				archive.setNextName("Name");
				archive.startNode();
				archive.loadValue(key);
				archive.finishNode();

				archive.setNextName("Details");

				archive(meta);

				archive.finishNode();

				m_dictionary[key] = meta;
			}

			archive.finishNode();

			TR_SYSTEM_TRACE("Dictionary loading complete: '{0}' with '{1}' entries", m_version, m_dictionary.size());
		}

		void AssetDictionary::save_v_00_00_00(cereal::XMLOutputArchive& archive)
		{
			archive.setNextName("MetaObjects");
			archive.startNode();

			for (auto& kvp : m_dictionary)
			{
				archive.setNextName("Object");
				archive.startNode();

				archive.setNextName("Name");
				archive.startNode();
				archive.saveValue(kvp.first);
				archive.finishNode();


				archive.setNextName("Details");
				archive(kvp.second);

				archive.finishNode();
			}

			archive.finishNode();

			TR_SYSTEM_TRACE("Dictionary saving complete: '{0}' with '{1}' entries", m_version, m_dictionary.size());
		}
	}
}