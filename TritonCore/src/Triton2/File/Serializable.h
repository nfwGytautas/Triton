#pragma once

// Forward declaration
namespace cereal
{
	class BinaryOutputArchive;
	class BinaryInputArchive;
}

namespace Triton
{
	/**
	 * [Interface]
	 * Interface that provides a common interface for saving and loading 
	 * classes from files
	 */
	class Serializable
	{
	public:
		using BinaryOutputArchive = cereal::BinaryOutputArchive;
		using BinaryInputArchive = cereal::BinaryInputArchive;
	public:
		virtual ~Serializable();

		/**
		 * Serialize the contents of the class into the specified archive
		 *
		 * @param archive The archive to serialize class into
		 */
		virtual void serialize(BinaryOutputArchive& archive) = 0;

		/**
		 * Deserialize the contents of the archive into this class instance
		 *
		 * @param archive The archive to serialize from
		 */
		virtual void deserialize(BinaryInputArchive& archive) = 0;

		/**
		 * Returns the string representation of the serializable type used
		 * in polymorphic object serialization
		 *
		 * @param archive The archive to serialize from
		 */
		virtual std::string type() = 0;
	};
}