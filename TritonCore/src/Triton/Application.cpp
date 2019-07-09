#include "TRpch.h"
#include "Application.h"

#include "Triton\Logger\Log.h"
#include "Events\EventManager.h"
#include "Events\ApplicationEvent.h"
#include "Events\KeyEvent.h"

#include <chrono>

namespace Triton {

	std::tuple<bool, bool> BindVisual(Components::Visual& prevVisual, Components::Visual& newVisual)
	{
		bool diffMat = false;
		bool diffMesh = false;

		if (prevVisual.Material != newVisual.Material)
		{
			diffMat = true;
		}

		if (prevVisual.Mesh != newVisual.Mesh)
		{
			diffMesh = true;
		}

		prevVisual = newVisual;

		return { diffMat , diffMesh };
	}


	Application::Application(const AppSettings& aSettings)
		: EventManager(), EventListener(this)
	{
		m_iManager = new Core::InputManager();
		m_iManager->setEventManager(this);

		// Init graphics, create context and set up event callbacks
		Context = Impl::createContext(aSettings);

		Context->init();
		Context->setContextEventCallBacks(m_iManager);
		Context->window->create(aSettings.WindowWidth, aSettings.WindowHeight);
		Context->init_additional();

		m_SceneManager = new Manager::SceneManager(Context, m_iManager);
		m_ObjectManager = new Manager::ObjectManager();
		m_AssetManager = new Manager::AssetManager();

		m_RenderBuffer1 = new Core::RenderBuffer();
		m_RenderBuffer2 = new Core::RenderBuffer();

		m_CurrentUpdateBuffer = m_RenderBuffer1;
		m_CurrentRenderBuffer = m_RenderBuffer1;

		// Set up relay ptrs
		SceneManager = m_SceneManager;
		Input = m_iManager;
	}
	
	Application::~Application()
	{
		delete m_iManager;

		// Make sure that all resources are freed before freeing the context
		delete m_SceneManager; // Release the assets from all scenes
		delete m_AssetManager; // Release all objects from assets
		delete m_ObjectManager; // Release all objects from Triton

		delete m_renderThread;

		// Clear memory from context
		Impl::destroyContext(Context);
	}

	void Application::Execute()
	{
#if TR_MT_ENABLED == 1
		// Create render thread and start executing immediately
		m_renderThread = new std::thread(std::bind(&Application::renderMT, this));

		// Start update loop
		updateMT();

		m_renderThread->join();
#else
		while (!shouldClose())
		{
			frame();
		}
#endif
	}

	void Application::setup()
	{
		Context->renderer->recalc_matrices();
		PreExecutionSetup();
	}

	void Application::frame()
	{	
		static std::chrono::high_resolution_clock timer;
		using ms = std::chrono::duration<float, std::milli>;
		auto start = timer.now();


		auto updateStart = timer.now();

		m_CurrentUpdateBuffer->clear();

		Context->update();

		Dispatch();

		OnUpdate();

		Render();

		auto updateStop = timer.now();

		prtc_UpdateDelta = std::chrono::duration_cast<ms>(updateStop - updateStart).count() / 1000.0f;

		auto renderStart = timer.now();

		Context->renderer->newFrame(TR_CLEAR_COLOR);
		Context->renderer->default(); // Reset the renderer to it's default state

		m_CurrentUpdateBuffer->exec(Context);

		RenderOnTop();

		Context->renderer->endFrame();

		auto renderStop = timer.now();

		prtc_RenderDelta = std::chrono::duration_cast<ms>(renderStop - renderStart).count() / 1000.0f;


		auto stop = timer.now();
		prtc_Delta = std::chrono::duration_cast<ms>(stop - start).count() / 1000.0f;
	}

	bool Application::shouldClose()
	{
		return Context->window->windowClosed();
	}

