/*
Socketπ‹¿Ì∆˜
*/
#pragma once

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#include <mutex>
#else //LINUX UNIX

#endif

#include "Logger.h"

#pragma comment(lib, "ws2_32.lib")

using std::mutex;

namespace tyGame {

#define BUFFER_SIZE 1024
	class CSocketMgr
	{
	private:
		SOCKET m_sockServ;
		SOCKADDR_IN m_sockaddrServ;
		char* m_szUrl;
		char* m_szAddr;
		int m_nPort;

		fd_set m_fdSet = {0};
		vector<SOCKET> m_vecSockClient;
		mutex m_mutex;
		
		static CSocketMgr* m_Inst;
	public:
		CLogger* logger;

	private:
		CSocketMgr();
	public:
		~CSocketMgr();
		static CSocketMgr* getInstance();

		bool initCfg();
		bool start();
		void close();

		char* getIpByUrl(char* &szIp, const char* szUrl);
		const SOCKET getServSocket();

		void handleMessage(SOCKET sockCli);
		int broadcastMessage(char* szMsg);

		void closeServSocket();
		void closeCliSocket(SOCKET &cliSock);
	};
}

