
#include <map>
#include <string>
#include "Config.h"
#include "DatabaseMgr.h"

using std::map;
using std::string;

namespace tyGame {

#define CONST_CAST(value) const_cast<void*>(CConfig::getInstance()->getEngineValue(value))

	CDatabaseMgr* CDatabaseMgr::m_Inst = nullptr;

	CDatabaseMgr::CDatabaseMgr()
	{
		logger = new CLogger("CDatabaseMgr");
	}

	CDatabaseMgr::~CDatabaseMgr()
	{
		if (logger != nullptr)
		{
			delete logger;
		}
		if (m_pConnPool != nullptr)
		{
			delete m_pConnPool;
		}
	}

	CDatabaseMgr* CDatabaseMgr::getInstance()
	{
		if (m_Inst == nullptr)
		{
			m_Inst = new CDatabaseMgr();
		}
		return m_Inst;
	}

	bool CDatabaseMgr::init()
	{
		do
		{
			m_szIp = (char*)CONST_CAST("DbIp");
			m_szDbName = (char*)CONST_CAST("DbName");
			m_szDbUser = (char*)CONST_CAST("DbUser");
			m_szDbPassword = (char*)CONST_CAST("DbPassword");
			m_nDbPort = atoi((char*)CONST_CAST("DbPort"));

			if (m_szIp == nullptr ||
				m_szDbName == nullptr ||
				m_szDbUser == nullptr ||
				m_szDbPassword == nullptr)
			{
				logger->print(LOG_TYPE_ERROR, "database config init failed!");
				break;
			}

			m_pConnPool = new CMysqlConnPool(m_szDbUser, m_szDbPassword, m_szIp, m_nDbPort, m_szDbName);
			m_pConnPool->init();

			return true;
		} while (false);

		return false;
	}

	CMysqlConnPool* CDatabaseMgr::getConnPool()
	{
		return m_pConnPool;
	}
}
