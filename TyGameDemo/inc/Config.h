/*
≈‰÷√Ω‚Œˆ
*/
#pragma once
#include <map>
#include <vector>
#include <string>
#include "Logger.h"

using std::string;
using std::vector;
using std::map;

namespace tyGame {

#define ENGINE_LINE_SIZE 256
#define GAME_LINE_SIZE 1024

#define ENGINE_SPLIT_DELIM "="
#define GAME_SPLIT_DELIM "\t"

#define CFG_GETVALUE CConfig::getInstance()->getValue

	typedef map<string, map<string, string>> GAME_CONFIG_MAP;

	class CConfig
	{
	private:
		CLogger* logger;
		static CConfig* ms_Inst;
		map<string, string> m_mapEngineConfig;
		
	private:
		CConfig();
	public:
		~CConfig();
		bool init();
		static CConfig* getInstance();
		const map<string, string> getEngineConfig();
		const void* getEngineValue(char* szItemName);

		bool analysisConfigFile(const char* szFilePath, GAME_CONFIG_MAP &mapConfig);
		const void* getValue(const GAME_CONFIG_MAP &mapConfig, int nIndex, char* szItemName) const;
		const void* getValue(const GAME_CONFIG_MAP &mapConfig, const char* szIndex, char* szItemName) const;
	};
}
