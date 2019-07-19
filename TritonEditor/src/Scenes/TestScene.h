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


namespace Triton
{

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
		reference<Utility::Timer> m_timer;
		reference<Core::InputManager> m_input;

		size_t Ent;

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