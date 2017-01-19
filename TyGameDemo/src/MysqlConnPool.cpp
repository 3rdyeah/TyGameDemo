
#include <map>
#include <string>
#include "Config.h"
#include "MysqlConnPool.h"

#include <List>

using std::map;
using std::string;
using std::list;

namespace tyGame {

#define CONST_CAST(value) const_cast<void*>(CConfig::getInstance()->getEngineValue(value))

	CMysqlConnPool::CMysqlConnPool()
	{
		m_nBusyCnt = 0;
		logger = new CLogger("CMysqlConnPool");
	}

	CMysqlConnPool::CMysqlConnPool(char* szDbUser, char* szDbPassword, char* szIp, int nPort, char* szDbName)
	{
		m_szDbUser = szDbUser;
		m_szDbPassword = szDbPassword;
		m_szDbIp = szIp;
		m_nDbPort = nPort;
		m_szDbName = szDbName;

		m_nBusyCnt = 0;
		logger = new CLogger("CMysqlConnPool");
	}

	CMysqlConnPool::~CMysqlConnPool()
	{
		if (logger != nullptr)
		{
			delete logger;
			logger = nullptr;
		}

		getPool()->clear();
		getPool()->shrink_to_fit();
	}

	bool CMysqlConnPool::init()
	{
		setUsedCountMax(atoi((char*)CONST_CAST("UsedCountMax")));
		setPoolSizeMin(atoi((char*)CONST_CAST("ConnPoolMin")));
		setPoolSizeMax(atoi((char*)CONST_CAST("ConnPoolMax")));
		if (getPoolSizeMin() < 0 || getPoolSizeMax() <= 0)
		{
			logger->print(LOG_TYPE_ERROR, "connection pool config init failed!");
			return false;
		}
		getPool()->reserve(getPoolSizeMax());

		for (size_t i = 0; i < getPoolSizeMin(); i++)
		{
			CDbConnection* pConnection = createConnection();
			if (pConnection == nullptr)
			{
				logger->print(LOG_TYPE_ERROR, "create a CConncetion inst failed!");
			}
			else
			{
				getPool()->push_back(pConnection);
			}
		}

		return true;
	}

	CDbConnection* CMysqlConnPool::createConnection()
	{
		CDbConnection* pConnection = nullptr;
		MYSQL Conn;
		MYSQL* pMysqlConn;
		if ((pMysqlConn = mysql_init(&Conn)) == nullptr)
		{
			logger->print(LOG_TYPE_ERROR, "new mysql conn init failed!");
			return nullptr;
		}
		if (mysql_real_connect(pMysqlConn, m_szDbIp, m_szDbUser, m_szDbPassword, m_szDbName, m_nDbPort, NULL, 0))
		{
			if (!mysql_select_db(pMysqlConn, m_szDbName))
			{
				logger->print(LOG_TYPE_INFO, "new mysql conn created and connected to database!");
				pConnection = new CDbConnection(*pMysqlConn, false);
			}
			else
			{
				mysql_close(pMysqlConn);
			}
		}

		return pConnection;
	}

	CDbConnection* CMysqlConnPool::getConnect()
	{
		vector<CDbConnection*>::iterator iter = getPool()->begin();
		while (iter != getPool()->end())
		{
			if (!(*iter)->m_isBusy)
			{
				(*iter)->m_isBusy = true;
				(*iter)->m_nUsedCnt++;
				m_nBusyCnt++;
				return *iter;
			}
			iter++;
		}

		if (getPool()->size() < getPoolSizeMax())
		{
			CDbConnection* pConnection = createConnection();
			if (pConnection != nullptr)
			{
				pConnection->m_isBusy = true;
				pConnection->m_nUsedCnt++;
				m_nBusyCnt++;
				return pConnection;
			}
		}

		return nullptr;
	}

	void CMysqlConnPool::releaseConnect(CDbConnection* pConn)
	{
		vector<CDbConnection*>::iterator iter = getPool()->begin();
		while (iter != getPool()->end())
		{
			if (*iter == pConn)
			{
				if ((*iter)->m_nUsedCnt >= getUsedCountMax())
				{
					mysql_close(&(*iter)->m_Conn);
					mysql_library_end();

					m_nBusyCnt--;
					delete *iter;
					getPool()->erase(iter);
					break;
				}
				else
				{
					m_nBusyCnt--;
					(*iter)->m_isBusy = false;
					break;
				}
			}
			iter++;
		}
	}
}
