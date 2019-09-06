#pragma once

// Header containing the implementations of the Interop.cs methods

#include <mono/utils/mono-error.h>

#include "TritonPlatform/mathematical.h"

namespace Triton
{
	namespace Interop
	{
		// Send a trace(white) message to the Triton engine 
		void send_trace_Message_Internal(MonoString* payload)
		{
			MonoError* error = new MonoError();

			char* content = mono_string_to_utf8_checked(payload, error);

			auto ecode = mono_error_get_error_code(error);
			if (ecode != MONO_ERROR_NONE)
			{
				TR_SYSTEM_ERROR("Interop error {0} in 'send_trace_Message_Internal'", ecode);
				return;
			}
			else
			{
				TR_TRACE("{0}", content);
			}

			mono_free(content);
			mono_error_cleanup(error);
		}

		// Send a info(green) message to the Triton engine 
		void send_info_Message_Internal(MonoString* payload)
		{
			MonoError* error = new MonoError();

			char* content = mono_string_to_utf8_checked(payload, error);

			auto ecode = mono_error_get_error_code(error);
			if (ecode != MONO_ERROR_NONE)
			{
				TR_SYSTEM_ERROR("Interop error {0} in 'send_trace_Message_Internal'", ecode);
				return;
			}
			else
			{
				TR_INFO("{0}", content);
			}

			mono_free(content);
			mono_error_cleanup(error);
		}

		// Send a warning(yellow) message to the Triton engine 
		void send_warn_Message_Internal(MonoString* payload)
		{
			MonoError* error = new MonoError();

			char* content = mono_string_to_utf8_checked(payload, error);

			auto ecode = mono_error_get_error_code(error);
			if (ecode != MONO_ERROR_NONE)
			{
				TR_SYSTEM_ERROR("Interop error {0} in 'send_trace_Message_Internal'", ecode);
				return;
			}
			else
			{
				TR_WARN("{0}", content);
			}

			mono_free(content);
			mono_error_cleanup(error);
		}

		// Send a error(red) message to the Triton engine 
		void send_error_Message_Internal(MonoString* payload)
		{
			MonoError* error = new MonoError();

			char* content = mono_string_to_utf8_checked(payload, error);

			auto ecode = mono_error_get_error_code(error);
			if (ecode != MONO_ERROR_NONE)
			{
				TR_SYSTEM_ERROR("Interop error {0} in 'send_trace_Message_Internal'", ecode);
				return;
			}
			else
			{
				TR_ERROR("{0}", content);
			}

			mono_free(content);
			mono_error_cleanup(error);
		}

		// Returns the pointer to the game object transform component
		Components::Transform* get_transform_handle(relay_ptr<GameObject>* _handle)
		{
			return &(*_handle)->getComponent<Components::Transform>();
		}

		// Returns the handle to transforms position vector
		Vector3* get_position_handle(Components::Transform* _handle)
		{
			return &(_handle->Position);
		}

		// Returns the handle to transforms rotation vector
		Vector3* get_rotation_handle(Components::Transform* _handle)
		{
			return &(_handle->Rotation);
		}

		// Returns the handle to transforms scale vector
		Vector3* get_scale_handle(Components::Transform* _handle)
		{
			return &(_handle->Scale);
		}

		// Returns a float component of _component type from _handle object
		float get_vector3_component(Vector3* _handle, MonoObject* _component)
		{
			if (Enum::isEnum(mono_object_get_class(_component)))
			{
				void* unbox = mono_object_unbox(_component);
				Enum::VecComponent* vec = (Enum::VecComponent*)unbox;
				
				switch (*vec)
				{
				case Enum::VecComponent::X:
					return _handle->x;

				case Enum::VecComponent::Y:
					return _handle->y;

				case Enum::VecComponent::Z:
					return _handle->z;
				}
			}

			return 0.0f;
		}

		// Returns a float component of _component type from _handle object
		void set_vector3_component(Vector3* _handle, MonoObject* _component, float _value)
		{
			if (Enum::isEnum(mono_object_get_class(_component)))
			{
				void* unbox = mono_object_unbox(_component);
				Enum::VecComponent* vec = (Enum::VecComponent*)unbox;

				switch (*vec)
				{
				case Enum::VecComponent::X:
					_handle->x = _value;
					return;

				case Enum::VecComponent::Y:
					_handle->y = _value;
					return;

				case Enum::VecComponent::Z:
					_handle->z = _value;
					return;
				}
			}
		}

		// Returns a boolean that shows if the object has the transform component
		bool has_transform(relay_ptr<GameObject>* _handle)
		{
			return (*_handle)->hasComponent<Components::Transform>();
		}

		// Adds internal methods
		void addInternals()
		{
			mono_add_internal_call("TritonEngine.Interop::send_trace_Message_Internal",
				reinterpret_cast<void*>(Interop::send_trace_Message_Internal));

			mono_add_internal_call("TritonEngine.Interop::send_info_Message_Internal",
				reinterpret_cast<void*>(Interop::send_info_Message_Internal));

			mono_add_internal_call("TritonEngine.Interop::send_warn_Message_Internal",
				reinterpret_cast<void*>(Interop::send_warn_Message_Internal));

			mono_add_internal_call("TritonEngine.Interop::send_error_Message_Internal",
				reinterpret_cast<void*>(Interop::send_error_Message_Internal));

			mono_add_internal_call("TritonEngine.Types.Vector3::get_vector3_component",
				reinterpret_cast<void*>(Interop::get_vector3_component));

			mono_add_internal_call("TritonEngine.Types.Vector3::set_vector3_component",
				reinterpret_cast<void*>(Interop::set_vector3_component));

			mono_add_internal_call("TritonEngine.ECS.Transform::get_transform_handle",
				reinterpret_cast<void*>(Interop::get_transform_handle));

			mono_add_internal_call("TritonEngine.ECS.Transform::get_position_handle",
				reinterpret_cast<void*>(Interop::get_position_handle));

			mono_add_internal_call("TritonEngine.ECS.Transform::get_rotation_handle",
				reinterpret_cast<void*>(Interop::get_rotation_handle));

			mono_add_internal_call("TritonEngine.ECS.Transform::get_scale_handle",
				reinterpret_cast<void*>(Interop::get_scale_handle));

			mono_add_internal_call("TritonEngine.GameObject::has_transform",
				reinterpret_cast<void*>(Interop::has_transform));
		}
	}
}