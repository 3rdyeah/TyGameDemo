
#include "TTyMalloc.h"
#include "Config.h"
#include "DbConnection.h"

using std::pair;

namespace tyGame {

	CDbConnection::CDbConnection()
	{
	}

	CDbConnection::CDbConnection(MYSQL Conn, bool isBusy)
	{
		m_Conn = Conn;
		m_nUsedCnt = 0;
		m_isBusy = isBusy;
	}

	CDbConnection::~CDbConnection()
	{
		if (m_Conn.free_me)
		{
			mysql_close(&m_Conn);
			mysql_library_end();
		}
	}

	bool CDbConnection::splitQuery(vector<string> &vec, const char* szQuery)
	{
		int nStart = 0;
		int nEnd = 0;
		char* szDelim = ",";

		int nDelLen = strlen(szDelim);
		char* szTmp = const_cast<char*>(szQuery);
		string strSrc = szQuery;

		while ((szTmp = strstr(szTmp, "from")) || (szTmp = strstr(szTmp, "FROM")))
		{
			if (*(szTmp + 1) == ' ' || *(szTmp - 1) == ' ');
			{
				int diff = szTmp - szQuery;
				szTmp = const_cast<char*>(strSrc.substr(nStart, nEnd - nStart).c_str());
			}
		}

		while ((szTmp = strstr(szTmp, szDelim)))
		{
			if (strcmp(szTmp, "select") || strcmp(szTmp, "SELECT"))
			{

			}
			nEnd = szTmp - szQuery;
			vec.push_back(strSrc.substr(nStart, nEnd - nStart));

			nStart = nEnd + nDelLen;
			szTmp += nDelLen;
		}
		if (nStart != strlen(szQuery))
		{
			vec.push_back(strSrc.substr(nStart, strlen(szQuery) - nStart));
		}
		vec.shrink_to_fit();
		return false;
	}

	int CDbConnection::excuteQuery(DbResult &dbRes, char* szQuery, ...)
	{
		char* szReal = tyMalloc(&szReal, SQL_QUERY_LEN);

		int n;
		va_list argv;
		va_start(argv, szQuery);
		n = _vsnprintf(szReal, SQL_QUERY_LEN, szQuery, argv);
		va_end(argv);

		MYSQL_RES *res;
		MYSQL_ROW row;
		int nRet = mysql_real_query(&m_Conn, szReal, strlen(szReal));
		if (nRet == 0)
		{
			res = mysql_store_result(&m_Conn);
			while (row = mysql_fetch_row(res))
			{
				map<string, string> mapItem;
				for (size_t i = 0; i < mysql_num_fields(res); i++)
				{
					MYSQL_FIELD *col = mysql_fetch_field_direct(res, i);
					mapItem.insert(pair<string, string>(col->name, row[i]));
				}
				dbRes.rs.push_back(mapItem);
				dbRes.m_nEffectRows++;
			}
			dbRes.rs.shrink_to_fit();
		}
		mysql_free_result(res);

		tyFree(szReal);
		return nRet;
	}

	int CDbConnection::excute(char* szSql, ...)
	{
		char* szReal = tyMalloc(&szReal, SQL_QUERY_LEN);

		int n;
		va_list argv;
		va_start(argv, szSql);
		n = _vsnprintf(szReal, SQL_QUERY_LEN, szSql, argv);
		va_end(argv);

		int nRet = mysql_real_query(&m_Conn, szReal, strlen(szReal));

		tyFree(szReal);
		return nRet;
	}
}
