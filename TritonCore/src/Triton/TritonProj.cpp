#include "TRpch.h"
#include "TritonProj.h"

#include <Triton/Serialize/Serialize.h>

#include <filesystem>

namespace Triton
{
	// Shortening the namespace
	namespace fs = std::filesystem;

#define PATHS \
	/*Main directory*/ fs::path dir_main = dir;\
	/*Data directory*/ fs::path dir_data = dir_main / c_DirData;\
	/*Main path*/ fs::path pth_main = dir_main / c_ProjFile;\
	/*Asset path*/ fs::path pth_asset = dir_data / c_AssetFile

#pragma region Constants
	const char* TritonProject::c_DirData = "Data/";
	const char* TritonProject::c_GeneralFile = "GameDat0.dat";
	const char* TritonProject::c_AssetFile = "GameDat1.dat";
	const char* TritonProject::c_SceneFile = "GameDat2.dat";
	const char* TritonProject::c_ScriptFile = "GameDat3.dat";
	const char* TritonProject::c_ProjFile = "GameDat4.dat";
#pragma endregion

	TritonProject::TritonProject(const std::string& name)
		: m_projName(name)
	{

	}

	reference<TritonProject> TritonProject::loadFromDisk(const std::string& dir)
	{
		PATHS;

		// Create new instance of TritonProject
		reference<TritonProject> proj = new TritonProject();

		// Load project data
		proj->loadProjFile(pth_main.string());

		// Loads asset data
		proj->loadAssetFile(pth_asset.string());

		return proj;
	}

	void TritonProject::saveToDisk(reference<TritonProject> proj, const std::string& dir)
	{
		PATHS;

		// Assure directories exist
		fs::create_directory(dir_main);
		fs::create_directory(dir_data);

		// Saves project data
		proj->saveProjFile(pth_main.string());
		
		// Saves asset data
		proj->saveAssetFile(pth_asset.string());
	}

	void TritonProject::setName(const std::string& newName)
	{
		m_projName = newName;
	}

	std::string TritonProject::getName() const
	{
		return m_projName;
	}

	void TritonProject::addAsset(Resource::AssetCreateParams params)
	{
		m_assets.push_back(params);
	}

	void TritonProject::removeAsset(size_t id)
	{
		auto it = std::remove_if(m_assets.begin(), m_assets.end(), [&](Resource::AssetCreateParams cParams) { return cParams.ID == id; });
		if (it != m_assets.end())
		{
			m_assets.erase(it);
		}
	}

	std::vector<Resource::AssetCreateParams>& TritonProject::getAssetsToLoad()
	{
		return m_assets;
	}

	TritonProject::TritonProject()
	{

	}

#pragma region Serialization
	void TritonProject::saveProjFile(const std::string& path)
	{
		// Open the output stream for the main project file
		std::ofstream os(path, std::ios::binary);
		cereal::BinaryOutputArchive archive(os);

		archive(m_projName);
	}
	void TritonProject::loadProjFile(const std::string& path)
	{
		// Open the output stream for the main project file
		std::ifstream is(path, std::ios::binary);
		cereal::BinaryInputArchive archive(is);

		archive(m_projName);
	}
	void TritonProject::saveAssetFile(const std::string& path)
	{
		// Open the output stream for the main project file
		std::ofstream os(path, std::ios::binary);
		cereal::BinaryOutputArchive archive(os);

		archive(m_assets);
	}
	void TritonProject::loadAssetFile(const std::string & path)
	{
		// Open the output stream for the main project file
		std::ifstream is(path, std::ios::binary);
		cereal::BinaryInputArchive archive(is);

		archive(m_assets);
	}
#pragma endregion
}