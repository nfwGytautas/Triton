#pragma once
#include "TritonPlatform2/mathematical.h"

namespace Triton
{
	/**
	 * The simplest type of light that emits light from it's position of specified
	 * color and spans a specified range
	 */
	class PointLight
	{
	public:
		/// The position in the world where the light is situated at
		Vector3 Position = Vector3(0.0f, 0.0f, 0.0f);

		/// The color of the light that the light radiates
		Vector3 Color = Vector3(1.0f, 1.0f, 1.0f);

		/// The distance that the point light covers
		float Range = 15.0f;

		/// The constant component of attenuation lighting formula
		float Constant = 1.0f;

		/// The linear component of attenuation lighting formula
		float Linear = 0.09f;

		/// The quadratic component of attenuation lighting formula
		float Quadratic = 0.032f;
	};

	/**
	 * A flashlight type of light that emits light in a direction of specified length
	 * and angle
	 */
	class SpotLight
	{
	public:
		/// The position in the world where the light is situated at
		Vector3 Position = Vector3(0.0f, 0.0f, 0.0f);

		/// The color of the light that the light radiates
		Vector3 Color = Vector3(1.0f, 1.0f, 1.0f);

		/// The distance that the spot light reaches
		float Range = 15.0f;

		/// Angle of the spotlight
		float Angle = 20;

		/// Direction of the light
		Vector3 Direction = Vector3(0.0f, 0.0f, 0.0f);

		/// The constant component of attenuation lighting formula
		float Constant = 1.0f;

		/// The linear component of attenuation lighting formula
		float Linear = 0.09f;

		/// The quadratic component of attenuation lighting formula
		float Quadratic = 0.032f;
	};

	/**
	 * A light that is similar to that of the sun e.g infinitely far away and 
	 * emits light in a specific direction
	 */
	class DirectionalLight
	{
	public:
		/// The position in the world where the light is situated at
		Vector3 Position = Vector3(0.0f, 0.0f, 0.0f);

		/// The color of the light that the light radiates
		Vector3 Color = Vector3(1.0f, 1.0f, 1.0f);

		/// Direction of the light
		Vector3 Direction = Vector3(0.0f, 0.0f, 0.0f);
	};
}