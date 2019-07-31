#include <Triton.h>

#include <string>

#include <Triton/Utility/ClassGetters.h>

#include "TritonEditor/Data/EditorState.h"
#include "TritonEditor/Scenes/EditorScene.h"
#include "TritonEditor/Scenes/TestScene.h"
#include "TritonEditor/Data/NameMap.h"

namespace Triton
{
	Triton::Core::ClassRegisterParams c_tritonEditorRegisterParams =
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
	};

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

			m_tHost->registerClass(this, c_tritonEditorRegisterParams);

			m_tHost->registerClass(new Triton::NameMap(), c_nameMapRegisterParams);

			m_tHost->registerClass(new Triton::EditorState(), c_stateRegisterParams);

			m_tHost->registerClass(new Triton::EditorScene(), c_editorRegisterParams);

			m_tHost->registerClass(new Triton::TestScene(), c_testSceneRegisterParams);

		}

		void start()
		{
			m_tHost->startST();
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