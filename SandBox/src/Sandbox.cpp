#include <iostream>
#include "Triton2/Triton.h"

Triton::Engine& engine = Triton::Engine::getInstance();

Triton::Graphics::Window* window;
Triton::Graphics::Renderer* renderer;

void exportMesh(Triton::Core::AssetDictionary& dict, const std::string& from, const std::string& to, const std::string& name, bool tangents = false)
{
	using namespace Triton;

	IO::IntermediateAsset testMesh;
	testMesh.Version = IO::Serialization::c_Version_Latest;
	testMesh.Name = name;
	testMesh.Type = IO::Serialization::v_latest::c_MeshType;
	testMesh.Data = std::make_shared<IO::MeshData>();

	IO::loadMeshFromDisk(from, (IO::MeshData*)testMesh.Data.get(), tangents);

	IO::saveAssetToDisk(to, &testMesh);

	dict.associate(testMesh.Name, { to, false, Core::AssetDictionary::EntryType::Asset });
}

void exportTexture(Triton::Core::AssetDictionary& dict, const std::string& from, const std::string& to, const std::string& name)
{
	using namespace Triton;

	IO::IntermediateAsset textureAsset;
	textureAsset.Version = IO::Serialization::c_Version_Latest;
	textureAsset.Name = name;
	textureAsset.Type = IO::Serialization::v_latest::c_ImageType;
	textureAsset.Data = std::make_shared<IO::ImageData>();

	auto tDatap = (IO::ImageData*)textureAsset.Data.get();
	// This is the shader for the asset could be anything
	// This shader can be found in the Assets folder
	IO::loadImageFromDisk(from, tDatap);

	IO::saveAssetToDisk(to, &textureAsset);

	dict.associate(textureAsset.Name, { to, false, Core::AssetDictionary::EntryType::Asset });
}

void exportAudio(Triton::Core::AssetDictionary& dict, const std::string& from, const std::string& to, const std::string& name)
{
	using namespace Triton;

	IO::IntermediateAsset audioAsset;
	audioAsset.Version = IO::Serialization::c_Version_Latest;
	audioAsset.Name = name;
	audioAsset.Type = IO::Serialization::v_latest::c_AudioType;
	audioAsset.Data = std::make_shared<IO::AudioData>();

	IO::loadAudioFromDisk(from, (IO::AudioData*)audioAsset.Data.get());

	IO::saveAssetToDisk(to, &audioAsset);

	dict.associate(audioAsset.Name, { to, false, Core::AssetDictionary::EntryType::Asset });
}

void exportFont(Triton::Core::AssetDictionary& dict, const std::string& from, const std::string& to, const std::string& name)
{
	using namespace Triton;

	IO::IntermediateAsset fontAsset;
	fontAsset.Version = IO::Serialization::c_Version_Latest;
	fontAsset.Name = name;
	fontAsset.Type = IO::Serialization::v_latest::c_FontType;
	fontAsset.Data = std::make_shared<IO::FontData>();

	IO::loadFontFromDisk(from, (IO::FontData*)fontAsset.Data.get());

	IO::saveAssetToDisk(to, &fontAsset);

	dict.associate(fontAsset.Name, { to, false, Core::AssetDictionary::EntryType::Asset });
}

void exportShader(Triton::Core::AssetDictionary& dict, 
	const std::string& from, const std::string& to, const std::string& vertex, const std::string& pixel, 
	std::vector<int> flags, const std::string& name)
{
	using namespace Triton;

	IO::IntermediateAsset textShader;
	textShader.Version = IO::Serialization::c_Version_Latest;
	textShader.Name = name;
	textShader.Type = IO::Serialization::v_latest::c_ShaderType;
	textShader.Data = std::make_shared<IO::ShaderData>();

	auto sDatap5 = (IO::ShaderData*)textShader.Data.get();
	IO::readFileFromDisk(from, &sDatap5->source);
	sDatap5->vertexEntry = vertex;
	sDatap5->pixelEntry = pixel;
	
	for (int flag : flags)
	{
		sDatap5->flags.set(flag);
	}

	IO::saveAssetToDisk(to, &textShader);

	dict.associate(textShader.Name, { to, false, Core::AssetDictionary::EntryType::Asset });
}

