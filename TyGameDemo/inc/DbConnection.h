/*
数据库连接
*/
#pragma once

#include <winsock2.h>
#include <windows.h>
#include "stdio.h"
#include "mysql.h"

#pragma comment(lib, "libmysql.lib")

namespace tyGame {

#define SQL_QUERY_LEN 1024

	typedef vector<map<string, string>> ResultSet; /*vector<map<item_name, item_value>>*/
	typedef struct _DBRESULT {
		int m_nEffectRows;
		ResultSet rs;

		_DBRESULT() { m_nEffectRows = 0; };
		~_DBRESULT() { rs.clear(); rs.shrink_to_fit(); };
	} DbResult;

	class CDbConnection
	{
	private:
		bool splitQuery(vector<string> &vec, const char* szQuery);

	public:
		CDbConnection();
		CDbConnection(MYSQL Conn, bool isBusy);
		~CDbConnection();

		/*
			select

			dbRes		input param (query result)
			return		error code (0=success !0=error)
		*/
		int excuteQuery(DbResult &dbRes, char* szQuery, ...);

		/*
			insert 
			update 
			delete...

			return effect rows
		*/
		int excute(char* szSql, ...);

		MYSQL m_Conn;
		int m_nUsedCnt;
		bool m_isBusy;
	};
}