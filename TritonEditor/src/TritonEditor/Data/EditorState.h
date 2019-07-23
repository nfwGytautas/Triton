#pragma once

#include <Triton/Core/TritonClass.h>
#include <Triton/Entity/Registry.h>
#include <Triton/Core/Scene.h>

#include "TritonEditor/Data/NameMap.h"

namespace Triton
{
	const Core::ClassRegisterParams c_stateRegisterParams =
	{
		false, // IsProtected
		{}, // Receivers
		"editorState", // Name
		Layers::c_nullLayer, // Update priority
		Layers::c_nullLayer, // PreRender priority
		Layers::c_nullLayer, // Render priority
		(
			Core::ReceivedMessages::ClassRegistered
		) // Received messages
	};

	// The class that contains the state of the editor that
	// allows for communication between multiple editor classes
	// it relies on TritonClass
	class EditorState : public Core::TritonClass
	{
	public:
		ECS::Entity CurrentEntity;
		reference<SceneBase> CurrentScene;
		reference<NameMap> NameMap;

		std::vector<reference<Data::Material>> AllMaterials;
		std::vector<reference<Data::Mesh>> AllMeshes;
		std::vector<reference<Data::PlainTexture>> AllTextures;

		virtual void onMessage(size_t message, void* payload);
	};

}