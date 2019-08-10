#pragma once

// Header containing the implementations of the Interop.cs methods

#include <mono/utils/mono-error.h>

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
	}
}