#pragma once

namespace cereal
{
	// Forward declaration
	class XMLInputArchive;

	// Forward declaration
	class XMLOutputArchive;
}

namespace Triton
{
	namespace Core
	{
		/**
		 * Class that stores asset names and the paths to their respective files
		 */
		class AssetDictionary
		{
		public:
			/// Latest version of asset dictionary
			static const char* c_LatestVersion;

			/// 00.00.00 Version of the asset dictionary
			static const char* c_v_00_00_00;
		public:
			/**
			 * Struct that contains the metadata of the asset
			 */
			struct AssetMeta
			{
				/// Path to the asset file
				std::string Path;

				/// If true then the path is to a packet
				bool PacketFile;
			};
		public:
			/**
			 * Creates an empty asset dictionary with the latest version
			 */
			AssetDictionary();

			/**
			 * Creates an empty asset dictionary with the specified format of the specified version
			 *
			 * @param version Version of the dictionary
			 */
			AssetDictionary(const std::string& version);

			/**
			 * Checks if the dictionary has an entry with the specified name
			 *
			 * @param assetName Name of the asset
			 * @return True if the dictionary contains an entry with the specified name
			 */
			bool has(const std::string& assetName) const;

			/**
			 * Returns the metadata struct with associated asset name
			 *
			 * @param assetName Name of the asset
			 * @return The AssetMeta structure of assetName asset
			 */
			const AssetMeta& getData(const std::string& assetName) const;

			/**
			 * Same as getData()
			 *
			 * @param assetName Name of the asset
			 * @return The AssetMeta structure of assetName asset
			 */
			const AssetMeta& operator[](const std::string& assetName) const
			{
				return getData(assetName);
			}

			/**
			 * Associates an asset name with the specified metadata structure
			 *
			 * @param assetName Name of the asset
			 * @param metaData The AssetMeta structure to associate with
			 */
			void associate(const std::string& assetName, const AssetMeta& metaData);

			/**
			 * Get the size of the dictionary
			 *
			 * @return The amount of associations that are made inside this dictionary
			 */
			size_t size() const;

			/**
			 * Loads an asset dictionary from the specified file
			 *
			 * @param path Path to the dictionary file
			 * @return Pointer to the loaded asset dictionary
			 */
			static AssetDictionary* loadFromFile(const std::string& path);

			/**
			 * Saves the asset dictionary to a file
			 *
			 * @param path Path to the file where the dictionary should be saved to
			 */
			void saveToFile(const std::string& path);
		private:
			/**
			 * Loads an asset dictionary whose version is 00.00.00
			 *
			 * @param archive Initialized xml input archive
			 */
			void load_v_00_00_00(cereal::XMLInputArchive& archive);

			/**
			 * Saves an asset dictionary whose version is 00.00.00
			 *
			 * @param archive Initialized xml output archive
			 */
			void save_v_00_00_00(cereal::XMLOutputArchive& archive);

		private:
			/// Version of the asset dictionary
			std::string m_version;

			/// Unordered map used to associate an asset name to it's meta structure
			std::unordered_map<std::string, AssetMeta> m_dictionary;
		};
	}
}