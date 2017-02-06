#include <stdio.h>
#include <fstream>
#include <Windows.h>
#include "Common.h"
#include "TTyMalloc.h"
#include "Logger.h"

using namespace std;

#define va_start __crt_va_start_a
#define va_arg __crt_va_arg
#define va_end __crt_va_end

namespace tyGame {
	CLogger::CLogger()
	{
		m_szClassName = "Default";
	}

	CLogger::CLogger(char* szClassName)
	{
		m_szClassName = szClassName;
	}

	CLogger::~CLogger()
	{
	}

	bool CLogger::InitDir()
	{
		do
		{
			if (!CheckDir(LOG_INFO_DIR))
			{
				print(LOG_TYPE_ERROR, "create folder failed! dir = %s", LOG_INFO_DIR);
				break;
			}

			if (!CheckDir(LOG_DEBUG_DIR))
			{
				print(LOG_TYPE_ERROR, "create folder failed! dir = %s", LOG_DEBUG_DIR);
				break;
			}

			if (!CheckDir(LOG_ERROR_DIR))
			{
				print(LOG_TYPE_ERROR, "create folder failed! dir = %s", LOG_ERROR_DIR);
				break;
			}

			return true;
		} while (false);
		
		return false;
	}

	void CLogger::LogToFile(E_LOG_TYPE eLogType, char* szLog)
	{
		char* szDir = nullptr;
		int nLen = 0;

		SYSTEMTIME systime;
		char szSysTime[LOG_TIME_LEN] = { 0 };
		GetLocalTime(&systime);
		sprintf(szSysTime, "%d-%d-%d", systime.wYear, systime.wMonth, systime.wDay);

		switch (eLogType)
		{
		case LOG_TYPE_INFO:
			nLen = strlen(LOG_INFO_DIR);
			szDir = LOG_INFO_DIR;
			break;
		case LOG_TYPE_DEBUG:
			nLen = strlen(LOG_DEBUG_DIR);
			szDir = LOG_DEBUG_DIR;
			break;
		case LOG_TYPE_ERROR:
			nLen = strlen(LOG_ERROR_DIR);
			szDir = LOG_ERROR_DIR;
			break;
		default:
			break;
		}

		nLen += LOG_FILE_PATH_LEN;
		char* szFile = tyMalloc(&szFile, nLen);
		memset(szFile, 0, nLen);
		sprintf_s(szFile, nLen, "%s%s.txt", szDir, szSysTime);

		ofstream ofLog(szFile, ios::app);
		ofLog.write(szLog, strlen(szLog));
		ofLog.close();
				
		tyFree(szFile);
		szFile = nullptr;
	}

	void CLogger::print(E_LOG_TYPE eLogType, char* fmt, ...)
	{
		char* szLog = tyMalloc(&szLog, LOG_BUFFER_LEN);

		SYSTEMTIME systime;
		char szSysTime[LOG_TIME_LEN] = { 0 };
		GetLocalTime(&systime);
		sprintf(szSysTime, "%d:%d:%d.%03d", systime.wHour, systime.wMinute, systime.wSecond, systime.wMilliseconds);

		int nOffset = strlen(m_szClassName) + strlen(EXT) + LOG_TIME_LEN + 1;
		char* pBuffer = szLog + nOffset;
		memcpy(szLog, m_szClassName, nOffset);

		int n;
		va_list argv;
		va_start(argv, fmt);
		n = _vsnprintf(pBuffer, LOG_BUFFER_LEN - nOffset, fmt, argv);
		va_end(argv);

		sprintf_s(szLog, LOG_BUFFER_LEN, "[%s %s] %s\n", m_szClassName, szSysTime, pBuffer);
		LogToFile(eLogType, szLog);

		printf("%s", szLog);

		tyFree(szLog);
		szLog = nullptr;
	}
}

