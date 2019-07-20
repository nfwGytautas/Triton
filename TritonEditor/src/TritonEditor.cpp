#include <Triton.h>

#include <Triton/Utility/ClassGetters.h>

#include "Scenes/EditorScene.h"
#include "Scenes/TestScene.h"
#include <string>

namespace Triton
{
	class TritonEditor :
		public Core::TritonClass
	{
	public:
		TritonEditor()
		{
			m_tHost = new Triton::TritonHost();

			Triton::AppSettings appSettings;
			appSettings.WindowWidth = 1280;
			appSettings.WindowHeight = 720;

			m_tHost->init({ appSettings });
			m_tHost->registerClass(
				Triton::reference<Triton::TritonEditor>(this).as<Triton::Core::TritonClass>(),
				Triton::Core::ClassRegisterParams(
					{ 
						false, 
						{}, 
						"editor",
						Layers::c_nullLayer,
						Layers::c_nullLayer,
						Layers::c_nullLayer,
						(
							Core::ReceivedMessages::None
						)
					}));



			m_tHost->registerClass(
				Triton::reference<Triton::EditorScene>(new Triton::EditorScene()).as<Triton::Core::TritonClass>(),
				Triton::Core::ClassRegisterParams(
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
					}));

			m_tHost->registerClass(
				Triton::reference<Triton::TestScene>(new Triton::TestScene()).as<Triton::Core::TritonClass>(),
				Triton::Core::ClassRegisterParams(
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
					}));
		}

		void start()
		{
			m_tHost->start();
		}

	private:
		TritonHost* m_tHost;
	};
}

int main(int argc, char** argv)
{
	try
	{
		Triton::TritonEditor editor;
		editor.start();
	}
	catch (...)
	{
		TR_ASSERT(0, "Unidentified error");
		return 1;
	}
}