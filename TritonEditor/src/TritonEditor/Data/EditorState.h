#pragma once

#include <Triton/Core/TritonClass.h>
#include <Triton/Entity/Registry.h>
#include <Triton/Core/Scene.h>

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
		struct MaterialEditProperties
		{
			reference<Triton::PType::Texture> Texture = nullptr;
			reference<Triton::Data::ShaderProgram> Shader = nullptr;
			std::string Name = "new material";
			float Shininess = 0;
			Vector3 Ambient;
			Vector3 Diffuse;
			Vector3 Specular;
		} CurrentEditMaterial;

		reference<GameObject> CurrentGameObject;
		reference<Data::Material> CurrentMaterial;
		reference<SceneBase> CurrentScene;

		std::vector<reference<Data::Material>> AllMaterials;
		std::vector<reference<Data::Mesh>> AllMeshes;
		std::vector<reference<Data::PlainTexture>> AllTextures;
		std::vector<reference<Data::ShaderProgram>> AllShaders;

		virtual void onMessage(size_t message, void* payload);
	};

}