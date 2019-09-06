#pragma once

#include "Triton/Core/TritonClass.h"
#include "Triton/Entity/GameObject.h"

namespace Triton
{
	namespace Script
	{
		// C# method invoke parameters
		// Used when creating the necessary parameters for DSS(DynamicSharpScript)
		// method calls
		// For example if the method signature is
		// (string[], string, string) 
		// then to build this method params one just needs to call 
		// (addStringParam_arr, addStringParam, addStringParam)
		class MethodParams
		{
		public:
			MethodParams();
			~MethodParams();

			// Adds a string to the params and increments the parameter size
			void addStringParam(std::string str);

			// Adds a string array to the params and increments the parameter size
			void addStringParam_arr(std::vector<std::string> strArr);

			// Convert the specified MethodParams class into a continuous vector that
			// can be passed to the invoke method
			std::vector<void*>& toArgs() const;
		private:
			// PImpl idiom
			class MethodParamsImpl;
			MethodParamsImpl* m_impl;
		};

		// Dynamic C# script object
		// This class contains a map of methods inside the C# script
		// and some helper methods for invoking them, can't be attached to game object
		class DynamicSharpScript
		{
		public:
			// PImpl idiom
			class DynamicScriptImpl;

			DynamicSharpScript(DynamicScriptImpl* impl);
			virtual ~DynamicSharpScript();

			// Invokes a method of specified name and with given arguments
			void* invokeMethod(const std::string& methodName, std::vector<void*>& args);
		private:
			DynamicScriptImpl* m_impl;
		};

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
			// The path to the assembly where the script is found
			std::string Assembly;

			// The namespace where the class is defined in
			std::string Namespace;

			// The class name of the script
			std::string ClassName;
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

			// Returns a dynamic script of the specified type
			reference<DynamicSharpScript> getDynamicScript(const std::string& className);

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