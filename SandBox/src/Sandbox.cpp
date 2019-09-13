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
	meshAsset.Version = IO::Version::c_Version_Latest;
	meshAsset.Name = "stallMesh";
	meshAsset.Type = IO::Version::v_latest::c_MeshType;
	meshAsset.Data = std::make_shared<IO::MeshData>();

	// This is a model for the asset could be anything
	// These models can be found in the Assets folder
	IO::loadMeshFromDisk("D:\\Programming\\Test files\\nfw\\stall.obj", (IO::MeshData*)meshAsset.Data.get());

	IO::saveAssetToDisk("../Assets/stall.asset", &meshAsset);


	IO::IntermediateAsset shaderAsset;
	shaderAsset.Version = IO::Version::c_Version_Latest;
	shaderAsset.Name = "simpleShader";
	shaderAsset.Type = IO::Version::v_latest::c_ShaderType;
	shaderAsset.Data = std::make_shared<IO::ShaderData>();

	auto sDatap = (IO::ShaderData*)shaderAsset.Data.get();
	// This is the shader for the asset could be anything
	// This shader can be found in the Assets folder
	IO::readFileFromDisk("C:\\dev\\Triton\\Shaders\\Simple.hlsl", &sDatap->source);
	sDatap->vertexEntry = "vertex_Simple";
	sDatap->pixelEntry = "pixel_Simple";

	IO::saveAssetToDisk("../Assets/shader.asset", &shaderAsset);

	IO::IntermediateAsset textureAsset;
	textureAsset.Version = IO::Version::c_Version_Latest;
	textureAsset.Name = "stallTexture";
	textureAsset.Type = IO::Version::v_latest::c_ImageType;
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
	assets->loadAsset("../Assets/shader.asset");
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
	//createAssets();

	// Load the assets
	loadAssets();

	// Create a new window
	window = context->newWindow();
	window->initNew(1280, 720);

	// Key callback
	window->keyboard().charInputCallback([](char c) {TR_SYSTEM_TRACE("KEY: {0}", c); });

	// Create a new renderer for a window
	renderer = context->newRenderer(window);

	// Since the assets are loaded they can be acquired here
	// They are loaded using the name that was stored not the file name
	shader = (*assets)["simpleShader"].as<ShaderAsset>()->shader();
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

	StaticCamera sCamera(Vector3(0, 0, 0), Triton::Vector3(0, 0, 5));

	// Update shader matrices
	shader->buffer_matrices().Model = transformation;
	shader->buffer_matrices().Projection = projection;
	shader->buffer_matrices().View = sCamera.viewMatrix();

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