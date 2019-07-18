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

		void TritonClass::takeParams(ClassRegisterParams params)
		{
			m_Protected = params.IsProtected;
			m_Receivers = params.Receivers;
		}
	}
}