
#include <fstream>
#include <winsock2.h>
#include <Windows.h>
#include "GlobalDef.h"
#include "TTyMalloc.h"
#include "Common.h"
#include "Config.h"

using namespace std;

#pragma comment(lib, "ws2_32")

namespace tyGame {
	CConfig* CConfig::ms_Inst = NULL;

	CConfig::CConfig()
	{
		logger = new CLogger("CConfig");
	}


	CConfig::~CConfig()
	{
		if (logger != NULL)
		{
			delete logger;
		}
		m_mapEngineConfig.clear();
	}

	bool CConfig::init()
	{
		bool bRet = false;

		do
		{
			ifstream ifCfg(CFG_PATH, ios::in);
			if (!ifCfg.is_open())
			{
				logger->print(LOG_TYPE_ERROR, "file open failed! file = %s\n", CFG_PATH);
				break;
			}

			char szLine[ENGINE_LINE_SIZE] = { 0 };
			while (ifCfg.getline(szLine, ENGINE_LINE_SIZE))
			{
				if (strlen(szLine) > 0)
				{
					split(szLine, m_mapEngineConfig, ENGINE_SPLIT_DELIM);
					memset(szLine, 0, ENGINE_LINE_SIZE);
				}
			}
			ifCfg.close();
			
			logger->print(LOG_TYPE_INFO, "engine config analysis success!");
			return true;
		} while (false);

		return bRet;
	}

	CConfig* CConfig::getInstance()
	{
		if (ms_Inst == nullptr)
		{
			ms_Inst = new CConfig();
		}
		return ms_Inst;
	}

	const map<string, string> CConfig::getEngineConfig()
	{
		return m_mapEngineConfig;
	}

	const void* CConfig::getEngineValue(char* szItemName)
	{
		map<string, string>::iterator iter = m_mapEngineConfig.find(szItemName);
		while (iter != m_mapEngineConfig.end())
		{
			return iter->second.c_str();
		}
		return nullptr;
	}

	/*解析游戏配置文件*/
	bool CConfig::analysisConfigFile(const char* szFilePath, GAME_CONFIG_MAP &mapConfig)
	{
		do
		{
			ifstream ifCfg(szFilePath, ios::in);
			if (!ifCfg.is_open())
			{
				logger->print(LOG_TYPE_ERROR, "file open failed! file = %s\n", szFilePath);
				break;
			}

			char szLine[GAME_LINE_SIZE] = { 0 };

			vector<string> vecKeys;
			ifCfg.getline(szLine, GAME_LINE_SIZE);
			if (strlen(szLine) > 0)
			{
				split(szLine, vecKeys, GAME_SPLIT_DELIM);
				memset(szLine, 0, GAME_LINE_SIZE);
			}

			while (ifCfg.getline(szLine, GAME_LINE_SIZE))
			{
				vector<string> vecValues;
				if (strlen(szLine) > 0)
				{
					split(szLine, vecValues, GAME_SPLIT_DELIM);
					memset(szLine, 0, GAME_LINE_SIZE);
				}

				int index = 0;
				vector<string>::iterator iter1 = vecKeys.begin();
				vector<string>::iterator iter2 = vecValues.begin();
				map<string, string> mapItem;

				string szIndex = *iter2;

				while (++iter1 != vecKeys.end() && ++iter2 != vecValues.end())
				{
					mapItem.insert(pair<string, string>(*iter1, *iter2));
				}

				mapConfig.insert(pair<string, map<string, string>>(szIndex, mapItem));
			}
			
			ifCfg.close();

			return true;
		} while (false);

		return false;
	}

	/*从游戏配置文件取值*/
	const void* CConfig::getValue(const GAME_CONFIG_MAP &mapConfig, int nIndex, char* szItemName) const
	{
		char szIndex[16] = {0};
		itoa(nIndex, szIndex, 16);
		return getValue(mapConfig, szIndex, szItemName);
	}

	/*从游戏配置文件取值*/
	const void* CConfig::getValue(const GAME_CONFIG_MAP &mapConfig, const char* szIndex, char* szItemName) const
	{
		if (mapConfig.size() <= 0 || 
			szIndex == NULL || strlen(szIndex) == 0 || 
			szItemName == NULL || strlen(szItemName) == 0)
		{
			return NULL;
		}

		GAME_CONFIG_MAP::const_iterator iter = mapConfig.begin();
		while (iter != mapConfig.end())
		{
			if (strcmp(iter->first.c_str(), szIndex) == 0)
			{
				map<string, string>::const_iterator subiter = iter->second.begin();
				while (subiter != iter->second.end())
				{
					if (strcmp(subiter->first.c_str(), szItemName) == 0)
					{
						return subiter->second.c_str();
					}
					subiter++;
				}
			}
			iter++;
		}
		return NULL;
	}

}
