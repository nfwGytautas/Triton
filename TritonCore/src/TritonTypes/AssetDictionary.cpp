#include "TRpch.h"
#include "AssetDictionary.h"

#include <shared_mutex>

#include "Triton2/Assert.h"

#include "Triton2/Utility/Log.h"

#include "Triton2/File/Serialize.h"

namespace Triton
{
	namespace Core
	{
		class AssetDictionary::AssetDictionaryImpl
		{
		public:
			AssetDictionaryImpl()
				: m_version(c_LatestVersion)
			{
			}

			AssetDictionaryImpl(const AssetDictionaryImpl& other)
				: m_version(other.m_version), m_dictionary(other.m_dictionary)
			{

			}

			AssetDictionaryImpl(const std::string& version)
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

			bool has(const std::string& assetName) const
			{
				std::shared_lock lock(m_dictLock);
				return m_dictionary.find(assetName) != m_dictionary.end();
			}

			const AssetMeta& getData(const std::string& assetName) const
			{
				std::shared_lock lock(m_dictLock);
				TR_CORE_ASSERT(m_dictionary.find(assetName) != m_dictionary.end(),
					"The asset dictionary does not have metadata for the specific asset");

				return m_dictionary.at(assetName);
			}

			void associate(const std::string& assetName, const AssetMeta& metaData)
			{
				std::unique_lock lock(m_dictLock);
				TR_CORE_ASSERT(m_dictionary.find(assetName) == m_dictionary.end(),
					"Asset with the specified name already has an association");

				m_dictionary[assetName] = metaData;
			}

			void append(const AssetDictionaryImpl& dict)
			{
				std::unique_lock lock(m_dictLock);
				if (dict.m_version != this->m_version)
				{
					TR_SYSTEM_WARN("Dictionary version mismatch detected: Appending with '{0}', to '{1}'", dict.m_version, this->m_version);
					TR_WARN("Dictionary version mismatch detected: Appending with '{0}', to '{1}'", dict.m_version, this->m_version);
				}

				this->m_dictionary.insert(dict.m_dictionary.begin(), dict.m_dictionary.end());
			}

			size_t size() const
			{
				std::shared_lock lock(m_dictLock);
				return m_dictionary.size();
			}

			void loadFromFile(const std::string& path)
			{
				std::unique_lock lock(m_dictLock);

				// Create a cereal xml output archive
				std::ifstream os(path);
				cereal::XMLInputArchive archive(os);

				archive(m_version);

				if (m_version != c_LatestVersion)
				{
					TR_SYSTEM_WARN("Asset dictionary version '{0}' is outdated, the newest version is '{1}'", m_version, c_LatestVersion);
					TR_WARN("Asset dictionary version '{0}' is outdated, the newest version is '{1}'", m_version, c_LatestVersion);
				}

				if (m_version == c_v_00_00_00)
				{
					load_v_00_00_00(archive);
				}
			}

			void saveToFile(const std::string& path)
			{
				std::unique_lock lock(m_dictLock);

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
		private:
			/**
			 * Loads an asset dictionary whose version is 00.00.00
			 *
			 * @param archive Initialized xml input archive
			 */
			void load_v_00_00_00(cereal::XMLInputArchive& archive)
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

			/**
			 * Saves an asset dictionary whose version is 00.00.00
			 *
			 * @param archive Initialized xml output archive
			 */
			void save_v_00_00_00(cereal::XMLOutputArchive& archive)
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

		private:
			/// Version of the asset dictionary
			std::string m_version;

			/// Unordered map used to associate an asset name to it's meta structure
			std::unordered_map<std::string, AssetMeta> m_dictionary;

			/// Mutex to make sure the dictionary is thread safe
			mutable std::shared_mutex m_dictLock;
		};


		/// Latest version of asset dictionary
		const char* AssetDictionary::c_LatestVersion = AssetDictionary::c_v_00_00_00;

		/// 00.00.00 Version of the asset dictionary
		const char* AssetDictionary::c_v_00_00_00 = "00.00.00";

		AssetDictionary::AssetDictionary()
		{
			m_impl = new AssetDictionaryImpl();
		}

		AssetDictionary::AssetDictionary(const AssetDictionary& other)
		{
			m_impl = new AssetDictionaryImpl(*other.m_impl);
		}

		AssetDictionary::AssetDictionary(const std::string& version)
		{
			m_impl = new AssetDictionaryImpl(version);
		}

		AssetDictionary::~AssetDictionary()
		{
			delete m_impl;
		}


		bool AssetDictionary::has(const std::string& assetName) const
		{
			return m_impl->has(assetName);
		}

		const AssetDictionary::AssetMeta& AssetDictionary::getData(const std::string& assetName) const
		{
			return m_impl->getData(assetName);
		}

		void AssetDictionary::associate(const std::string& assetName, const AssetMeta& metaData)
		{
			m_impl->associate(assetName, metaData);
		}

		void AssetDictionary::append(const AssetDictionary& dict)
		{
			m_impl->append(*dict.m_impl);
		}

		size_t AssetDictionary::size() const
		{
			return m_impl->size();
		}

		AssetDictionary* AssetDictionary::loadFromFile(const std::string& path)
		{
			TR_SYSTEM_TRACE("Loading dictionary: '{0}'", path);

			AssetDictionary* dict = new AssetDictionary(c_LatestVersion);
			
			dict->m_impl->loadFromFile(path);

			return dict;
		}

		void AssetDictionary::saveToFile(const std::string& path)
		{
			TR_SYSTEM_TRACE("Saving dictionary: '{0}'", path);

			m_impl->saveToFile(path);
		}
	}
}