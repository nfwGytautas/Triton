#pragma once

#include <cereal/archives/binary.hpp>
#include <cereal\types\vector.hpp>
#include <cereal\types\memory.hpp>
#include <cereal\types\string.hpp>

namespace glm
{
	template<class Archive> void serialize(Archive& archive, glm::vec2& v) { archive(v.x, v.y); }
	template<class Archive> void serialize(Archive& archive, glm::vec3& v) { archive(v.x, v.y, v.z); }
	template<class Archive> void serialize(Archive& archive, glm::vec4& v) { archive(v.x, v.y, v.z, v.w); }
}

namespace Triton
{
	namespace Components
	{
		struct Transform;
		struct Visual;
		struct LightEmitter;

		template<class Archive>
		void serialize(Archive & archive, Transform& c)
		{
			archive(
				c.Position.x, c.Position.y, c.Position.z,
				c.Rotation.x, c.Rotation.y, c.Rotation.z,
				c.Scale.x, c.Scale.y, c.Scale.z);
		}

		template<class Archive>
		void serialize(Archive & archive, Visual& c)
		{
			archive(
				c.Mesh, c.Material);
		}

		template<class Archive>
		void serialize(Archive & archive, LightEmitter& c)
		{
			archive(
				c.Light);
		}
	}

	template<class Archive>
	void serialize(Archive& archive, Vector2& c)
	{
		archive(
			c.x, c.y);
	}

	template<class Archive>
	void serialize(Archive& archive, Vector3& c)
	{
		archive(
			c.x, c.y, c.z);
	}

	template<class Archive>
	void serialize(Archive& archive, Vector4& c)
	{
		archive(
			c.x, c.y, c.z, c.w);
	}

	namespace Data
	{
		struct MaterialData;
	
		template <class Archive>
		void serialize(Archive& ar, MaterialData& c)
		{
			ar(c.Shader);
			ar(c.Texture);
			ar(c.Ambient);
			ar(c.Diffuse);
			ar(c.Specular);
			ar(c.Shininess);
		}
	}

	namespace Core
	{
		struct ShaderData;

		template <class Archive>
		void serialize(Archive& archive, ShaderData& c)
		{
			archive(c.VertexFilepath);
			archive(c.FragmentFilepath);
		}
	}
}