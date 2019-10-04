#pragma once

#include <QWidget>
#include <QResizeEvent>
#include <QApplication>

#include <Triton2/Triton.h>

class ViewportRenderWidget : public QWidget
{
	Q_OBJECT;

public:
	explicit ViewportRenderWidget(QWidget* parent)
		: QWidget{ parent }
	{
		setAttribute(Qt::WA_NativeWindow);
		setAttribute(Qt::WA_PaintOnScreen);
		setAttribute(Qt::WA_NoSystemBackground);
	}

	virtual ~ViewportRenderWidget() = default;

	virtual QPaintEngine* paintEngine() const override
	{
		return nullptr;
	}

	void render()
	{
		if (_updatePending == false)
		{
			_updatePending = true;
			QApplication::postEvent(
				this,
				new QEvent{ QEvent::UpdateRequest });
		}
	}

public:
	bool continuousRender = true;

public slots:
	void sizeSliderChanged(int value)
	{
		size = ((float)value) / 10;
	}

protected:
	virtual void paintEvent(QPaintEvent* paintEvent) override
	{
		if (_isInit == false)
			_init();
		render();
	}

	virtual void showEvent(QShowEvent* showEvent) override
	{
		QWidget::showEvent(showEvent);
		if (_isInit == false)
			_init();
	}

	virtual void resizeEvent(QResizeEvent* resizeEvent) override
	{
		QWidget::resizeEvent(resizeEvent);
		auto sz = resizeEvent->size();
		if ((sz.width() < 0) || (sz.height() < 0))
			return;

		//
		// put your resize code here...
		//
		if (renderer != nullptr && renderer->valid())
		{
			renderer->resize();
		}

		// because Qt is not sending update request when resizing smaller
		render();
	}

	virtual bool event(QEvent* event) override
	{
		switch (event->type())
		{
		case QEvent::UpdateRequest:
			_updatePending = false;
			_doRender();
			return true;
		default:
			return QWidget::event(event);
		}
	}

private:
	void _doRender()
	{
		if (isVisible() == false)
			return;
		if (_isInit == false)
			return;

		// you may want to add some code to control rendering frequency
		// and ensure you are not rendering too fast in case of continuous
		// rendering...
		// if you control the rendering frequency, don't forget to make a
		// call to render() if you're not going to do the rendering...


		//
		// do your custom rendering here...
		//

		static float rotation = 0;

		window->update();

		Triton::Matrix44 transformation = Triton::Core::CreateTransformationMatrix(Triton::Vector3(0, 0, 20), Triton::Vector3(0, rotation, 0), Triton::Vector3(size));
		shader->buffer_matrices().Model = transformation;

		renderer->newFrame(1.0, 0.5, 0.5, 0.5);

		shader->update_matrices();
		renderer->render(model->getIndiceCount());

		rotation += 1 * 0.2;

		renderer->endFrame();


		// next frame if rendering continuously
		if (continuousRender == true)
			render();
	}

	void _init()
	{
		// you can grab the native window handler (HWND for Windows) of
		// this widget:
		auto nativeWindowHandler = winId();

		createAssets();
		loadAssets();

		window = Triton::Engine::getInstance().context().newWindow();
		window->initFromCreated({ (HWND)winId() });

		auto[_width, _height] = window->size();
		renderer = Triton::Engine::getInstance().context().newRenderer(window);

		Triton::Engine::getInstance().scenes().setScene("sample");
		Triton::Engine::getInstance().assets().wait();

		shader = Triton::Engine::getInstance().assets()["lightingShader"].as<Triton::ShaderAsset>()->shader();
		model = Triton::Engine::getInstance().assets()["stallMesh"].as<Triton::MeshAsset>()->VAO();
		texture = Triton::Engine::getInstance().assets()["stallTexture"].as<Triton::ImageAsset>()->texture();

		// Enable shader and mesh
		shader->enable();
		model->enable();
		texture->enable();

		// Create matrices
		Triton::Matrix44 transformation = Triton::Core::CreateTransformationMatrix(Triton::Vector3(0, 0, 20), Triton::Vector3(0, 0, 0), Triton::Vector3(1, 1, 1));

		renderer->fov(3.141592654f / 4.0f);
		renderer->nearPlane(0.1f);
		renderer->farPlane(100.0f);

		Triton::Matrix44 projection = renderer->projection();

		Triton::StaticCamera sCamera(Triton::Vector3(0, 5, 50), Triton::Vector3(0, 0, 20));

		// Update shader matrices
		shader->buffer_matrices().Model = transformation;
		shader->buffer_matrices().Projection = projection;
		shader->buffer_matrices().View = sCamera.viewMatrix();

		Triton::Vector3 pos = sCamera.getPosition();
		Triton::Vector3 dir = sCamera.getViewDirection();
		shader->buffer_camera().Position = Triton::Vector4(pos.x, pos.y, pos.z, 1.0f);
		shader->buffer_camera().ViewDirection = Triton::Vector4(dir.x, dir.y, dir.z, 1.0f);

		Triton::reference<Triton::Scene> sampleScene = Triton::Engine::getInstance().scenes()["sample"];

		shader->buffer_lights() = sampleScene->lights();

		// do your init code here...

		_isInit = true;
	}

private:
	void createAssets()
	{
		using namespace Triton;

		Triton::Core::AssetDictionary dict(Triton::Core::AssetDictionary::c_LatestVersion);

		IO::IntermediateAsset meshAsset;
		meshAsset.Version = IO::Serialization::c_Version_Latest;
		meshAsset.Name = "stallMesh";
		meshAsset.Type = IO::Serialization::v_latest::c_MeshType;
		meshAsset.Data = std::make_shared<IO::MeshData>();

		IO::createDirectory("../Assets");
		IO::createDirectory("../Assets/Scenes");

		// This is a model for the asset could be anything
		// These models can be found in the Assets folder
		IO::loadMeshFromDisk("D:\\Programming\\Test files\\nfw\\stall.obj", (IO::MeshData*)meshAsset.Data.get());

		IO::saveAssetToDisk("../Assets/stall.asset", &meshAsset);

		dict.associate(meshAsset.Name, { "../Assets/stall.asset", false });


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

		dict.associate(lightingShader.Name, { "../Assets/lightingShader.asset", false });

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

		dict.associate(textureAsset.Name, { "../Assets/texture.asset", false });


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
		sl.Position = Vector3(0, 5, 50);

		//scene.lights().PointLights.push_back(pl);
		//scene.lights().PointLights.push_back(pl2);
		scene.lights().DirLights.push_back(dl);
		//scene.lights().SpotLights.push_back(sl);

		auto& scene_assets = scene.assets();

		scene_assets.push_back("lightingShader");
		scene_assets.push_back("stallMesh");
		scene_assets.push_back("stallTexture");

		IO::saveSceneToDisk("../Assets/Scenes/sampleScene.scene", &scene.toRawData());

		dict.saveToFile("../Assets/dictionary.meta");
	}

	void loadAssets()
	{
		Triton::Engine::getInstance().assets().loadDictionary("../Assets/dictionary.meta");

		Triton::Engine::getInstance().scenes().loadScene("../Assets/Scenes/sampleScene.scene");
	}

private:
	bool _updatePending = false;
	bool _isInit = false;

	float size = 1;

	Triton::Graphics::Window* window = nullptr;
	Triton::Graphics::Renderer* renderer = nullptr;

	Triton::reference<Triton::Graphics::VAO> model;
	Triton::reference<Triton::Graphics::Texture> texture;
	Triton::reference<Triton::Graphics::Shader> shader;
};