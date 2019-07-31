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

	const Triton::Core::ClassRegisterParams c_testSceneRegisterParams =
	{
		false,
		{},
		"test_scene",
		Layers::Update::c_layer1,
		Layers::c_nullLayer,
		Layers::Render::c_layer1,
		(
			Core::ReceivedMessages::ClassRegistered |
			Core::ReceivedMessages::Update |
			Core::ReceivedMessages::Render
		)
	};

	class TestScene : 
		public Scene,
		public EventListener
	{
	public:
		TestScene();
		virtual ~TestScene() {}

		virtual void onMessage(size_t message, void* payload);
		bool keyPressed(const Event& event);
		bool mouseMoved(const Event& event);

		void loadAssets();
		void createEntities();
		void updateEntities();

	private:
		TR_EDTR_STATE(m_edtr_state)

		reference<Utility::Timer> m_timer;
		reference<Core::InputManager> m_input;

		ECS::Entity Ent;

		Triton::Matrix44 m_Transformation;

		size_t m_Light1;
		size_t m_Light2;

		Triton::reference<Triton::Data::Material> mat;
		Triton::reference<Triton::Data::Material> cubeMat;
		Triton::reference<Triton::Data::Mesh> mesh;
		Triton::reference<Triton::Data::Mesh> cubeMesh;
		Triton::reference<Triton::Data::Image> image;
	};

}