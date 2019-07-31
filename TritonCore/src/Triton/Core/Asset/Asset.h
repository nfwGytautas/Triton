#pragma once

#include "TritonPlatform/PlatformAbstraction.h"

namespace Triton
{
	namespace Manager
	{
		class AssetManager;
	}

	namespace Resource
	{
		class Asset;

		// Structure that specifies what type of asset should be created
		// similar to DirectX API
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
				IMAGE,
				CUBEMAP,
				SHADER,
				TEXTURE
			} Type = AssetType::NONE;

			// Enum that specifies how the engine will interpret the data
			// by default CREATE, if the Operation is COPY identical copy 
			// of CopyAsset will be created
			enum class CreationOperation : size_t
			{
				CREATE,
				COPY
			} Operation = CreationOperation::CREATE;

			// If asset is an external file the engine will take the path according to the asset to load it
			// say if the asset is a mesh or material it will take the path in the [0] index if it's a cube map
			// it will take [0] ~ [5] paths {right, left, bottom, top, front, back}
			// for shaders it will take [0] ~ [1] {vertex, pixel}
			// else don't specify
			std::string Paths[6];

			// If asset requires some special arguments they should be specified here
			// for shaders it will take the [0] ~ [1] {vertex entry shader point, pixel shader entry point}
			// else don't specify
			std::string Arguments[2];

			// For copying the object the already existent asset must be specified
			// else don't specify
			reference<Asset> CopyAsset;

			// Width of created asset used when creating Bitmap or Viewports
			// else don't specify
			unsigned int Width;

			// Height of created asset used when creating Bitmap or Viewports
			// else don't specify
			unsigned int Height;

			// Name of the asset the value is ignored if string representations are disabled
			std::string Name;

			// Specify if the created asset should be cached
			bool CacheResult = true;

			// If specified the asset manager will use these params to create the asset
			// leave as nullptr if you want the asset manager to handle the loading
			// the params are set to nullptr after creating but NOT deleted
			Triton::PType::FactoryCreateParams* CreateParams = nullptr;

			// If specified the asset manager will use this PType object to create the asset
			// it is up to the user to provide a valid PType else errors might occur
			// this variable is set to nullptr after creating an asset
			reference<Triton::PType::PlatformObject> PType = nullptr;
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

#if TR_STRING_REPRESENTATIONS == 1
			// Get the asset name the method is
			// hidden if string representations are disabled
			std::string getName()
			{
				return m_Name;
			}

			// Get the reference to the asset name the method is
			// hidden if string representations are disabled
			std::string& getNameRef()
			{
				return m_Name;
			}
#endif
		private:
			bool m_destroyOnUnload = true;
			bool m_isDestroyed = false;

			size_t m_AssetID;
			
#if TR_STRING_REPRESENTATIONS == 1
			std::string m_Name;
#endif

			friend Manager::AssetManager;
		};
	}
}