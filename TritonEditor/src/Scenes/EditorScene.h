#pragma once

#include <string>
#include <imgui.h>

#include "Triton/Core/Scene.h"
#include "Triton/Events/EventManager.h"
#include "Triton/Events/EventInterface.h"
#include "Triton/Core/Wrapers/Viewport.h"

#include "Triton/Core/RenderBuffer.h"
#include "Triton/Utility/Utility.h"
#include "Triton/Core/GameWindow/GameWindow.h"


namespace Triton
{

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
	private:
		void onRegistered();
		void onUpdate();
		void onRender();

		void loadResources();
		void createEntities();
		void renderEntities();
	private:
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

	private:
		std::unique_ptr<Camera> edtr_Camera;

		reference<Data::Mesh> edtr_3DPOINTER;
		reference<Data::Material> edtr_mat_3DPOINTER;

		size_t edtr_pointer_id;
	};

}