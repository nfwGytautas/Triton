#include "ShellApplication.h"

#include "Triton\Logger\Log.h"

#include <glad\glad.h>
#include <GLFW\glfw3.h>

namespace Triton {

	ShellApplication::ShellApplication()
	{
		prtc_EntityRegistry = std::unique_ptr<ECS::Registry>(new ECS::Registry());

		prtc_BatchSystem = std::make_unique<Triton::Systems::BatchSystem>();
	}
	
	ShellApplication::~ShellApplication()
	{
	}

	void ShellApplication::RunShell()
	{	
		prtc_Shader->Enable();

		prtc_Camera->OnUpdate();
		prtc_Shader->SetUniform("viewMatrix", prtc_Camera->ViewMatrix());

		prtc_Shader->Disable();

		prtc_BatchSystem->OnUpdate(*prtc_EntityRegistry.get(), 0.0f);
		prtc_RenderBatch = &prtc_BatchSystem->GetBatches();


#ifndef TR_DISABLE_SCRIPTING 
		prtc_py_Update.attr("entry").call(prtc_EntityRegistry.get(), prtc_Delta);
#endif
	}

	void ShellApplication::RestartShell()
	{
		prtc_EntityRegistry = std::unique_ptr<ECS::Registry>(new ECS::Registry());
		py_ReloadModules();

#ifndef TR_DISABLE_SCRIPTING 
		this->prtc_py_PreExecution.attr("entry").call(prtc_EntityRegistry.get());
#endif

		PreExecutionSetup();
	}

	void ShellApplication::Execute()
	{
#ifndef TR_DISABLE_SCRIPTING 
		this->prtc_py_PreExecution.attr("entry").call(prtc_EntityRegistry.get());
#endif

		PreExecutionSetup();
		while (!prtc_Display->Closed())
		{
			RunShell();
			Run();
		}
	}
}