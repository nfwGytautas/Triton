#pragma once

#include "TritonPlatform2/mathematical.h"

namespace Triton
{
	/**
	 * Base camera object with the ability to calculate a view matrix from it's data
	 * said view matrix can then be directly applied to the shader matrices buffer
	 */
	class Camera
	{
	public:
		virtual ~Camera() {}

		/**
		 * Updates the camera depending on it's type
		 */
		virtual void onUpdate() = 0;

		/**
		 * Returns the view matrix of the camera
		 *
		 * @return View matrix
		 */
		const Matrix44& viewMatrix() const;

		/**
		 * Get the position where the camera is currently situated
		 *
		 * @return 3 component vector containing x,y,z position of the camera
		 */
		virtual Vector3 getPosition() const = 0;

		/**
		 * Get the direction the camera is looking in
		 *
		 * @return 3 component vector containing x,y,z view direction of the camera
		 */
		virtual Vector3 getViewDirection() const = 0;

	protected:
		Camera();
	protected:
		/**
		 * Variable used to store the value of cameras view matrix
		 * Classes that inherit camera need to calculate the view matrix
		 * themselves
		 */
		Matrix44 m_viewMatrix;
	};

	/**
	 * A camera that doesn't move and doesn't require any updating
	 */
	class StaticCamera : public Camera
	{
	public:
		StaticCamera(Vector3 position, Vector3 targetPosition);

		/**
		 * Updates the camera depending on it's type
		 */
		virtual void onUpdate() override {}

		// Inherited via Camera
		virtual Vector3 getPosition() const override;
		virtual Vector3 getViewDirection() const override;
	private:
		Vector3 m_Position;
		Vector3 m_Target;
	};
}