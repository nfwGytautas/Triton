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
			 * Enum used to specify dictionary entry type
			 */
			enum class EntryType : int
			{
				Asset = 0,
				Scene = 1
			};

			/**
			 * Struct that contains the metadata of the asset
			 */
			struct AssetMeta
			{
				/// Path to the asset file
				std::string Path;

				/// If true then the path is to a packet
				bool PacketFile;

				/**
				 * The type of the entry
				 *
				 * 0 - Asset
				 * 1 - Scene
				 */
				EntryType Type;
			};
		public:
			/**
			 * Creates an empty asset dictionary with the latest version
			 */
			AssetDictionary();

			/**
			 * Copy constructor
			 */
			AssetDictionary(const AssetDictionary& other);

			/**
			 * Creates an empty asset dictionary with the specified format of the specified version
			 *
			 * @param version Version of the dictionary
			 */
			AssetDictionary(const std::string& version);

			/**
			 * Destructor used to clean up all the data used by the dictionary
			 */
			~AssetDictionary();

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
			 * Appends dictionary contents to this dictionary
			 *
			 * @param dict Dictionary to append to this one
			 */
			void append(const AssetDictionary& dict);

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
			class AssetDictionaryImpl;
			AssetDictionaryImpl* m_impl;
		};
	}
}