#include "TRpch.h"
#include "Camera.h"

#include "Triton2/File/Serialize.h"
#include "Triton2/File/FormatDefinitions.h"
#include <TritonPlatform2\mathematical.h>

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

	void StaticCamera::onUpdate()
	{

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
	
	FreeLookCamera::FreeLookCamera()
		: Camera()
	{
	}

	FreeLookCamera::FreeLookCamera(std::string name, Vector3 position, float pitch, float yaw)
		: Camera(name), m_position(position), m_pitch(pitch), m_yaw(yaw)
	{
		calculateMatrix();
	}

	void FreeLookCamera::onUpdate()
	{
		calculateMatrix();
	}

	Vector3 FreeLookCamera::getPosition() const
	{
		return m_position;
	}

	Vector3 FreeLookCamera::getViewDirection() const
	{
		Vector3 dir = Vector3(
			m_target.x - m_position.x,
			m_target.y - m_position.y,
			m_target.z - m_position.z
		);

		return dir;
	}

	void FreeLookCamera::serialize(BinaryOutputArchive& archive)
	{
		// Version
		std::string version(Serialization::c_Version_Latest);
		archive(version);

		// Name
		archive(m_name);

		// Position and target of the camera
		archive(m_position, m_target, m_pitch, m_yaw);
	}

	void FreeLookCamera::deserialize(BinaryInputArchive& archive)
	{
		// Version
		std::string version;
		archive(version);

		// Check version
		if (version.compare(Serialization::c_Version_00_00_00) >= 0)
		{
			archive(m_name);

			archive(m_position, m_target, m_pitch, m_yaw);
		}

		m_viewMatrix = Core::lookAt(m_position, m_target);
	}

	std::string FreeLookCamera::type()
	{
		return Serialization::c_type_FreeLook_Camera;
	}

	float FreeLookCamera::getPitch() const
	{
		return m_pitch;
	}

	float FreeLookCamera::getYaw() const
	{
		return m_yaw;
	}

	Vector3 FreeLookCamera::getTarget() const
	{
		return m_target;
	}

	void FreeLookCamera::setPitch(float val)
	{
		m_pitch = val;
	}

	void FreeLookCamera::setYaw(float val)
	{
		m_yaw = val;
	}

	void FreeLookCamera::setTarget(Vector3 val)
	{
		m_target = val;
	}

	void FreeLookCamera::calculateMatrix()
	{
		if (m_pitch > 89.99999f)
			m_pitch = 89.99999f;
		if (m_pitch < -89.99999f)
			m_pitch = -89.99999f;

		Vector3 viewDirection = Vector3(
			cos(to_radians(m_pitch)) * cos(to_radians(m_yaw)),
			sin(to_radians(m_pitch)),
			cos(to_radians(m_pitch)) * sin(to_radians(m_yaw))
		);

		m_viewDirection = Core::normalize(viewDirection);
		m_target = m_position + m_viewDirection;

		m_viewMatrix = Core::lookAt(m_position, m_target);
	}
}