void exportMaterial(Triton::Core::AssetDictionary& dict, const std::string& to, const std::string& shader, 
	std::vector<std::string> textures, const std::string& name)
{
	using namespace Triton;

	IO::IntermediateAsset materialAsset;
	materialAsset.Version = IO::Serialization::c_Version_Latest;
	materialAsset.Name = name;
	materialAsset.Type = IO::Serialization::v_latest::c_MaterialType;
	materialAsset.Data = std::make_shared<IO::MaterialData>();

	auto mDatap = (IO::MaterialData*)materialAsset.Data.get();
	mDatap->Textures = textures;
	mDatap->Shader = shader;

	IO::saveAssetToDisk(to, &materialAsset);

	dict.associate(materialAsset.Name, { to, false, Core::AssetDictionary::EntryType::Asset });
}

void createAssets()
{
	using namespace Triton;

	Triton::Core::AssetDictionary dict(Triton::Core::AssetDictionary::c_LatestVersion);

	exportAudio(dict, "../Assets/Audio/example_audio.wav", "../Assets/Audio/audioTest.asset", "audioTest");

	exportFont(dict, "../Assets/Fonts/arial.ttf", "../Assets/Fonts/arialFont.asset", "arialFont");

	exportMesh(dict, "C:\\dev\\Triton\\Models\\shaderBall.obj", "../Assets/testMesh.asset", "testMesh");
	exportMesh(dict, "D:\\Programming\\Test files\\nfw\\stall.obj", "../Assets/stall.asset", "stallMesh");
	exportMesh(dict, "C:\\dev\\Triton\\Models\\FlatPlain.obj", "../Assets/flatPlain.asset", "flatPlainMesh");
	exportMesh(dict, "C:\\dev\\Triton\\Models\\FlatPlain.obj", "../Assets/flatPlainNormal.asset", "flatPlainMeshNormal", true);

	exportShader(dict, "C:\\dev\\Triton\\Shaders\\Text.hlsl", "../Assets/textShader.asset", "vertex_text",
		"pixel_text", { Flags::sFlag_Matrices, Flags::sFlag_NoNormals }, "textShader");

	exportShader(dict, "C:\\dev\\Triton\\Shaders\\Simple.hlsl", "../Assets/simpleShader.asset", "vertex_Simple",
		"pixel_Simple", { Flags::sFlag_Matrices }, "simpleShader");

	exportShader(dict, "C:\\dev\\Triton\\Shaders\\MultiTexture.hlsl", "../Assets/multiShader.asset", "vertex_multi",
		"pixel_multi", { Flags::sFlag_Matrices }, "multiTextureNoLightShader");

	exportShader(dict, "C:\\dev\\Triton\\Shaders\\Lighting.hlsl", "../Assets/lightingShader.asset", "vertex_lighting",
		"pixel_lighting", { Flags::sFlag_Matrices, Flags::sFlag_Settings, Flags::sFlag_Lighting, Flags::sFlag_Camera }, "lightingShader");

	exportShader(dict, "C:\\dev\\Triton\\Shaders\\LightMap.hlsl", "../Assets/lightMapShader.asset", "vertex_multi",
		"pixel_multi", { Flags::sFlag_Matrices }, "lightMapShader");

	exportShader(dict, "C:\\dev\\Triton\\Shaders\\AlphaMap.hlsl", "../Assets/alphaMapShader.asset", "vertex_alpha",
		"pixel_alpha", { Flags::sFlag_Matrices }, "alphaMapShader");

	exportShader(dict, "C:\\dev\\Triton\\Shaders\\BumpMap.hlsl", "../Assets/multiShader.asset", "vertex_bump",
		"pixel_bump", { Flags::sFlag_Matrices, Flags::sFlag_TBN, Flags::sFlag_Lighting, Flags::sFlag_Camera }, "bumpShader");

	exportShader(dict, "C:\\dev\\Triton\\Shaders\\SpecMap.hlsl", "../Assets/dbsShader.asset", "vertex_dbs",
		"pixel_dbs", { Flags::sFlag_Matrices, Flags::sFlag_TBN, Flags::sFlag_Lighting, Flags::sFlag_Camera }, "dbsShader");


	exportTexture(dict, "D:\\Programming\\Test files\\nfw\\stallTexture.png", "../Assets/texture.asset", "stallTexture");

	exportTexture(dict, "D:\\Programming\\Test files\\nfw\\dirt01.png", "../Assets/dirt01.asset", "dirt_1");
	exportTexture(dict, "D:\\Programming\\Test files\\nfw\\stone01.png", "../Assets/stone01.asset", "stone_1");
	exportTexture(dict, "D:\\Programming\\Test files\\nfw\\spot.png", "../Assets/spot.asset", "spot");
	exportTexture(dict, "D:\\Programming\\Test files\\nfw\\alphaMap.png", "../Assets/alphaMap.asset", "alphaMap");
	exportTexture(dict, "D:\\Programming\\Test files\\nfw\\bumpMap.png", "../Assets/bumpMap.asset", "normalMap");

	exportTexture(dict, "D:\\Programming\\Test files\\nfw\\specDif.png", "../Assets/specDif.asset", "specMapDiffuse");
	exportTexture(dict, "D:\\Programming\\Test files\\nfw\\specNormal.png", "../Assets/specNormal.asset", "specMapNormal");
	exportTexture(dict, "D:\\Programming\\Test files\\nfw\\specSpec.png", "../Assets/specSpec.asset", "specMapSpecular");

	exportMaterial(dict, "../Assets/material.asset", "lightingShader", { "stallTexture" }, "stallMaterial");

	exportMaterial(dict, "../Assets/plainMaterial.asset", "lightMapShader", { "stone_1", "dirt_1", "spot" }, "plainMaterial");
	exportMaterial(dict, "../Assets/plainAlphaMaterial.asset", "alphaMapShader", { "stone_1", "dirt_1", "spot", "alphaMap" }, "plainAlphaMaterial");

	exportMaterial(dict, "../Assets/plainBumpMaterial.asset", "bumpShader", { "stone_1", "normalMap"}, "plainBumpMaterial");

	exportMaterial(dict, "../Assets/plainSpecMapMaterial.asset", "dbsShader", { "specMapDiffuse", "specMapNormal", "specMapSpecular"}, "plainSpecMapMaterial");

	


	Scene scene("sample");

	PointLight pl;
	pl.Color = Vector3(1.0f, 1.0f, 1.0f);
	pl.Position = Vector3(5, 0, 20);

	PointLight pl2;
	pl2.Color = Vector3(1.0f, 1.0f, 1.0f);
	pl2.Position = Vector3(-5, 0, 20);

	DirectionalLight dl;
	dl.Color = Vector3(1.0f, 1.0f, 1.0f);
	dl.Direction = Vector3(200.0f, 200.0f, 200.0f);

	SpotLight sl;
	sl.Color = Vector3(1.0f, 1.0f, 1.0f);
	sl.Direction = Vector3(0, 0, 20);
	sl.Angle = 5;
	sl.Range = 50;
	sl.Linear = 0.045;
	sl.Quadratic = 0.0075;
	sl.Position = Vector3(0, 5, 25);

	//scene.lights().PointLights.push_back(pl);
	//scene.lights().PointLights.push_back(pl2);
	scene.lights().DirLights.push_back(dl);
	scene.lights().SpotLights.push_back(sl);

	auto& scene_assets = scene.assets();

	scene_assets.push_back("lightingShader");
	scene_assets.push_back("stallMesh");
	scene_assets.push_back("stallTexture");
	scene_assets.push_back("stallMaterial");
	scene_assets.push_back("arialFont");
	scene_assets.push_back("textShader");
	scene_assets.push_back("simpleShader");
	scene_assets.push_back("testMesh");
	scene_assets.push_back("audioTest");
	scene_assets.push_back("flatPlainMesh");
	scene_assets.push_back("plainMaterial");
	scene_assets.push_back("dirt_1");
	scene_assets.push_back("stone_1");
	scene_assets.push_back("multiTextureNoLightShader");
	scene_assets.push_back("lightMapShader");
	scene_assets.push_back("spot");
	scene_assets.push_back("plainAlphaMaterial");
	scene_assets.push_back("alphaMap");
	scene_assets.push_back("alphaMapShader");
	scene_assets.push_back("bumpShader");
	scene_assets.push_back("flatPlainMeshNormal");
	scene_assets.push_back("plainBumpMaterial");
	scene_assets.push_back("normalMap");
	scene_assets.push_back("dbsShader");
	scene_assets.push_back("specMapDiffuse");
	scene_assets.push_back("specMapNormal");
	scene_assets.push_back("specMapSpecular");
	scene_assets.push_back("plainSpecMapMaterial");

	auto registry = scene.entities();
	auto entity = registry->create();
	registry->assign<Components::MetaComponent>(entity, "stall");
	registry->assign<Components::Transform>(entity, Triton::Vector3(0, 5, -20), Triton::Vector3(0, 0, 0), Triton::Vector3(1, 1, 1));
	registry->assign<Components::Visual>(entity, "stallMesh", "stallMaterial");

	auto plainEntity = registry->create();
	registry->assign<Components::MetaComponent>(plainEntity, "plain1");
	registry->assign<Components::Transform>(plainEntity, Triton::Vector3(25, 0, 10), Triton::Vector3(90, 0, 0), Triton::Vector3(1, 1, 1));
	registry->assign<Components::Visual>(plainEntity, "flatPlainMesh", "plainMaterial");

	auto plainEntity2 = registry->create();
	registry->assign<Components::MetaComponent>(plainEntity2, "plain2");
	registry->assign<Components::Transform>(plainEntity2, Triton::Vector3(13, 0, 10), Triton::Vector3(90, 0, 0), Triton::Vector3(1, 1, 1));
	registry->assign<Components::Visual>(plainEntity2, "flatPlainMesh", "plainAlphaMaterial");

	auto plainEntity3 = registry->create();
	registry->assign<Components::MetaComponent>(plainEntity3, "plain3");
	registry->assign<Components::Transform>(plainEntity3, Triton::Vector3(1, 0, 10), Triton::Vector3(70, 180, 0), Triton::Vector3(1, 1, 1));
	registry->assign<Components::Visual>(plainEntity3, "flatPlainMesh", "plainBumpMaterial");

	auto plainEntity4 = registry->create();
	registry->assign<Components::MetaComponent>(plainEntity4, "plain4");
	registry->assign<Components::Transform>(plainEntity4, Triton::Vector3(-11, 0, 10), Triton::Vector3(90, 180, 0), Triton::Vector3(1, 1, 1));
	registry->assign<Components::Visual>(plainEntity4, "flatPlainMesh", "plainSpecMapMaterial");

	scene.cameras().push_back(new StaticCamera("mainCamera", Vector3(0, 5, 50), Triton::Vector3(0, 0, 20)));
	scene.setActiveCamera("mainCamera");

	IO::saveSceneToDisk("../Assets/Scenes/sampleScene.scene", &scene);

	dict.associate("sample", { "../Assets/Scenes/sampleScene.scene", false, Core::AssetDictionary::EntryType::Scene });

	dict.saveToFile("../Assets/dictionary.meta");
}

