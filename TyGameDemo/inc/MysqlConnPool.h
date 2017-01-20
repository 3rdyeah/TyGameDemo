/*
Mysql¡¨Ω”≥ÿ
*/
#pragma once

#include "DbConnection.h"
#include "TyBasePool.h"

namespace tyGame {

	class CMysqlConnPool : virtual public TTyBasePool<CDbConnection>
	{
	private:
		char* m_szDbUser;
		char* m_szDbPassword;
		char* m_szDbIp;
		char* m_szDbName;
		int	m_nDbPort;
		int m_nBusyCnt;

		CDbConnection* createConnection();
	public:
		CMysqlConnPool();
		CMysqlConnPool(char* szDbUser, char* szDbPassword, char* szIp, int nPort, char* szDbName);
		~CMysqlConnPool();
		bool init();

		CDbConnection* getConnect();
		void releaseConnect(CDbConnection* pConn);
	};
}