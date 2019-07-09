#pragma once

#include <string>
#include "Triton/Core/Scene.h"
#include "Triton/Events/EventManager.h"
#include "Triton/Events/EventInterface.h"
#include "Triton/Core/Wrapers/Viewport.h"
#include <imgui.h>

#include "Triton/Core/RenderBuffer.h"


namespace Triton
{

	class EditorScene : public SceneBase, Utility::EventInterface
	{
	public:
		EditorScene(PType::Context* context, reference<Core::InputManager> iManager, Core::EventManager* manager);

		// Inherited via SceneBase
		virtual void update(float delta) override;
		virtual void render(Core::RenderBuffer* renderBuffer) override;
		virtual void destroy() override;


		// Inherited via EventInterface
		virtual bool OnKeyPressed(int aKeycode, int aRepeatCount, int aScancode, int aMods) override;
		virtual bool OnKeyReleased(int aKeycode, int aScancode, int aMods) override;
		virtual bool OnKeyInput(unsigned int aCharInput) override;

		virtual bool OnMouseMoved(double aX, double aY) override;
		virtual bool OnMouseButtonPressed(int aKeycode) override;
		virtual bool OnMouseButtonReleased(int aKeycode) override;
		virtual bool OnMouseScrolled(double aXOffset, double aYOffset) override;

		virtual bool OnWindowResized(int aWidth, int aHeight) override;

		reference<Triton::Data::Viewport> ViewPort;

		float RenderDelta;
		float UpdateDelta;
	private:
		ImGuiIO* m_imguiIO;
		bool m_dockspace = false;
		bool m_viewport = true;
		bool m_logWindow = true;
		bool m_metrics = true;
	};

}