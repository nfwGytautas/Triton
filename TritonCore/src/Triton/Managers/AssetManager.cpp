#include "TRpch.h"
#include "AssetManager.h"

#include "TritonPlatform/PlatformAbstraction.h"
#include "Triton/File/File.h"
#include "Triton/Managers/TritonCache.h"

namespace Triton
{
	namespace Manager
	{
		AssetManager::AssetManager()
		{
		}

		AssetManager::~AssetManager()
		{
			cleanUp();
		}

		reference<Resource::Asset> AssetManager::createAsset(Resource::AssetCreateParams& params)
		{
			reference<Resource::Asset> result;
			size_t assetID = getNextID();

			reference<Core::TritonCache> cache = this->getClassByID((size_t)Core::TritonClasses::Cache).as<Core::TritonCache>();
			reference<PType::Context> context = this->getClassByID((size_t)Core::TritonClasses::Context).as<PType::Context>();

			bool inCache  = true;
			reference<PType::FactoryObject>& createdObject = params.PType.as<PType::FactoryObject>();

			if (!createdObject.valid())
			{
				createdObject = cache->checkFObject(params, inCache);
			}

			if (params.Type == Resource::AssetCreateParams::AssetType::MESH) // Create mesh
			{
				// Copy the asset
				if (params.Operation == Resource::AssetCreateParams::CreationOperation::COPY)
				{
					result = reference<Data::Mesh>(new Data::Mesh(assetID, params.CopyAsset.as<Data::Mesh>()->VAO)).as<Resource::Asset>();
				}
				else
				{
					if (!inCache || !createdObject.as<PType::VAO>().valid())
					{
						// Create VAO object
						Triton::PType::VAOCreateParams vao_params;

						if (params.CreateParams != nullptr)
						{
							vao_params = *(Triton::PType::VAOCreateParams*)params.CreateParams;
						}
						else
						{
							Triton::Data::File::ReadMesh(params.Paths[0], &vao_params);
						}
						createdObject = context->factory->createVAO(&vao_params);
					}

					result = reference<Data::Mesh>(new Data::Mesh(assetID, createdObject.as<PType::VAO>())).as<Resource::Asset>();
				}
			}
			else if (params.Type == Resource::AssetCreateParams::AssetType::MATERIAL) // Create material
			{
				// Copy the asset
				if (params.Operation == Resource::AssetCreateParams::CreationOperation::COPY)
				{
					result = reference<Data::Material>(new Data::Material(assetID, params.CopyAsset.as<Data::Material>()->Texture)).as<Resource::Asset>();
				}
				else 
				{
					if (!inCache || !createdObject.as<PType::Texture>().valid())
					{
						if (params.CreateParams != nullptr)
						{
							createdObject = context->factory->createTexture(params.CreateParams);
						}
						else
						{
							Triton::PType::TextureCreateParams tex_params;
							Triton::Data::File::ReadTexture(params.Paths[0], &tex_params);
							createdObject = context->factory->createTexture(&tex_params);
						}
					}
					result = reference<Data::Material>(new Data::Material(assetID, createdObject.as<PType::Texture>())).as<Resource::Asset>();
				}
			}
			else if (params.Type == Resource::AssetCreateParams::AssetType::VIEWPORT) // Create viewport
			{
				// Copy the asset
				if (params.Operation == Resource::AssetCreateParams::CreationOperation::COPY)
				{
					result = reference<Data::Viewport>(new Data::Viewport(assetID, params.CopyAsset.as<Data::Viewport>()->Framebuffer)).as<Resource::Asset>();
				}
				else
				{
					if (!inCache || !createdObject.as<PType::Framebuffer>().valid())
					{
						// Create framebuffer object
						Triton::PType::TextureCreateParams fbo_params;
						fbo_params.width = params.Width;
						fbo_params.height = params.Height;

						createdObject = context->factory->createFramebuffer(&fbo_params);
					}
					result = reference<Data::Viewport>(new Data::Viewport(assetID, createdObject.as<PType::Framebuffer>())).as<Resource::Asset>();
					result.as<Data::Viewport>()->Height = params.Height;
					result.as<Data::Viewport>()->Width = params.Width;
				}
			}
			else if (params.Type == Resource::AssetCreateParams::AssetType::IMAGE) // Create image
			{
				// Copy the asset
				if (params.Operation == Resource::AssetCreateParams::CreationOperation::COPY)
				{
					result = reference<Data::Image>(new Data::Image(assetID, params.CopyAsset.as<Data::Image>()->Bitmap)).as<Resource::Asset>();
				}
				else
				{
					if (!inCache || !createdObject.as<PType::Texture>().valid())
					{
						// Create Texture object
						Triton::PType::TextureCreateParams tex_params;
						Triton::Data::File::ReadTexture(params.Paths[0], &tex_params);

						createdObject = context->factory->createTexture(&tex_params);
					}

					// Create Bitmap object
					Triton::PType::BitmapCreateParams bitmap_params;

					bitmap_params.width = params.Width;
					bitmap_params.height = params.Height;
					bitmap_params.texture = createdObject.as<PType::Texture>();

					createdObject = context->factory->createBitmap(&bitmap_params);

					auto bitmap = createdObject.as<PType::Bitmap>();

					result = reference<Data::Image>(new Data::Image(assetID, bitmap)).as<Resource::Asset>();
				}
			}
			else if (params.Type == Resource::AssetCreateParams::AssetType::CUBEMAP) // Create cubemap
			{
				// Copy the asset
				if (params.Operation == Resource::AssetCreateParams::CreationOperation::COPY)
				{
					result = reference<Data::Material>(new Data::Material(assetID, params.CopyAsset.as<Data::Material>()->Texture)).as<Resource::Asset>();
				}
				else
				{
					if (!inCache || !createdObject.as<PType::CubeTexture>().valid())
					{
						// Create Texture object
						Triton::PType::TextureArrayCreateParams tex_params;

						tex_params.count = 6;

						for (unsigned int i = 0; i < tex_params.count; i++)
						{
							Triton::PType::TextureCreateParams* tex_param = new Triton::PType::TextureCreateParams();
							Triton::Data::File::ReadTexture(params.Paths[i], tex_param);
							tex_params.individualTextures.push_back(tex_param);
						}

						tex_params.width = tex_params.individualTextures[0]->width;
						tex_params.height = tex_params.individualTextures[0]->height;
						tex_params.BPP = tex_params.individualTextures[0]->BPP;

						createdObject = context->factory->createCubeTexture(&tex_params);
					}
					result = reference<Data::Material>(new Data::Material(assetID, createdObject.as<PType::Texture>())).as<Resource::Asset>();
				}
			}
			else if (params.Type == Resource::AssetCreateParams::AssetType::SHADER) // Create shader
			{
				// Copy the asset
				if (params.Operation == Resource::AssetCreateParams::CreationOperation::COPY)
				{
					result = reference<Data::ShaderProgram>(new Data::ShaderProgram(assetID, params.CopyAsset.as<Data::ShaderProgram>()->Program)).as<Resource::Asset>();
				}
				else 
				{
					if (!inCache || !createdObject.as<PType::Shader>().valid())
					{
						// Create shader
						Triton::PType::ShaderCreateParams shader_params;

						shader_params.vertexPath = params.Paths[0];
						shader_params.fragmentPath = params.Paths[1];
									 
						shader_params.entryPointVertex = params.Arguments[0];
						shader_params.entryPointFragment = params.Arguments[1];

						Triton::PType::ShaderLayout shader_layout = Triton::Data::File::ReadShaderLayout(&shader_params);

						shader_params.layout = &shader_layout;

						createdObject = context->factory->createShader(&shader_params);
					}
					result = reference<Data::ShaderProgram>(new Data::ShaderProgram(assetID, createdObject.as<PType::Shader>())).as<Resource::Asset>();
				}
			}
			else if (params.Type == Resource::AssetCreateParams::AssetType::TEXTURE) // Create plain texture
			{
				// Copy the asset
				if (params.Operation == Resource::AssetCreateParams::CreationOperation::COPY)
				{
					result = reference<Data::PlainTexture>(new Data::PlainTexture(assetID, params.CopyAsset.as<Data::PlainTexture>()->Texture)).as<Resource::Asset>();
				}
				else
				{
					if (!inCache || !createdObject.as<PType::Texture>().valid())
					{
						if (params.CreateParams != nullptr)
						{
							createdObject = context->factory->createTexture(params.CreateParams);
						}
						else
						{
							Triton::PType::TextureCreateParams tex_params;
							Triton::Data::File::ReadTexture(params.Paths[0], &tex_params);
							createdObject = context->factory->createTexture(&tex_params);
						}
					}
					result = reference<Data::PlainTexture>(new Data::PlainTexture(assetID, createdObject.as<PType::Texture>())).as<Resource::Asset>();
				}
			}

			if (result.valid())
			{
				addObject(result);

				if (params.CacheResult)
				{
					cache->cacheFObject(params, createdObject);
				}

#if TR_STRING_REPRESENTATIONS == 1
				result->m_Name = params.Name;
#endif

				params.CreateParams = nullptr;
				params.PType = nullptr;

				return result;
			}

			TR_ERROR("Invalid asset creation parameters");
			return reference<Resource::Asset>(nullptr);
		}

