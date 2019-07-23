#pragma once

#include <string>
#include <imgui.h>

#include <Triton/Core/Scene.h>
#include <Triton/Events/EventManager.h>
#include <Triton/Events/EventInterface.h>
#include <Triton/Core/Wrapers/Viewport.h>
		 
#include <Triton/Core/RenderBuffer.h>
#include <Triton/Utility/Utility.h>
#include <Triton/Core/GameWindow/GameWindow.h>

#include "TritonEditor/Data/EditorState.h"
#include "TritonEditor/Macros.h"

namespace Triton
{
	const Triton::Core::ClassRegisterParams c_editorRegisterParams =
	{
		false,
		{},
		"editor_scene",
		Layers::Update::c_layer1,
		Layers::c_nullLayer,
		Layers::Render::c_layer2,
		(
			Core::ReceivedMessages::ClassRegistered |
			Core::ReceivedMessages::Update |
			Core::ReceivedMessages::Render
		)
	};

	class EditorScene : public SceneBase, Utility::EventInterface
	{
	public:
		EditorScene();
		virtual ~EditorScene();

		// Inherited via SceneBase
		virtual void onMessage(size_t message, void* payload) override;


		// Inherited via EventInterface
		virtual bool OnKeyPressed(int aKeycode, int aRepeatCount, int aScancode, int aMods) override;
		virtual bool OnKeyReleased(int aKeycode, int aScancode, int aMods) override;
		virtual bool OnKeyInput(unsigned int aCharInput) override;

		virtual bool OnMouseMoved(double aX, double aY) override;
		virtual bool OnMouseButtonPressed(int aKeycode) override;
		virtual bool OnMouseButtonReleased(int aKeycode) override;
		virtual bool OnMouseScrolled(double aXOffset, double aYOffset) override;

		virtual bool OnWindowResized(int aWidth, int aHeight) override;
		virtual bool onAppDrop(std::vector<std::string> files) override;
	private:
		void onRegistered();
		void onUpdate();
		void onRender();

		void loadResources();
		void createEntities();
		void renderEntities();
		void createShaderballs();
	private:
		TR_EDTR_STATE(m_edtr_state)

		Triton::reference<GameWindow> m_gameWindow;

		reference<Core::RenderBuffer> m_mainRenderBuffer;
		reference<PType::Context> m_graphicsContext;
		reference<Core::InputManager> m_input;
		reference<Manager::AssetManager> m_assetManager;
		reference<Utility::Timer> m_timer;

		ImGuiIO* m_imguiIO;
		bool m_dockspace = false;
		bool m_viewport = true;
		bool m_logWindow = true;
		bool m_metrics = true;
		bool m_sceneView = true;
		bool m_assetWindow = true;
		bool m_inspectorWindow = true;
	private:		
		unsigned int m_prevMatSize = 0;
		bool m_renderShaderballs = false;
		std::unique_ptr<ECS::Registry> m_shaderBalls;

		std::unique_ptr<Camera> edtr_Camera;

		reference<Data::Mesh> edtr_3DPOINTER;
		reference<Data::Material> edtr_mat_3DPOINTER;

		size_t edtr_pointer_id;

		reference<Data::Viewport> edtr_materialViewport;
		reference<Graphics::Light> m_mainMaterialPreviewLight;
		reference<Data::Mesh> edtr_shaderBall;
		std::unique_ptr<Camera> edtr_materialPreviewCamera;
	};

}