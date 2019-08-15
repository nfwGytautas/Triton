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
#include <Triton/Script/ScriptEngine.h>

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
		Layers::PreRender::c_layer2,
		Layers::Render::c_layer2,
		(
			Core::ReceivedMessages::ClassRegistered |
			Core::ReceivedMessages::Update |
			Core::ReceivedMessages::Render |
			Core::ReceivedMessages::PreRender
		)
	};

	class EditorScene : public SceneBase, Utility::EventInterface
	{
	public:
		EditorScene();
		virtual ~EditorScene();

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

		// Inherited via SceneBase
		virtual void setupForRendering() override;
		virtual void setupForObjects() override;
		virtual void onRenderObject(reference<GameObject>& object, Components::Transform& transform, Components::Visual& visual) override;
		virtual void relayMessage(size_t message, void * payload) override;
		virtual void onRenderDone() override;
	private:
		void onRegistered();
		virtual void onUpdate() override;

		void loadResources();
		void createEntities();

		void messageBox(std::string content);
	private:
		TR_EDTR_STATE(m_edtr_state)

		Triton::reference<GameWindow> m_gameWindow;

		reference<Core::RenderBuffer> m_mainRenderBuffer;
		reference<PType::Context> m_graphicsContext;
		reference<Core::InputManager> m_input;
		reference<Manager::AssetManager> m_assetManager;
		reference<Utility::Timer> m_timer;
		reference<Script::ScriptEngine> m_scriptEngine;

		reference<Script::DynamicSharpScript> m_codeExtension;
		reference<Script::DynamicSharpScript> m_dialogExtension;

		ImGuiIO* m_imguiIO;
	public:
		struct WidgetIsOpen
		{
			bool dockspace = false;
			bool viewport = true;
			bool logWindow = true;
			bool metrics = true;
			bool sceneView = true;
			bool assetWindow = true;
			bool inspectorWindow = true;
			
			bool materialViewport = false;
		};

		struct _Callbacks
		{
			std::function<void()> CompileScripts;
			std::function<void(std::string)> NewProject;
			std::function<void()> SaveProject;
			std::function<void()> OpenProject;
			std::function<void(std::string)> ShowMessage;
		} Callbacks;
	private:
		WidgetIsOpen m_isOpen;

		std::unique_ptr<Camera> edtr_Camera;

		reference<Data::Mesh> edtr_3DPOINTER;
		reference<Data::Material> edtr_mat_3DPOINTER;

		reference<GameObject> edtr_pointer;
	};

}