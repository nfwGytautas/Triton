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
		: EventListener()
	{
		m_iManager = new Core::InputManager();
		m_iManager->AddListener(*this);

		// Init graphics, create context and set up event callbacks
		Context = Impl::createContext(aSettings);

		Context->window->create();
		Context->init();

		m_SceneManager = new Manager::SceneManager(Context, m_iManager);

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

		m_iManager->Dispatch();

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

	

	void Application::renderScene(reference<Scene>& scene, reference<Data::Viewport>& renderTo, bool clearFBO)
	{
		/*
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
		});*/
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

			m_iManager->Dispatch();

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