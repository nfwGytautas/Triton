#include <iostream>
#include "Triton2/Triton.h"

Triton::Graphics::Context* context;
Triton::Graphics::Window* window;
Triton::Graphics::Renderer* renderer;
Triton::Core::AssetManager* assets;

void createAssets()
{
	using namespace Triton;

	IO::IntermediateAsset meshAsset;
	meshAsset.Version = IO::Serialization::c_Version_Latest;
	meshAsset.Name = "stallMesh";
	meshAsset.Type = IO::Serialization::v_latest::c_MeshType;
	meshAsset.Data = std::make_shared<IO::MeshData>();

	// This is a model for the asset could be anything
	// These models can be found in the Assets folder
	IO::loadMeshFromDisk("D:\\Programming\\Test files\\nfw\\stall.obj", (IO::MeshData*)meshAsset.Data.get());

	IO::saveAssetToDisk("../Assets/stall.asset", &meshAsset);


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
}

void loadAssets()
{
	assets->loadAsset("../Assets/stall.asset");
	assets->loadAsset("../Assets/texture.asset");
	assets->loadAsset("../Assets/lightingShader.asset");
	assets->loadAsset("../Assets/simpleShader.asset");
}

int main()
{
	using namespace Triton;

	Log::init();

	reference<Graphics::Shader> shader;
	reference<Graphics::VAO> model;
	reference<Graphics::Texture> texture;
	
	// Initialize the context
	context = Graphics::Context::create();
	context->init();

	// Create an asset manager for the context
	assets = new Core::AssetManager(context);

	// Create assets
	createAssets();

	// Load the assets
	loadAssets();

	// Create a new window
	window = context->newWindow();
	window->initNew(1280, 720);

	// Key callback
	window->keyboard().charInputCallback([](char c) {TR_SYSTEM_TRACE("KEY: {0}", c); });

	// Create a new renderer for a window
	renderer = context->newRenderer(window);

	assets->wait();

	// Since the assets are loaded they can be acquired here
	// They are loaded using the name that was stored not the file name
	//shader = (*assets)["simpleShader"].as<ShaderAsset>()->shader();
	shader = (*assets)["lightingShader"].as<ShaderAsset>()->shader();
	model = (*assets)["stallMesh"].as<MeshAsset>()->VAO();
	texture = (*assets)["stallTexture"].as<ImageAsset>()->texture();

	// Enable shader and mesh
	shader->enable();
	model->enable();
	texture->enable();

	// Create matrices
	Matrix44 transformation = Core::CreateTransformationMatrix(Triton::Vector3(0, 0, 20), Triton::Vector3(0, 0, 0), Triton::Vector3(1, 1, 1));
	
	renderer->fov(3.141592654f / 4.0f);
	renderer->nearPlane(0.1f);
	renderer->farPlane(100.0f);

	Matrix44 projection = renderer->projection();

	StaticCamera sCamera(Vector3(0, 5, 50), Triton::Vector3(0, 0, 20));

	// Update shader matrices
	shader->buffer_matrices().Model = transformation;
	shader->buffer_matrices().Projection = projection;
	shader->buffer_matrices().View = sCamera.viewMatrix();

	Vector3 pos = sCamera.getPosition();
	Vector3 dir = sCamera.getViewDirection();
	shader->buffer_camera().Position = Vector4(pos.x, pos.y, pos.z, 1.0f);
	shader->buffer_camera().ViewDirection = Vector4(dir.x, dir.y, dir.z, 1.0f);

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
	sl.Position = Vector3(0, 5, 40);

	shader->buffer_lights().PointLights.push_back(pl);
	shader->buffer_lights().PointLights.push_back(pl2);
	//shader->buffer_lights().DirLights.push_back(dl);
	shader->buffer_lights().SpotLights.push_back(sl);

	float rotation = 0;

	while (!window->isClosed())
	{
		window->update();

		Matrix44 transformation = Core::CreateTransformationMatrix(Triton::Vector3(0, 0, 20), Triton::Vector3(0, rotation, 0), Triton::Vector3(1, 1, 1));
		shader->buffer_matrices().Model = transformation;

		renderer->newFrame(1.0, 0.5, 0.5, 0.5);

		shader->update_matrices();
		renderer->render(model->getIndiceCount());

		rotation += 1 * 0.2;

		renderer->endFrame();
	}

#if _DEBUG
	std::cin.get();
#endif

	return 0;
}