	reference<Resource::Asset> Application::createAsset(Resource::AssetCreateParams& params)
	{
		bool alreadyLoaded = false;
		reference<PType::FactoryObject> createdObject;

		// Check this is a create operation
		if (params.Operation == Resource::AssetCreateParams::CreationOperation::CREATE)
		{
			if (params.Type != Resource::AssetCreateParams::AssetType::CUBEMAP && params.Type != Resource::AssetCreateParams::AssetType::SHADER)
			{
				// Check if the asset that we are trying to load isn't already loaded if it is
				// then we can just use the one we cached
				if (m_AlreadyLoaded.find(params.Paths[0]) != m_AlreadyLoaded.end())
				{
					alreadyLoaded = true;
					createdObject = m_ObjectManager->getObject(m_AlreadyLoaded[params.Paths[0]]).as<PType::FactoryObject>();
				}
			}
		}

		// Create mesh
		if (params.Type == Resource::AssetCreateParams::AssetType::MESH)
		{
			// Copy the asset
			if (params.Operation == Resource::AssetCreateParams::CreationOperation::COPY)
			{
				size_t assetID = m_AssetManager->getNextID();

				auto asset = reference<Data::Mesh>(new Data::Mesh(assetID, params.CopyAsset.as<Data::Mesh>()->VAO)).as<Resource::Asset>();

				m_AssetManager->addObject(assetID, asset);

				return asset;
			}

			if (!alreadyLoaded || !createdObject.as<PType::VAO>().valid())
			{
				// Create VAO object
				Triton::PType::VAOCreateParams* vao_params = new Triton::PType::VAOCreateParams();

				Triton::Data::File::ReadMesh(params.Paths[0], vao_params);

				createdObject = Context->factory->createVAO(vao_params);

				size_t objectID = m_ObjectManager->addObject(createdObject.as<PType::PlatformObject>());

				delete vao_params;
			
				m_AlreadyLoaded[params.Paths[0]] = objectID;
			}

			size_t assetID = m_AssetManager->getNextID();

			auto asset = reference<Data::Mesh>(new Data::Mesh(assetID, createdObject.as<PType::VAO>()));

			m_AssetManager->addObject(assetID, asset.as<Resource::Asset>());

			return asset.as<Resource::Asset>();
		}

		// Create material
		if (params.Type == Resource::AssetCreateParams::AssetType::MATERIAL)
		{
			// Copy the asset
			if (params.Operation == Resource::AssetCreateParams::CreationOperation::COPY)
			{
				size_t assetID = m_AssetManager->getNextID();

				auto asset = reference<Data::Material>(new Data::Material(assetID, params.CopyAsset.as<Data::Material>()->Texture)).as<Resource::Asset>();

				m_AssetManager->addObject(assetID, asset);

				return asset;
			}

			if (!alreadyLoaded || !createdObject.as<PType::Texture>().valid())
			{
				// Create Texture object
				Triton::PType::TextureCreateParams* tex_params = new Triton::PType::TextureCreateParams();
				Triton::Data::File::ReadTexture(params.Paths[0], tex_params);

				createdObject = Context->factory->createTexture(tex_params);

				size_t objectID = m_ObjectManager->addObject(createdObject.as<PType::PlatformObject>());

				delete tex_params;

				m_AlreadyLoaded[params.Paths[0]] = objectID;
			}		

			size_t assetID = m_AssetManager->getNextID();

			auto asset = reference<Data::Material>(new Data::Material(assetID, createdObject.as<PType::Texture>()));

			m_AssetManager->addObject(assetID, asset.as<Resource::Asset>());

			return asset.as<Resource::Asset>();
		}

		// Create viewport
		if (params.Type == Resource::AssetCreateParams::AssetType::VIEWPORT)
		{
			// Copy the asset
			if (params.Operation == Resource::AssetCreateParams::CreationOperation::COPY)
			{
				size_t assetID = m_AssetManager->getNextID();

				auto asset = reference<Data::Viewport>(new Data::Viewport(assetID, params.CopyAsset.as<Data::Viewport>()->Framebuffer)).as<Resource::Asset>();

				m_AssetManager->addObject(assetID, asset);

				return asset;
			}

			if (!alreadyLoaded || !createdObject.as<PType::Framebuffer>().valid())
			{
				// Create framebuffer object
				Triton::PType::TextureCreateParams* fbo_params = new Triton::PType::TextureCreateParams();
				fbo_params->width = params.Width;
				fbo_params->height = params.Height;

				createdObject = Context->factory->createFramebuffer(fbo_params);

				size_t objectID = m_ObjectManager->addObject(createdObject.as<PType::PlatformObject>());

				delete fbo_params;
			
				m_AlreadyLoaded[params.Paths[0]] = objectID;
			}
			
			size_t assetID = m_AssetManager->getNextID();

			auto asset = reference<Data::Viewport>(new Data::Viewport(assetID, createdObject.as<PType::Framebuffer>()));

			m_AssetManager->addObject(assetID, asset.as<Resource::Asset>());

			return asset.as<Resource::Asset>();
		}

		// Create image
		if (params.Type == Resource::AssetCreateParams::AssetType::IMAGE)
		{
			// Copy the asset
			if (params.Operation == Resource::AssetCreateParams::CreationOperation::COPY)
			{
				size_t assetID = m_AssetManager->getNextID();

				auto asset = reference<Data::Image>(new Data::Image(assetID, params.CopyAsset.as<Data::Image>()->Bitmap)).as<Resource::Asset>();

				m_AssetManager->addObject(assetID, asset);

				return asset;
			}

			if (!alreadyLoaded || !createdObject.as<PType::Texture>().valid())
			{
				// Create Texture object
				Triton::PType::TextureCreateParams* tex_params = new Triton::PType::TextureCreateParams();
				Triton::Data::File::ReadTexture(params.Paths[0], tex_params);

				createdObject = Context->factory->createTexture(tex_params);

				size_t objectID = m_ObjectManager->addObject(createdObject.as<PType::PlatformObject>());

				m_AlreadyLoaded[params.Paths[0]] = objectID;

				delete tex_params;
			}

			// Create Bitmap object
			Triton::PType::BitmapCreateParams* bitmap_params = new Triton::PType::BitmapCreateParams();

			bitmap_params->width = params.Width;
			bitmap_params->height = params.Height;
			bitmap_params->texture = createdObject.as<PType::Texture>();

			createdObject = Context->factory->createBitmap(bitmap_params);

			auto bitmap = createdObject.as<PType::Bitmap>();
			
			size_t assetID = m_AssetManager->getNextID();

			auto asset = reference<Data::Image>(new Data::Image(assetID, bitmap));

			m_AssetManager->addObject(assetID, asset.as<Resource::Asset>());

			return asset.as<Resource::Asset>();
		}

		// Create cubemap
		if (params.Type == Resource::AssetCreateParams::AssetType::CUBEMAP)
		{
			// Copy the asset
			if (params.Operation == Resource::AssetCreateParams::CreationOperation::COPY)
			{
				size_t assetID = m_AssetManager->getNextID();

				auto asset = reference<Data::Material>(new Data::Material(assetID, params.CopyAsset.as<Data::Material>()->Texture)).as<Resource::Asset>();

				m_AssetManager->addObject(assetID, asset);

				return asset;
			}

			if (!alreadyLoaded || !createdObject.as<PType::Texture>().valid())
			{
				// Create Texture object
				Triton::PType::TextureArrayCreateParams* tex_params = new Triton::PType::TextureArrayCreateParams();

				tex_params->count = 6;

				for (unsigned int i = 0; i < tex_params->count; i++)
				{
					Triton::PType::TextureCreateParams* tex_param = new Triton::PType::TextureCreateParams();
					Triton::Data::File::ReadTexture(params.Paths[i], tex_param);

					tex_params->individualTextures.push_back(tex_param);
				}
				
				tex_params->width = tex_params->individualTextures[0]->width;
				tex_params->height = tex_params->individualTextures[0]->height;
				tex_params->BPP = tex_params->individualTextures[0]->BPP;

				createdObject = Context->factory->createCubeTexture(tex_params);

				size_t objectID = m_ObjectManager->addObject(createdObject.as<PType::PlatformObject>());

				delete tex_params;

				m_AlreadyLoaded[params.Paths[0]] = objectID;
			}

			size_t assetID = m_AssetManager->getNextID();

			auto asset = reference<Data::Material>(new Data::Material(assetID, createdObject.as<PType::Texture>()));

			m_AssetManager->addObject(assetID, asset.as<Resource::Asset>());

			return asset.as<Resource::Asset>();
		}

		// Create shader
		if (params.Type == Resource::AssetCreateParams::AssetType::SHADER)
		{
			// Copy the asset
			if (params.Operation == Resource::AssetCreateParams::CreationOperation::COPY)
			{
				size_t assetID = m_AssetManager->getNextID();

				auto asset = reference<Data::ShaderProgram>(new Data::ShaderProgram(assetID, params.CopyAsset.as<Data::ShaderProgram>()->Program)).as<Resource::Asset>();

				m_AssetManager->addObject(assetID, asset);

				return asset;
			}

			if (!alreadyLoaded || !createdObject.as<PType::Texture>().valid())
			{
				// Create shader
				Triton::PType::ShaderCreateParams* shader_params = new Triton::PType::ShaderCreateParams();

				shader_params->vertexPath = params.Paths[0];
				shader_params->fragmentPath = params.Paths[1];

				shader_params->entryPointVertex = params.Arguments[0];
				shader_params->entryPointFragment = params.Arguments[1];

				Triton::PType::ShaderLayout shader_layout = Triton::Data::File::ReadShaderLayout(shader_params);

				shader_params->layout = &shader_layout;

				createdObject = Context->factory->createShader(shader_params);

				size_t objectID = m_ObjectManager->addObject(createdObject.as<PType::PlatformObject>());

				delete shader_params;

				m_AlreadyLoaded[params.Paths[0]] = objectID;
			}

			size_t assetID = m_AssetManager->getNextID();

			auto asset = reference<Data::ShaderProgram>(new Data::ShaderProgram(assetID, createdObject.as<PType::Shader>()));

			m_AssetManager->addObject(assetID, asset.as<Resource::Asset>());

			return asset.as<Resource::Asset>();
		}

		TR_ERROR("Ivalid asset creation parameters");
		return reference<Resource::Asset>(nullptr);
	}