void loadAssets()
{
	engine.loadDictionary("../Assets/dictionary.meta");

	engine.scenes().loadSceneByName("sample");
}

int main()
{
	using namespace Triton;

	EngineSettings settings;
	settings.in_customInputLoop = false;

	engine.init(settings);

	// Create assets
	createAssets();

	// Load the assets
	loadAssets();

	// Create a new window
	window = engine.graphicsContext().newWindow();
	window->initNew(1280, 720);

	// Key callback
	window->keyboard().charInputCallback([](char c) {TR_SYSTEM_TRACE("KEY: {0}", c); });

	// Create a new renderer for a window
	renderer = engine.graphicsContext().newRenderer(window);

	engine.scenes().setScene("sample");

	// Create matrices	
	renderer->fov(3.141592654f / 4.0f);
	renderer->nearPlane(0.1f);
	renderer->farPlane(100.0f);

	{
		reference<Scene> sampleScene = engine.scenes().currentScene();

		ECS::Entity entity = sampleScene->getByMeta([](const Triton::Components::MetaComponent& comp) { return comp.Name == "stall"; })[0];
		Components::Transform& transform = sampleScene->entities()->get<Components::Transform>(entity);

		engine.assets().wait();

		Utility::Timer timer(false);

		int frames = 0;
		long double frameSum = 0;

		engine.assets().waitFor("audioTest", 0);
		reference<AudioAsset> testAudio = engine.assets().getAsset("audioTest").as<AudioAsset>();

		while (!window->isClosed())
		{
			timer.start();

			engine.graphicsContext().synchronizer().synchronize(0);
			engine.audioContext().synchronizer().synchronize(0);

			window->update();

			renderer->newFrame(1.0f, 0.5f, 0.5f, 0.5f);

			Extension::renderScene(engine.scenes().currentScene(), renderer, &engine.assets());

			// Temporary
			Extension::renderText("Sample text", "arialFont", { 50, 50 }, renderer, &engine.assets());

			testAudio->audio()->play();

			transform.Rotation.y += 1 * 0.2;

			renderer->endFrame();

			timer.end();

			frameSum += 1 / (timer.microseconds() / 1000 / 1000);

			frames++;

			if (frames == 1000)
			{
				frameSum = frameSum / frames;
				TR_SYSTEM_ERROR("AVERAGE FPS FOR 1000 FRAMES: {0}", frameSum);
				frameSum = 0;
				frames = 0;
			}
		}
	}

	engine.shutdown();

#if _DEBUG
	std::cin.get();
#endif

	return 0;
}