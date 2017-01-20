#include "Reactor.h"


namespace tyGame {

	CReactor* CReactor::m_Inst = nullptr;

	CReactor::CReactor()
	{
	}
	
	CReactor::~CReactor()
	{
	}

	CReactor* CReactor::getInstance()
	{
		if (m_Inst == nullptr)
		{
			m_Inst = new CReactor();
		}

		return m_Inst;
	}

}
