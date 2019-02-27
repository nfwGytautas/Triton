#include "ShellApplication.h"

#include "Triton\Logger\Log.h"
#include "Triton\Core\Math\Math.h"
#include <glm\gtc\matrix_transform.hpp>

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

		if(prtc_Camera.get() != nullptr)
		{
			prtc_Camera->OnUpdate();
			prtc_Shader->SetUniform("viewMatrix", prtc_Camera->ViewMatrix());
		}

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

		prtc_Shader->SetUniform("viewMatrix", view);

		prtc_Shader->Disable();

		prtc_RenderOrder = new Data::RenderOrder();
		prtc_BatchSystem->OnUpdate(*prtc_EntityRegistry.get(), 0.0f);
		prtc_RenderOrder->Batches = &prtc_BatchSystem->GetBatches();

#ifndef TR_DISABLE_SCRIPTING 
		TR_PYTHON_SCRIPT_GUARD(prtc_py_Update.attr("entry").call(prtc_EntityRegistry.get(), prtc_Delta));
#endif
	}

	void ShellApplication::RestartShell()
	{
		prtc_EntityRegistry = std::unique_ptr<ECS::Registry>(new ECS::Registry());
		py_ReloadModules();

#ifndef TR_DISABLE_SCRIPTING
		TR_PYTHON_SCRIPT_GUARD(this->prtc_py_PreExecution.attr("entry").call(prtc_EntityRegistry.get()));
#endif

		PreExecutionSetup();
	}

	void ShellApplication::Execute()
	{
		UpdateProjectionMatrix();

		PreExecutionSetup();

#ifndef TR_DISABLE_SCRIPTING 
		TR_PYTHON_SCRIPT_GUARD(this->prtc_py_PreExecution.attr("entry").call(prtc_EntityRegistry.get()));
#endif

		while (!prtc_Display->Closed())
		{
			RunShell();
			Run();
		}
	}
}