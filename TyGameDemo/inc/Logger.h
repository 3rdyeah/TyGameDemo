/*
»’÷æ
*/
#pragma once

namespace tyGame {

#define LOG_INFO_DIR	"./logs/info/"
#define LOG_DEBUG_DIR	"./logs/debug/"
#define LOG_ERROR_DIR	"./logs/error/"

#define LOG_BUFFER_LEN 1024
#define LOG_FILE_PATH_LEN 64
#define LOG_TIME_LEN 32

#define EXT "[] "

	typedef enum E_LOG_TYPE
	{
		LOG_TYPE_INFO = 0,
		LOG_TYPE_DEBUG,
		LOG_TYPE_ERROR
	} E_logtype;

	class CLogger
	{
	private:
		char* m_szClassName;
	public:
		CLogger();
		CLogger(char* szClassName);
		~CLogger();

	public:
		bool InitDir();
		void LogToFile(E_LOG_TYPE eLogType, char* szLog);
		void print(E_LOG_TYPE eLogType, char* fmt, ...);
	};
}

