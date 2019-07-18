#pragma once

// FILE CONTAINING ALL THE TRITON CLASS REGISTER PARAMS OBJECTS FOR EVERY
// ENGINE OBJECT THE CONTENTS OF THE FILE SHOULD NOT BE CHANGED UNLESS YOU
// KNOW WHAT YOU ARE DOING SINCE PROTECTED CLASSES SHOULDN'T BE INTERFERED WITH

#include "TritonClass.h"

namespace Triton
{
	namespace Core
	{

		const ClassRegisterParams InputRegisterParams
		{
			false, // IsProtected
			{}, // Receivers
			"inputManager"
		};

		const Triton::Core::ClassRegisterParams ContextRegisterParams
		{
			true, // IsProtected
			{(size_t)TritonClasses::InputManager, (size_t)TritonClasses::AssetManager}, // Receivers
			"graphicsContext"
		};

		const Triton::Core::ClassRegisterParams CacheRegisterParams
		{
			true, // IsProtected
			{(size_t)TritonClasses::AssetManager}, // Receivers
			"cache"
		};

		const Triton::Core::ClassRegisterParams AssetManagerRegisterParams
		{
			false, // IsProtected
			{}, // Receivers
			"assetManager"
		};
	}
}