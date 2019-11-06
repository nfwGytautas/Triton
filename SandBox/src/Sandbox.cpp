#include <iostream>
#include "Triton2/Triton.h"

Triton::Engine& engine = Triton::Engine::getInstance();

Triton::Graphics::Window* window;
Triton::Graphics::Renderer* renderer;

void createAssets()
{
	using namespace Triton;

	Triton::Core::AssetDictionary dict(Triton::Core::AssetDictionary::c_LatestVersion);

	IO::IntermediateAsset fontAsset;
	fontAsset.Version = IO::Serialization::c_Version_Latest;
	fontAsset.Name = "arialFont";
	fontAsset.Type = IO::Serialization::v_latest::c_FontType;
	fontAsset.Data = std::make_shared<IO::FontData>();

	IO::loadFontFromDisk("../Assets/Fonts/arial.ttf", (IO::FontData*)fontAsset.Data.get());

	IO::saveAssetToDisk("../Assets/Fonts/arialFont.asset", &fontAsset);

	dict.associate(fontAsset.Name, { "../Assets/Fonts/arialFont.asset", false, Core::AssetDictionary::EntryType::Asset });

	IO::IntermediateAsset textShader;
	textShader.Version = IO::Serialization::c_Version_Latest;
	textShader.Name = "textShader";
	textShader.Type = IO::Serialization::v_latest::c_ShaderType;
	textShader.Data = std::make_shared<IO::ShaderData>();

	auto sDatap5 = (IO::ShaderData*)textShader.Data.get();
	IO::readFileFromDisk("C:\\dev\\Triton\\Shaders\\Text.hlsl", &sDatap5->source);
	sDatap5->vertexEntry = "vertex_text";
	sDatap5->pixelEntry = "pixel_text";
	sDatap5->flags.set(Flags::sFlag_Matrices);

	IO::saveAssetToDisk("../Assets/textShader.asset", &textShader);

	dict.associate(textShader.Name, { "../Assets/textShader.asset", false, Core::AssetDictionary::EntryType::Asset });

	IO::IntermediateAsset testMesh;
	testMesh.Version = IO::Serialization::c_Version_Latest;
	testMesh.Name = "testMesh";
	testMesh.Type = IO::Serialization::v_latest::c_MeshType;
	testMesh.Data = std::make_shared<IO::MeshData>();

	// This is a model for the asset could be anything
	// These models can be found in the Assets folder
	IO::loadMeshFromDisk("C:\\dev\\Triton\\Models\\shaderBall.obj", (IO::MeshData*)testMesh.Data.get());

	IO::saveAssetToDisk("../Assets/testMesh.asset", &testMesh);

	dict.associate(testMesh.Name, { "../Assets/testMesh.asset", false, Core::AssetDictionary::EntryType::Asset });


	IO::IntermediateAsset meshAsset;
	meshAsset.Version = IO::Serialization::c_Version_Latest;
	meshAsset.Name = "stallMesh";
	meshAsset.Type = IO::Serialization::v_latest::c_MeshType;
	meshAsset.Data = std::make_shared<IO::MeshData>();

	// This is a model for the asset could be anything
	// These models can be found in the Assets folder
	IO::loadMeshFromDisk("D:\\Programming\\Test files\\nfw\\stall.obj", (IO::MeshData*)meshAsset.Data.get());

	IO::saveAssetToDisk("../Assets/stall.asset", &meshAsset);

	dict.associate(meshAsset.Name, { "../Assets/stall.asset", false, Core::AssetDictionary::EntryType::Asset });


	IO::IntermediateAsset simpleShader;
	simpleShader.Version = IO::Serialization::c_Version_Latest;
	simpleShader.Name = "simpleShader";
	simpleShader.Type = IO::Serialization::v_latest::c_ShaderType;
	simpleShader.Data = std::make_shared<IO::ShaderData>();

	auto sDatap = (IO::ShaderData*)simpleShader.Data.get();
	// This is the shader for the asset could be anything
	// This shader can be found in the Assets folder
	IO::readFileFromDisk("C:\\dev\\Triton\\Shaders\\Simple.hlsl", &sDatap->source);
	sDatap->vertexEntry = "vertex_Simple";
	sDatap->pixelEntry = "pixel_Simple";
	sDatap->flags.set(Flags::sFlag_Matrices);

	IO::saveAssetToDisk("../Assets/simpleShader.asset", &simpleShader);

	dict.associate(simpleShader.Name, { "../Assets/simpleShader.asset", false });

	IO::IntermediateAsset lightingShader;
	lightingShader.Version = IO::Serialization::c_Version_Latest;
	lightingShader.Name = "lightingShader";
	lightingShader.Type = IO::Serialization::v_latest::c_ShaderType;
	lightingShader.Data = std::make_shared<IO::ShaderData>();

	auto sDatap2 = (IO::ShaderData*)lightingShader.Data.get();
	// This is the shader for the asset could be anything
	// This shader can be found in the Assets folder
	IO::readFileFromDisk("C:\\dev\\Triton\\Shaders\\Lighting.hlsl", &sDatap2->source);
	sDatap2->vertexEntry = "vertex_lighting";
	sDatap2->pixelEntry = "pixel_lighting";
	sDatap2->flags.set(Flags::sFlag_Matrices);
	sDatap2->flags.set(Flags::sFlag_Settings);
	sDatap2->flags.set(Flags::sFlag_Lighting);
	sDatap2->flags.set(Flags::sFlag_Camera);

	IO::saveAssetToDisk("../Assets/lightingShader.asset", &lightingShader);

	dict.associate(lightingShader.Name, { "../Assets/lightingShader.asset", false, Core::AssetDictionary::EntryType::Asset });

	IO::IntermediateAsset textureAsset;
	textureAsset.Version = IO::Serialization::c_Version_Latest;
	textureAsset.Name = "stallTexture";
	textureAsset.Type = IO::Serialization::v_latest::c_ImageType;
	textureAsset.Data = std::make_shared<IO::ImageData>();

	auto tDatap = (IO::ImageData*)textureAsset.Data.get();
	// This is the shader for the asset could be anything
	// This shader can be found in the Assets folder
	IO::loadImageFromDisk("D:\\Programming\\Test files\\nfw\\stallTexture.png", tDatap);

	IO::saveAssetToDisk("../Assets/texture.asset", &textureAsset);

	dict.associate(textureAsset.Name, { "../Assets/texture.asset", false, Core::AssetDictionary::EntryType::Asset });

	IO::IntermediateAsset materialAsset;
	materialAsset.Version = IO::Serialization::c_Version_Latest;
	materialAsset.Name = "stallMaterial";
	materialAsset.Type = IO::Serialization::v_latest::c_MaterialType;
	materialAsset.Data = std::make_shared<IO::MaterialData>();

	auto mDatap = (IO::MaterialData*)materialAsset.Data.get();
	mDatap->MainTexture = "stallTexture";
	mDatap->Shader = "lightingShader";

	IO::saveAssetToDisk("../Assets/material.asset", &materialAsset);

	dict.associate(materialAsset.Name, { "../Assets/material.asset", false, Core::AssetDictionary::EntryType::Asset });

	
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
	scene_assets.push_back("testMesh");

	auto registry = scene.entities();
	auto entity = registry->create();
	registry->assign<Components::MetaComponent>(entity, "stall");
	registry->assign<Components::Transform>(entity, Triton::Vector3(0, 0, 20), Triton::Vector3(0, 0, 0), Triton::Vector3(1, 1, 1));
	registry->assign<Components::Visual>(entity, "stallMesh", "stallMaterial");

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
	window = engine.context().newWindow();
	window->initNew(1280, 720);

	// Key callback
	window->keyboard().charInputCallback([](char c) {TR_SYSTEM_TRACE("KEY: {0}", c); });

	// Create a new renderer for a window
	renderer = engine.context().newRenderer(window);

	engine.scenes().setScene("sample");

	// Create matrices	
	renderer->fov(3.141592654f / 4.0f);
	renderer->nearPlane(0.1f);
	renderer->farPlane(100.0f);

	reference<Scene> sampleScene = engine.scenes().currentScene();
	
	ECS::Entity entity = sampleScene->getByMeta([](const Triton::Components::MetaComponent& comp) { return comp.Name == "stall"; })[0];
	Components::Transform& transform = sampleScene->entities()->get<Components::Transform>(entity);

	engine.assets().wait();

	Utility::Timer timer(false);

	int frames = 0;
	long double frameSum = 0;

	while (!window->isClosed())
	{
		timer.start();

		engine.context().synchronizer().synchronize(0);

		window->update();

		renderer->newFrame(1.0f, 0.5f, 0.5f, 0.5f);

		Extension::renderScene(engine.scenes().currentScene(), renderer, &engine.assets());

		// Temporary
		Extension::renderText("Sample text", "arialFont", { 50, 50 }, renderer, &engine.assets());

		transform.Rotation.y += 1 * 0.2;

		renderer->endFrame();

		timer.end();

		frameSum += 1 / (timer.microseconds() / 1000 / 1000);

		frames++;

		if (frames == 6000)
		{
			frameSum = frameSum / frames;
			TR_SYSTEM_ERROR("AVERAGE FPS FOR 6000 FRAMES: {0}", frameSum);
			frameSum = 0;
			frames = 0;
		}
	}

	engine.shutdown();

#if _DEBUG
	std::cin.get();
#endif

	return 0;
}