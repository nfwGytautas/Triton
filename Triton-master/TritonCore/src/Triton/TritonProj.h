#pragma once

#include "Triton/Core/Asset/Asset.h"

namespace Triton 
{
	// This is the class that represents a Triton project, which means that
	//  1. All necessary data is stored in this class (e.g Asset paths, scenes, etc.)
	//  2. The engine interprets this class in order to set up the game
	//  3. Triton project is meant to be stored on the disk to be loaded later
	// The way that TritonProject is saved and loaded is in a certain structure:
	//  When loading the project searches for 'c_ProjFile' and then loads all 
	//  'c_DirData' data files then constructs all the data inside those files
	class TritonProject
	{
	private:
#pragma region Constants
		// The directory for game data files
		static const char* c_DirData;

		// The directory for resource files
		static const char* c_DirRes;

		// The file where general game data is stored
		static const char* c_GeneralFile;

		// The file where asset create data is stored
		// NOTE: This is not where assets themselves are stored
		//		 only the path to those assets and how to create them
		static const char* c_AssetFile;

		// The file where scene data is stored
		static const char* c_SceneFile;

		// The file where script data is stored
		static const char* c_ScriptFile;

		// The file where project data is stored
		static const char* c_ProjFile;
#pragma endregion
	public:
		// Constructs a project with specified name
		TritonProject(const std::string& name);

		// Load project from disk from the specified directory
		static reference<TritonProject> loadFromDisk(const std::string& dir);
		// Save project to disk inside the specified directory
		static void saveToDisk(reference<TritonProject> proj, const std::string& dir);

#pragma region Proj
	public:
		// Sets the project name
		void setName(const std::string& newName);

		// Returns project name
		std::string getName() const;
	private:
		// Name of the project
		std::string m_projName;

		// Directory for the project
		std::string m_projDir;
#pragma endregion
		
#pragma region Asset
	public:
		// Adds asset params to the project
		// When loading the project all the create params will be executed
		// Meaning they will be sent to the asset manager and assets will be created
		// In the order in which they are added
		void addAsset(Resource::AssetCreateParams params);

		// Removes an asset with specified ID
		void removeAsset(size_t id);

		// Returns the vector to the assets that are used by the project
		std::vector<Resource::AssetCreateParams>& getAssetsToLoad();

	private:
		// Moves all the assets into the resource folder
		void moveAssets();
	private:
		// Assets used by the project
		std::vector<Resource::AssetCreateParams> m_assets;
#pragma endregion

	private:
		TritonProject();
#pragma region Serialization
	private:
		// Saves data to project file
		void saveProjFile(const std::string& path);
		// Loads data from project file
		void loadProjFile(const std::string& path);

		// Saves asset data
		void saveAssetFile(const std::string& path);
		// Loads asset data
		void loadAssetFile(const std::string& path);
#pragma endregion
	};
}