	void Application::renderScene(reference<Scene>& scene, reference<Data::Viewport>& renderTo, bool clearFBO)
	{
		// Bind/enable the viewport
		m_CurrentUpdateBuffer->addCommand<RCommands::PushViewport>(renderTo);

		// Render the background

		// Disable culling
		m_CurrentUpdateBuffer->addCommand<RCommands::ChangeContextSetting>
		([&](relay_ptr<PType::Context> context)
		{
			context->cullBufferState(false);
		});

		m_CurrentUpdateBuffer->addCommand<RCommands::PushMesh>(scene->BackgroundMesh);
		m_CurrentUpdateBuffer->addCommand<RCommands::PushMaterial>(scene->BackgroundMaterial);

		m_CurrentUpdateBuffer->addCommand<RCommands::UpdateUniformValues>
		([&](reference<PType::Shader> shader)
		{
			auto trans_mat = Triton::Core::CreateTransformationMatrix(scene->Camera->Position, Vector3(0, 0, 0), Vector3(1, 1, 1));

			shader->setBufferValue("persistant_Persistant", "projectionMatrix", &Context->renderer->projection());
			shader->setBufferValue("object_PerObject", "transformationMatrix", &trans_mat);
			shader->setBufferValue("frame_PerFrame", "viewMatrix", &scene->getViewMatrix());
		});

		// Update shader buffers and render the background
		m_CurrentUpdateBuffer->addCommand<RCommands::UpdateShaderBuffer>(PType::BufferUpdateType::ALL);
		m_CurrentUpdateBuffer->addCommand<RCommands::Draw>();

		// Enable culling
		m_CurrentUpdateBuffer->addCommand<RCommands::ChangeContextSetting>
		([&](relay_ptr<PType::Context> context)
		{
			context->cullBufferState(true);
		});

		// Render all entities in a scene
		scene->m_CurrVisual.Material = -1;
		scene->m_CurrVisual.Mesh = -1;
		scene->Entities->view<Components::Transform, Components::Visual>().each([&](auto& transform, auto& visual) {

			// Check if visual needs to be bound
			auto[changeMat, changeMesh] = BindVisual(scene->m_CurrVisual, visual);

			if (changeMat)
			{
				// Get material used by the entity
				auto material = m_AssetManager->getObject(visual.Material).as<Triton::Data::Material>();

				// Push material
				m_CurrentUpdateBuffer->addCommand<RCommands::PushMaterial>(material);

				// Update uniforms
				m_CurrentUpdateBuffer->addCommand<RCommands::UpdateUniformValues>
				(
					[&](reference<PType::Shader> shader)
					{
						shader->setBufferValue("persistant_Persistant", "projectionMatrix", &Context->renderer->projection());
						shader->setBufferValue("frame_PerFrame", "viewMatrix", &scene->getViewMatrix());
						shader->setBufferValue("CameraBuffer", "cameraPosition", &scene->Camera->Position);

						// Push lights
						for (size_t i = 0; i < scene->m_Lights.size(); i++)
						{
							scene->m_Lights[i]->bind(shader);
						}

						shader->updateBuffers(PType::BufferUpdateType::FRAME);
						shader->updateBuffers(PType::BufferUpdateType::PERSISTANT);
					}
				);
			}

			// Get mesh object
			reference<Data::Mesh>& mesh = m_AssetManager->getObject(visual.Mesh).as<Data::Mesh>();
			reference<PType::VAO>& vao = mesh->VAO;

			if (changeMesh)
			{
				// Push mesh
				m_CurrentUpdateBuffer->addCommand<RCommands::PushMesh>(mesh);
			}

			// Update uniforms
			m_CurrentUpdateBuffer->addCommand<RCommands::UpdateUniformValues>
			(
				[&](reference<PType::Shader> shader)
				{
					auto trans_mat = Triton::Core::CreateTransformationMatrix(transform.Position, transform.Rotation, transform.Scale);
					shader->setBufferValue("object_PerObject", "transformationMatrix", &trans_mat);

					shader->updateBuffers(PType::BufferUpdateType::OBJECT);
				}
			);

			// Push render command
			m_CurrentUpdateBuffer->addCommand<RCommands::Draw>();
		});

		// Change context settings
		m_CurrentUpdateBuffer->addCommand<RCommands::ChangeContextSetting>
		([&](relay_ptr<PType::Context> context)
		{
			context->depthBufferState(false);
		});

		// Push image shader
		m_CurrentUpdateBuffer->addCommand<RCommands::PushShader>(scene->image_shader);

		// Update uniforms
		m_CurrentUpdateBuffer->addCommand<RCommands::UpdateUniformValues>
		([&](reference<PType::Shader> shader)
		{
			shader->setBufferValue("persistant_Persistant", "projectionMatrix", &Context->renderer->orthographic());
			shader->setBufferValue("frame_PerFrame", "viewMatrix", &scene->getViewMatrix());

			shader->updateBuffers(PType::BufferUpdateType::FRAME);
			shader->updateBuffers(PType::BufferUpdateType::PERSISTANT);
		});

		// Render all images in the scene
		scene->Entities->view<Components::Transform, Components::Image>().each([&](auto& transform, auto& imageComp) {

			// Get image object
			reference<Data::Image> image = m_AssetManager->getObject(imageComp.Bitmap).as<Data::Image>();

			// Push image
			m_CurrentUpdateBuffer->addCommand<RCommands::PushImage>(image);

			// Update uniforms
			m_CurrentUpdateBuffer->addCommand<RCommands::UpdateUniformValues>
			([&](reference<PType::Shader> shader)
			{
				auto trans_mat = Triton::Core::CreateTransformationMatrix(transform.Position, transform.Rotation, transform.Scale);
				shader->setBufferValue("object_PerObject", "transformationMatrix", &trans_mat);

				shader->updateBuffers(PType::BufferUpdateType::OBJECT);
			});

			// Push render command
			m_CurrentUpdateBuffer->addCommand<RCommands::Draw>();
		});

		// Change context settings
		m_CurrentUpdateBuffer->addCommand<RCommands::ChangeContextSetting>
		([&](relay_ptr<PType::Context> context)
		{
			context->depthBufferState(true);
			context->renderer->default();
		});
	}

