#pragma once

#include "TritonPlatform/PlatformAbstraction.h"

namespace Triton
{
	namespace Resource
	{
		class Asset;

		// Structure that specifies what type of asset should be created
		// similiar to DirectX API
		struct AssetCreateParams
		{
			// Enum that specifies the asset type
			// must be specified else defaults to NONE and will not create anything
			enum class AssetType : size_t
			{
				NONE,
				MESH,
				MATERIAL,
				VIEWPORT,
				IMAGE
			} Type = AssetType::NONE;

			// Enum that specifies how the engine will interpret the data
			// by default CREATE, if the Operation is COPY identical copy 
			// of CopyAsset will be created
			enum class CreationOperation : size_t
			{
				CREATE,
				COPY
			} Operation = CreationOperation::CREATE;

			// If asset is an external file the engine will take the primary path to load it
			// else don't specify
			std::string PrimaryPath;

			// If asset is a shader then the engine will interpret this as shader pixel shader
			std::string SecondaryPath;

			// For copying the object the already existant asset must be specified
			// else don't specify
			reference<Asset> CopyAsset;

			// Width of created asset used when creating Bitmap or Viewports
			// else don't specify
			unsigned int Width;

			// Height of created asset used when creating Bitmap or Viewports
			// else don't specify
			unsigned int Height;
		};

		class Asset
		{
		public:
			Asset(size_t assetID)
			: m_AssetID(assetID) {}
			virtual ~Asset() { }

			void dontDestroyOnUnload();
			bool valid() const;

			// Get asset ID
			size_t getAssetID() const
			{
				return m_AssetID;
			}
		private:
			bool m_destroyOnUnload = true;
			bool m_isDestroyed = false;

			size_t m_AssetID;
		};
	}
}