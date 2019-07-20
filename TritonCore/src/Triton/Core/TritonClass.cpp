#include "TRpch.h"
#include "TritonClass.h"

#include "Triton/TritonHost.h"

namespace Triton
{
	namespace Core
	{
		reference<TritonClass> TritonClass::getClassByID(size_t id)
		{
			return m_HostInstance->getClassByID(id, this->m_ID);
		}

		reference<TritonClass> TritonClass::getClassByName(std::string name)
		{
			return m_HostInstance->getClassByName(name, this->m_ID);
		}

		void TritonClass::takeParams(ClassRegisterParams& params)
		{
			m_Protected = params.IsProtected;
			m_Receivers = params.Receivers;

#if TR_STRING_REPRESENTATIONS == 1
			m_Name = params.Name;
#endif

			if (params.MessagesToListenTo & Core::ReceivedMessages::All)
			{
				m_messages = c_BuiltInMessageMask | ReceivedMessages::Custom;
			}
			else if (params.MessagesToListenTo & Core::ReceivedMessages::None)
			{
				// Removes any other set flags if class is configures to receive no flags
				m_messages = Core::ReceivedMessages::None;
			}
			else
			{
				m_messages = params.MessagesToListenTo;
			}

			m_layerSettings.Update = params.UpdatePriority;
			m_layerSettings.PreRender = params.PreRenderPriority;
			m_layerSettings.Render = params.RenderLayer;
		}
	}
}