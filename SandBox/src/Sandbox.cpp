#include <iostream>
#include "Triton2/Triton.h"

int main()
{
	using namespace Triton;

	Log::init();

	Graphics::Context* context;
	Graphics::Window* window;
	Graphics::Renderer* renderer;

	reference<Graphics::Shader> shader;
	reference<Graphics::VAO> model;
	
	context = Graphics::Context::create();
	context->init();

	Core::AssetManager* assets = new Core::AssetManager(context);

	IO::IntermediateAsset intAsset;
	intAsset.Version = IO::Version::c_Version_Latest;
	intAsset.Name = "stallMesh";
	intAsset.Type = IO::Version::v_latest::c_MeshType;
	intAsset.Data = new IO::MeshData();
	IO::loadMeshFromDisk("D:\\Programming\\Test files\\nfw\\stall.obj", (IO::MeshData*)intAsset.Data);

	IO::saveAssetToDisk("stall.asset", &intAsset);

	assets->loadAsset("stall.asset");

	window = context->newWindow();
	window->initNew(1280, 720);

	window->keyboard().charInputCallback([](char c) {TR_SYSTEM_TRACE("KEY: {0}", c); });

	renderer = context->newRenderer(window);

	IO::ShaderData sData;
	sData.pathToSource = "D:\\Programming\\Test files\\nfw\\shaders\\directx\\Simple.hlsl";
	sData.vertexEntry = "vertex_Simple";
	sData.pixelEntry = "pixel_Simple";

	shader = context->newShader(sData);

	shader->enable();

	model = (*assets)["stallMesh"].as<MeshAsset>()->VAO();

	model->enable();

	Matrix44 transformation = Core::CreateTransformationMatrix(Triton::Vector3(0, 0, 50), Triton::Vector3(0, 0, 0), Triton::Vector3(1, 1, 1));

	shader->buffer_matrices().Transformation = transformation;

	while (!window->isClosed())
	{
		window->update();

		renderer->newFrame(1.0, 0.5, 0.5, 0.5);

		shader->update_matrices();
		renderer->render(model->getIndiceCount());

		renderer->endFrame();
	}

#if _DEBUG
	std::cin.get();
#endif

	return 0;
}