		reference<Resource::Asset> AssetManager::getAssetByID(size_t id)
		{
			if (checkReturnability(id))
			{
				return m_IDAssetPairs[id];
			}
			else
			{
				return nullptr;
			}
		}

		void AssetManager::freeAssetByID(size_t id)
		{
			if (checkReturnability(id))
			{
				auto& asset = getAssetByID(id);
				m_createdAssets.erase(std::remove(m_createdAssets.begin(), m_createdAssets.end(), asset), m_createdAssets.end());
				m_IDAssetPairs.erase(id);
			}
		}

#if TR_STRING_REPRESENTATIONS == 1

		reference<Resource::Asset> AssetManager::getAssetByName(std::string name)
		{
			auto& it = std::find_if(m_createdAssets.begin(), m_createdAssets.end(), [&](auto& Asset) {return Asset->getName() == name; });

			if (it != m_createdAssets.end())
			{
				TR_ERROR("Asset with name '{0}' does not exist!", name);
				return nullptr;
			}

			if (checkReturnability((*it)->getAssetID()))
			{
				return m_IDAssetPairs[(*it)->getAssetID()];
			}
			else
			{
				return nullptr;
			}
		}

		void AssetManager::freeAssetByName(std::string name)
		{
			auto& asset = getAssetByName(name);

			if (!asset.valid())
			{
				return;
			}

			m_createdAssets.erase(std::remove(m_createdAssets.begin(), m_createdAssets.end(), asset), m_createdAssets.end());
			m_IDAssetPairs.erase(asset->getAssetID());
		}

#endif

		void AssetManager::cleanUp()
		{
			m_createdAssets.clear();
			m_IDAssetPairs.clear();
			m_nextAssetID = 0;
		}

		bool AssetManager::valid(size_t id)
		{
			if (m_IDAssetPairs.find(id) == m_IDAssetPairs.end())
			{
				return false;
			}
			else
			{
				return true;
			}
		}

		bool AssetManager::checkReturnability(size_t id)
		{
			if (m_IDAssetPairs.find(id) == m_IDAssetPairs.end())
			{
				TR_ERROR("Asset with id: '{0}' doesn't exist.", id);
				return false;
			}

			return true;
		}

		void AssetManager::addObject(reference<Resource::Asset>& asset)
		{
			m_createdAssets.push_back(asset);
			m_IDAssetPairs[asset->getAssetID()] = asset;
		}

		size_t AssetManager::getNextID()
		{
			return m_nextAssetID++;
		}

	}
}