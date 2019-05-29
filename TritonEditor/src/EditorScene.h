#pragma once

#include <string>
#include "Triton/Core/Scene.h"
#include "Triton/Events/EventManager.h"
#include "Triton/Events/EventInterface.h"
#include <imgui.h>


namespace Triton
{

	class EditorScene : public SceneBase, Utility::EventInterface
	{
	public:
		EditorScene(PType::Context* context, Core::EventManager* manager);

		// Inherited via SceneBase
		virtual void update(float delta) override;
		virtual void render() override;
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
	private:
		ImGuiIO* m_imguiIO;
		bool m_dockspace = false;

	};

}