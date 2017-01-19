/*
通用方法
*/
#pragma once

#include <map>
#include <vector>
#include <string>

using std::map;
using std::vector;
using std::string;

namespace tyGame {

#define ENGINE_LINE_SIZE 256
#define GAME_LINE_SIZE 1024

	bool split(const char* szSrc, vector<string> &map, const char* szDelim);
	bool split(const char* szSrc, map<string, string> &map, const char* szDelim);
	//bool split(const char* szSrc, map<string, map<string, string>> &map, const char* szDelim);
}
