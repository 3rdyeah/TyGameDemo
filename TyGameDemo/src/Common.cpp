#include <iostream>
#include "Common.h"

using namespace std;

namespace tyGame {

	/*解析游戏配置表行字符串*/
	bool split(const char* szSrc, vector<string> &vec, const char* szDelim)
	{
		int nStart = 0;
		int nEnd = 0;
		int nDelLen = strlen(szDelim);
		char* szTmp = const_cast<char*>(szSrc);
		string strSrc = szSrc;
		while ((szTmp = strstr(szTmp, szDelim)))
		{
			nEnd = szTmp - szSrc;
			vec.push_back(strSrc.substr(nStart, nEnd - nStart));

			nStart = nEnd + nDelLen;
			szTmp += nDelLen;
		}
		if (nStart != strlen(szSrc))
		{
			vec.push_back(strSrc.substr(nStart, strlen(szSrc) - nStart));
		}
		vec.shrink_to_fit();

		return false;
	}

	/*解析键值关系一对一字符串*/
	bool split(const char* szSrc, map<string, string> &map, const char* szDelim)
	{
		if (!szSrc || strlen(szSrc) == 0 || !szDelim || strlen(szDelim) == 0)
		{
			return false;
		}

		string szKey, szValue;
		char szArr[ENGINE_LINE_SIZE] = {0};
		memcpy(szArr, szSrc, strlen(szSrc));

		szKey = strtok(szArr, szDelim);
		if (strlen(szKey.c_str()) > 0)
		{
			szValue = strtok(NULL, szDelim);
		}
		map.insert(pair<string, string>(szKey, szValue));

		return true;
	}

	bool CreateDir(char* szDirPath)
	{
		int nOffset = 0;
		int nRet = -1;
		char* szSubPath = szDirPath;
		char szFileDir[MAX_PATH_LEN] = { 0 };
		while (szSubPath = strstr(szSubPath, "/"))
		{
			szSubPath++;
			memset(szFileDir, 0, MAX_PATH_LEN);
			nOffset = szSubPath - szDirPath - 1;
			memcpy(szFileDir, szDirPath, nOffset);
			if (-1 == _access(szFileDir, 0))
			{
				mkdir(szFileDir);
			}
		}
		if (-1 == _access(szFileDir, 0))
		{
			return false;
		}
		return true;
	}

	bool CheckDir(char* szDirPath)
	{
		if (-1 == _access(szDirPath, 0))
		{
			return CreateDir(szDirPath);
		}

		return true;
	}
	
}