	void Application::renderCustomScene(reference<SceneBase>& scene)
	{
		// Pass the render buffer to the scene
		scene->render(m_CurrentRenderBuffer);
	}

	void Application::Restart()
	{
		PreExecutionSetup();
	}

	void Application::updateMT()
	{
		while (!shouldClose())
		{
			static std::chrono::high_resolution_clock timer;
			using ms = std::chrono::duration<float, std::milli>;

			auto updateStart = timer.now();

			Context->update();

			Dispatch();

			OnUpdate();

			auto updateStop = timer.now();

			prtc_UpdateDelta = std::chrono::duration_cast<ms>(updateStop - updateStart).count() / 1000.0f;
		}
	}

	void Application::renderMT()
	{
		while (!shouldClose())
		{
			static std::chrono::high_resolution_clock timer;
			using ms = std::chrono::duration<float, std::milli>;

			auto renderStart = timer.now();

			m_CurrentRenderBuffer->clear();

			Render();

			Context->renderer->newFrame(TR_CLEAR_COLOR);
			Context->renderer->default();

			m_CurrentRenderBuffer->exec(Context);

			RenderOnTop();

			Context->renderer->endFrame();

			auto renderStop = timer.now();

			prtc_RenderDelta = std::chrono::duration_cast<ms>(renderStop - renderStart).count() / 1000.0f;

		}
	}
}