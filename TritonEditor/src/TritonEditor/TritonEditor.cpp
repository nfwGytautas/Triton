#include <Triton.h>

#include <string>
#include <filesystem>

#include "TritonEditor/Widgets/Dialog.h"

#include <Triton/Utility/ClassGetters.h>

#include "TritonEditor/Data/EditorState.h"
#include "TritonEditor/Scenes/EditorScene.h"
#include "TritonEditor/Scenes/TestScene.h"


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
		reference<EditorScene> Scene;
		reference<EditorState> State;
	public:
		TritonEditor()
		{
			m_tHost = new Triton::TritonHost();

			Triton::AppSettings appSettings;
			appSettings.WindowWidth = 1280;
			appSettings.WindowHeight = 720;

			m_tHost->init({ appSettings });

			m_tHost->registerClass(this, c_tritonEditorRegisterParams);

			State = new Triton::EditorState();
			m_tHost->registerClass(State, c_stateRegisterParams);

			State->_Project = new Triton::TritonProject("new project");
			m_tHost->proj(State->_Project);

			Scene = new Triton::EditorScene();
			m_tHost->registerClass(Scene, c_editorRegisterParams);

			m_tHost->registerClass(new Triton::TestScene(), c_testSceneRegisterParams);

			setCallbacks();
		}

		void start()
		{
			m_tHost->startST();
		}

	private:
		void setCallbacks()
		{
			Scene->Callbacks.NewProject = [&](std::string name) {
				newProject(name);
			};

			Scene->Callbacks.OpenProject = [&]() {
				openProject();
			};

			Scene->Callbacks.SaveProject = [&]() {
				saveProject();
			};
		}

		void newProject(std::string name)
		{
			auto path = std::filesystem::path(selectFolder());
			path /= name;
			std::filesystem::create_directory(path);

			std::string dirPath = path.string();

			path /= (name + ".tproj");

			State->_Project = new TritonProject(name);
			m_tHost->proj(State->_Project);
			TritonProject::saveToDisk(State->_Project, dirPath);

			std::string projPath = path.string();

			std::ofstream projFile;
			projFile.open(projPath);
			projFile << "Path=" << dirPath << "\n";
			projFile.close();

			State->Proj.Created = true;
			State->Proj.Name = name;
			State->Proj.Directory = dirPath;
		}

		void saveProject()
		{
			if (State->Proj.Directory != "")
			{
				TritonProject::saveToDisk(State->_Project, State->Proj.Directory);
			}
			else
			{
				Scene->Callbacks.ShowMessage("Can't save empty project");
			}
		}

		void openProject()
		{
			auto path = std::filesystem::path(selectFile(FileFilter::Project));

			if (path != "")
			{
				auto dir = path.remove_filename();

				State->_Project = Triton::TritonProject::loadFromDisk(dir.string());

				State->Proj.Created = true;
				State->Proj.Name = State->_Project->getName();
				State->Proj.Directory = dir.string();

				m_tHost->proj(State->_Project);
			}
		}
	private:
		TritonHost* m_tHost;
	};
}

int main(int argc, char** argv)
{
	Triton::TritonEditor editor;
	editor.start();
}