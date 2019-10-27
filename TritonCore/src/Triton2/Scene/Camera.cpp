#include "TRpch.h"
#include "Camera.h"

#include "Triton2/File/Serialize.h"
#include "Triton2/File/FormatDefinitions.h"

namespace Triton
{
	const Matrix44& Camera::viewMatrix() const
	{
		return m_viewMatrix;
	}

	Camera::Camera(std::string name)
		: m_name(name)
	{
	}

	std::string Camera::getName() const
	{
		return m_name;
	}

	Camera::Camera()
	{
	}

	StaticCamera::StaticCamera()
		: Camera()
	{
	}

	StaticCamera::StaticCamera(std::string name, Vector3 position, Vector3 targetPosition)
		: Camera(name), m_Position(position), m_Target(targetPosition)
	{
		m_viewMatrix = Core::lookAt(position, targetPosition);
	}

	Vector3 StaticCamera::getPosition() const
	{
		return m_Position;
	}

	Vector3 StaticCamera::getViewDirection() const
	{
		Vector3 dir = Vector3(
			m_Target.x - m_Position.x,
			m_Target.y - m_Position.y,
			m_Target.z - m_Position.z
			);

		return dir;
	}

	void StaticCamera::serialize(BinaryOutputArchive& archive)
	{
		// Version
		std::string version(Serialization::c_Version_Latest);
		archive(version);

		// Name
		archive(m_name);

		// Position and target of the camera
		archive(m_Position, m_Target);
	}

	void StaticCamera::deserialize(BinaryInputArchive& archive)
	{
		// Version
		std::string version;
		archive(version);

		// Check version
		if (version.compare(Serialization::c_Version_00_00_00) >= 0)
		{
			// Name
			archive(m_name);

			// Position and target of the camera
			archive(m_Position, m_Target);
		}

		m_viewMatrix = Core::lookAt(m_Position, m_Target);
	}

	std::string StaticCamera::type()
	{
		return Serialization::c_type_Static_Camera;
	}
}