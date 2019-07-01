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

		// Clear memory from context
		Impl::destroyContext(Context);
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


		Context->renderer->newFrame(0.0f, 0.5f, 0.5f, 0.0f);

		Context->renderer->default(); // Reset the renderer to it's default state
		
		Dispatch();
		
		OnUpdate();	
		
		Render();

		
		Context->renderer->endFrame();
		
		Context->update();
		
		
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
			if (params.Type != Resource::AssetCreateParams::AssetType::CUBEMAP)
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

		TR_CORE_ERROR("Ivalid asset creation parameters");
		return reference<Resource::Asset>(nullptr);
	}

	void Application::renderScene(reference<Scene>& scene, reference<Data::Viewport>& renderTo, bool clearFBO)
	{
		unsigned int width = renderTo->Width;
		unsigned int height = renderTo->Height;

		auto& frameBuffer = renderTo->Framebuffer;

		frameBuffer->enable();
		
		if (clearFBO)
		{
			frameBuffer->clear(1.0f, 0.0f, 1.0f, 0.0f);
		}

		float fov = 3.141592654f / 4.0f;
		auto proj_mat = Triton::Core::CreateProjectionMatrix(width, height, fov, 0.1f, 100.0f);

		// Render background first
		Context->cullBufferState(false);

		scene->BackgroundMaterial->Shader->enable();

		scene->BackgroundMaterial->Shader->setBufferValue("persistant_Persistant", "projectionMatrix", &proj_mat);


		scene->BackgroundMaterial->Texture->enable();

		auto trans_mat = Triton::Core::CreateTransformationMatrix(scene->Camera->Position, Vector3(0, 0, 0), Vector3(1, 1, 1));

		scene->BackgroundMaterial->Shader->setBufferValue("object_PerObject", "transformationMatrix", &trans_mat);


		scene->BackgroundMaterial->Shader->updateBuffers(Triton::PType::BufferUpdateType::PERSISTANT);
		scene->BackgroundMaterial->Shader->updateBuffers(PType::BufferUpdateType::FRAME);
		scene->BackgroundMaterial->Shader->updateBuffers(Triton::PType::BufferUpdateType::OBJECT);

		scene->BackgroundMesh->VAO->enable();

		Context->renderer->render(scene->BackgroundMesh->VAO.as<PType::Renderable>());

		Context->cullBufferState(true);

		reference<PType::Shader> currentShader;
		scene->m_CurrVisual.Material = -1;
		scene->m_CurrVisual.Mesh = -1;

		scene->Entities->view<Components::Transform, Components::Visual>().each([&](auto& transform, auto& visual) {

			auto[changeMat, changeMesh] = BindVisual(scene->m_CurrVisual, visual);

			if (changeMat)
			{
				auto material = m_AssetManager->getObject(visual.Material).as<Triton::Data::Material>();

				currentShader = material->Shader;

				currentShader->enable();

				currentShader->setBufferValue("persistant_Persistant", "projectionMatrix", &proj_mat);
				currentShader->setBufferValue("LightBuffer", "specularPower", &material->Shininess);

				currentShader->updateBuffers(PType::BufferUpdateType::FRAME);
				currentShader->updateBuffers(Triton::PType::BufferUpdateType::PERSISTANT);

				auto& texture = material->Texture;

				texture->enable();

				//Shader->setUniformInt("material.matTexture", object()->Slot);
				//Shader->setUniformVector3("material.ambient", Ambient);
				//Shader->setUniformVector3("material.diffuse", Diffuse);
				//Shader->setUniformVector3("material.specular", Specular);
			}

			reference<PType::VAO>& vao = m_AssetManager->getObject(visual.Mesh).as<Data::Mesh>()->VAO;

			if (changeMesh)
			{
				vao->enable();
			}

			auto trans_mat = Triton::Core::CreateTransformationMatrix(transform.Position, transform.Rotation, transform.Scale);
			scene->model_shader->setBufferValue("object_PerObject", "transformationMatrix", &trans_mat);

			scene->model_shader->updateBuffers(PType::BufferUpdateType::OBJECT);
			Context->renderer->render(vao.as<PType::Renderable>());
		});

		Context->depthBufferState(false);
		
		scene->image_shader->enable();
		
		auto ortho_mat = Triton::Core::CreateOrthographicMatrix(width, height, 0.1f, 100.0f);
		scene->image_shader->setBufferValue("persistant_Persistant", "projectionMatrix", &ortho_mat);

		scene->image_shader->updateBuffers(PType::BufferUpdateType::FRAME);
		
		scene->image_shader->updateBuffers(Triton::PType::BufferUpdateType::PERSISTANT);

		scene->Entities->view<Components::Transform, Components::Image>().each([&](auto& transform, auto& imageComp) {
		
			reference<Data::Image> image = m_AssetManager->getObject(imageComp.Bitmap).as<Data::Image>();
		
			image->Bitmap->enable();
		
			auto trans_mat = Triton::Core::CreateTransformationMatrix(transform.Position, transform.Rotation, transform.Scale);
			scene->image_shader->setBufferValue("object_PerObject", "transformationMatrix", &trans_mat);
		
			scene->image_shader->updateBuffers(PType::BufferUpdateType::OBJECT);
			Context->renderer->render(image->Bitmap.as<PType::Renderable>());
		});

		Context->depthBufferState(true);

		Context->renderer->default();
	}

	void Application::renderObject(reference<Data::Mesh>& mesh, reference<Data::Material>& material, Matrix44 location, reference<Data::Viewport>& renderTo, bool clearFBO)
	{
		unsigned int width = renderTo->Width;
		unsigned int height = renderTo->Height;

		auto& frameBuffer = renderTo->Framebuffer;

		frameBuffer->enable();
		
		if (clearFBO)
		{
			frameBuffer->clear(1.0f, 0.0f, 1.0f, 0.0f);
		}

		float fov = 3.141592654f / 4.0f;
		auto proj_mat = Triton::Core::CreateProjectionMatrix(width, height, fov, 0.1f, 100.0f);

		auto& shader = material->Shader;

		shader->enable();

		shader->setBufferValue("persistant_Persistant", "projectionMatrix", &proj_mat);
		//shader->setBufferValue("LightBuffer", "specularPower", &material->Shininess);

		shader->updateBuffers(PType::BufferUpdateType::FRAME);
		shader->updateBuffers(Triton::PType::BufferUpdateType::PERSISTANT);

		auto& texture = material->Texture;

		texture->enable();

		//Shader->setUniformInt("material.matTexture", object()->Slot);
		//Shader->setUniformVector3("material.ambient", Ambient);
		//Shader->setUniformVector3("material.diffuse", Diffuse);
		//Shader->setUniformVector3("material.specular", Specular);

		reference<PType::VAO>& vao = mesh->VAO;

		vao->enable();

		//Matrix44 wvp = location * camView * camProjection;
		shader->setBufferValue("object_Persistant", "wvp", &location);

		shader->updateBuffers(PType::BufferUpdateType::OBJECT);
		Context->renderer->render(vao.as<PType::Renderable>());
	}

	void Application::Restart()
	{
		PreExecutionSetup();
	}
}