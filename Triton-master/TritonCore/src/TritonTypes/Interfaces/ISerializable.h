#pragma once

namespace Triton
{
	namespace Interface
	{

		class ISerializable
		{
		public:
			virtual ~ISerializable() { }

			virtual void Serialize() = 0;
			virtual void Deserialize() = 0;
		};

	}
}