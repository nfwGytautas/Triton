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


	exportShader(dict, "C:\\dev\\Triton\\Shaders\\Text.hlsl", "../Assets/textShader.asset", "vertex_text",
		"pixel_text", { Flags::sFlag_Matrices, Flags::sFlag_NoNormals }, "textShader");

	exportShader(dict, "C:\\dev\\Triton\\Shaders\\NormalShader.tcs", "../Assets/bumpShader.asset", "vertex_bump",
		"pixel_bump", { Flags::sFlag_Settings, Flags::sFlag_Matrices, Flags::sFlag_TBN, Flags::sFlag_Lighting, Flags::sFlag_Camera }, "bumpShader");
	
	// Cerberus model
	exportMesh(dict, "C:\\dev\\Triton\\Models\\Cerberus_by_Andrew_Maximov\\Cerberus_LP.FBX", "../Assets/cerberusMesh.asset", "cerberus_mesh", true);
	// Cerberus diffuse texture
	exportTexture(dict, "C:\\dev\\Triton\\Models\\Cerberus_by_Andrew_Maximov\\Textures\\Cerberus_A.tga", 
		"../Assets/cerberusAlbedo.asset", "cerberus_albedo");
	// Cerberus normal texture
	exportTexture(dict, "C:\\dev\\Triton\\Models\\Cerberus_by_Andrew_Maximov\\Textures\\Cerberus_N.tga",
		"../Assets/cerberusNormal.asset", "cerberus_normal");

	// Cerberus material
	exportMaterial(dict, "../Assets/cerberusMaterial.asset", "bumpShader", { "cerberus_albedo", "cerberus_normal" }, "cerberusMaterial");


	Scene scene("sample");

	PointLight pl;
	pl.Color = Vector3(1.0f, 1.0f, 1.0f);
	pl.Position = Vector3(-5, 0, 10);

	PointLight pl2;
	pl2.Color = Vector3(1.0f, 1.0f, 1.0f);
	pl2.Position = Vector3(5, 0, 10);

	DirectionalLight dl;
	dl.Color = Vector3(1.0f, 1.0f, 1.0f);
	dl.Direction = Vector3(0.0f, 0.0f, 0.0f);

	SpotLight sl;
	sl.Color = Vector3(1.0f, 1.0f, 1.0f);
	sl.Direction = Vector3(0, 0, 20);
	sl.Angle = 5;
	sl.Range = 50;
	sl.Linear = 0.045;
	sl.Quadratic = 0.0075;
	sl.Position = Vector3(0, 0, 25);

	scene.lights().PointLights.push_back(pl);
	scene.lights().PointLights.push_back(pl2);
	scene.lights().DirLights.push_back(dl);
	scene.lights().SpotLights.push_back(sl);

	auto& scene_assets = scene.assets();

	scene_assets.push_back("arialFont");
	scene_assets.push_back("textShader");
	scene_assets.push_back("audioTest");
	scene_assets.push_back("bumpShader");

	// Cerberus
	scene_assets.push_back("cerberus_mesh");
	scene_assets.push_back("cerberus_albedo");
	scene_assets.push_back("cerberusMaterial");
	scene_assets.push_back("cerberus_normal");

	auto registry = scene.entities();
	
	auto cerberus = registry->create();
	registry->assign<Components::MetaComponent>(cerberus, "cerberus");
	registry->assign<Components::Transform>(cerberus, Triton::Vector3(20, 0, 0), Triton::Vector3(90, 90, 0), Triton::Vector3(0.3, 0.3, 0.3));
	registry->assign<Components::Visual>(cerberus, "cerberus_mesh", "cerberusMaterial");



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

		engine.assets().wait();

		Utility::Timer timer(false);

		int frames = 0;
		long double frameSum = 0;

		engine.assets().waitFor("audioTest", 0);
		reference<AudioAsset> testAudio = engine.assets().getAsset("audioTest").as<AudioAsset>();

		unsigned int fbo = renderer->newSurface(320, 320);

		while (!window->isClosed())
		{
			timer.start();

			engine.graphicsContext().synchronizer().synchronize(0);
			engine.audioContext().synchronizer().synchronize(0);

			window->update();

			renderer->setSurface(fbo);
			renderer->newFrame(1.0f, 0.5f, 1.0f, 0.5f);

			Extension::renderScene(engine.scenes().currentScene(), renderer, &engine.assets());

			// Temporary
			Extension::renderText("Sample text", "arialFont", { 50, 50 }, renderer, &engine.assets());


			renderer->default();
			renderer->newFrame(1.0f, 0.5f, 0.5f, 0.5f);

			Extension::renderScene(engine.scenes().currentScene(), renderer, &engine.assets());

			// Temporary
			Extension::renderText("Sample text", "arialFont", { 50, 50 }, renderer, &engine.assets());

			Extension::renderSurface(fbo, { 900, 400 }, renderer, &engine.assets());


			//testAudio->audio()->play();

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