#include <stdio.h>
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

	void CLogger::print(E_LOG_TYPE eLogType, char* fmt, ...)
	{
		char* szLog = tyMalloc(&szLog, LOG_BUFFER_LEN);

		int nOffset = strlen(m_szClassName) + strlen(EXT);
		char* pBuffer = szLog + nOffset;
		memcpy(szLog, m_szClassName, nOffset);

		int n;
		va_list argv;
		va_start(argv, fmt);
		n = _vsnprintf(pBuffer, LOG_BUFFER_LEN - nOffset, fmt, argv);
		va_end(argv);
		
		sprintf_s(szLog, LOG_BUFFER_LEN, "[%s] %s\n", m_szClassName, pBuffer);

		printf("%s", szLog);

		tyFree(szLog);
	}
}

