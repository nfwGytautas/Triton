#pragma once

#include "Triton/Core/TritonClass.h"
#include "Triton/Entity/GameObject.h"

namespace Triton
{
	namespace Script
	{
		// C# script object
		// This class acts like a bridge between engine and C# 'ObjectScript' class
		class SharpScript
		{
		public:
			// PImpl idiom
			class ScriptImpl;

			SharpScript(ScriptImpl* impl);
			virtual ~SharpScript();

			// Returns the name of the script
			std::string getName() const;

			// Checks if the script is still valid meaning if the 
			// relay pointing to the game object is still valid
			bool isValid() const;
			
			// Calls OnUpdate method in C#
			void onUpdate(float delta);

			// Checks if the script is attached to the specified game object
			bool attachedTo(relay_ptr<GameObject> object) const;
		private:
			ScriptImpl* m_impl;
		};

		// Class holding the layout of a specific SharpScript
		// It's used to create an instance of SharpScript
		struct SharpScriptLayout
		{
			// The class name of the script
			std::string ClassName;

			// The path to the assembly where the script is found
			std::string Assembly;
		};

		// Script engine register params
		const Triton::Core::ClassRegisterParams ScriptEngineRegisterParams
		{
			false, // IsProtected
			{}, // Receivers
			"scriptEngine", // Name
			Layers::c_scriptUpdateLayer, // Update priority
			Layers::c_nullLayer, // PreRender priority
			Layers::c_nullLayer, // Render priority
			(
				Core::ReceivedMessages::ClassRegistered |
				Core::ReceivedMessages::Update
			) // Received messages
		};

		// The main class that powers Triton scripting capabilities
		// It contains everything one needs to run C# scripts for Triton
		class ScriptEngine : public Triton::Core::TritonClass
		{
		public:
			ScriptEngine();
			virtual ~ScriptEngine();

			// Loads all scripts from the specified assembly
			std::vector<SharpScriptLayout> loadAssembly(std::string assemblyPath);

			// Attaches a script to a game object
			void attachScript(std::string className, relay_ptr<GameObject> object);

			// Detaches a script from the specified game object
			void detachScript(std::string className, relay_ptr<GameObject> object);


			// Message handler from TritonClass
			virtual void onMessage(size_t message, void* payload) override;
			
		private:
			// Update all the SharpScripts
			void updateScripts();

			// Validates current SharpScripts
			// And deletes the ones that are invalid
			void validateScripts();
		private:
			// PImpl idiom
			class Impl;
			Impl* m_impl;

			// All loaded scripts
			std::vector<reference<SharpScript>> m_sharpScripts;

			// Layouts of registered scripts
			std::vector<SharpScriptLayout> m_scriptLayouts;
		};
	}
}