/*
数据库操作管理器
*/
#pragma once

#include "TyBasePool.h"
#include "MysqlConnPool.h"

namespace tyGame {

	class CDatabaseMgr
	{
	private:
		static CDatabaseMgr* m_Inst;

		char* m_szDbUser;
		char* m_szDbPassword;
		char* m_szIp;
		char* m_szDbName;
		int m_nDbPort;
				
		CMysqlConnPool* m_pConnPool;
	public:
		CLogger* logger;

	private:
		CDatabaseMgr();

	public:
		~CDatabaseMgr();

		static CDatabaseMgr* getInstance();

		bool init();
		CMysqlConnPool* getConnPool();
	};
}