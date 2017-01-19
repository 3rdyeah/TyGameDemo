
#include <thread>
#include "TTyMalloc.h"
#include "Config.h"
#include "SocketMgr.h"

using std::thread;

namespace tyGame {

#define MSG_RECVD_FLAG "#rt#"

	typedef struct _MSG {
		char seed;
		char szMsg[1024];
	} tyMsg;

	CSocketMgr* CSocketMgr::m_Inst = nullptr;

	CSocketMgr::CSocketMgr()
	{
		logger = new CLogger("CSocketMgr");
	}

	CSocketMgr::~CSocketMgr()
	{
		if (logger != nullptr)
		{
			delete logger;
		}
		close();
	}

	CSocketMgr* CSocketMgr::getInstance()
	{
		if (m_Inst == NULL)
		{
			m_Inst = new CSocketMgr();
		}
		return m_Inst;
	}

	char* CSocketMgr::getIpByUrl(char* &szIp, const char* szUrl)
	{
		if (szUrl == NULL || strlen(szUrl) == 0)
		{
			logger->print(LOG_TYPE_ERROR, "socket host address not set!");
			return NULL;
		}
		szIp = inet_ntoa(*((struct in_addr *)gethostbyname(szUrl)->h_addr_list[0]));;

		return szIp;
	}

	bool CSocketMgr::initCfg()
	{
		bool bRet = false;
		

		return bRet;
	}

	void CSocketMgr::handleMessage(SOCKET sockCli)
	{
		sockaddr_in sockaddrCli;
		int nAddrLen = sizeof(m_sockaddrServ);
		getpeername(sockCli, (sockaddr*)&sockaddrCli, &nAddrLen);
		char* szCliAddr = inet_ntoa(sockaddrCli.sin_addr);
		logger->print(LOG_TYPE_DEBUG, "get a connection from : %s", szCliAddr);

		int nRecvCnt = 0;
		int nTotalLen = 0;
		char szBuffer[BUFFER_SIZE] = { 0 };
		char szData[BUFFER_SIZE] = { 0 };
		char* pBuffer = szBuffer;
		char* pData = szData;

		while (true)
		{
			int nLen = recv(sockCli, pBuffer, BUFFER_SIZE, 0);
			if (nLen > 0)
			{
				nTotalLen += nLen;
				memcpy(pData + nTotalLen - nLen, szBuffer, strlen(szBuffer));
				logger->print(LOG_TYPE_INFO, "recv data from %s : %s", szCliAddr, szBuffer);
			}
		}
	}

	int CSocketMgr::broadcastMessage(char* szMsg)
	{
		int nSendCnt = 0;
		vector<SOCKET>::iterator iter = m_vecSockClient.begin();
		while (iter != m_vecSockClient.end())
		{
			send(*iter, szMsg, strlen(szMsg), 0);
			iter++;
			nSendCnt++;
		}
		return nSendCnt;
	}

	bool CSocketMgr::start()
	{
		bool bRet = false;

		do
		{
			WSADATA wsaData;
			if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
			{
				logger->print(LOG_TYPE_ERROR, "WSAStartup failed!");
				break;
			}

			m_sockServ = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
			if (m_sockServ == SOCKET_ERROR)
			{
				logger->print(LOG_TYPE_ERROR, "create socket failed!");
				break;
			}
			u_long mode = 1;
			ioctlsocket(m_sockServ, FIONBIO, (u_long*)&mode);

			CConfig* cfgInst = CConfig::getInstance();
			map<string, string> mapConfig = cfgInst->getEngineConfig();
			map<string, string>::iterator cfgIter;
			if (cfgInst == NULL)
			{
				logger->print(LOG_TYPE_ERROR, "get config instance failed!");
				break;
			}
			
			cfgIter = mapConfig.find("HostAddr");
			if (cfgIter == mapConfig.end())
			{
				cfgIter = mapConfig.find("HostUrl");
				if (cfgIter == mapConfig.end())
				{
					logger->print(LOG_TYPE_ERROR, "get host address failed!");
					break;
				}
				m_szUrl = const_cast<char*>(cfgIter->second.c_str());
				m_szAddr = getIpByUrl(m_szAddr, m_szUrl);
			}
			else
			{
				m_szAddr = const_cast<char*>(cfgIter->second.c_str());
			}

			cfgIter = mapConfig.find("HostPort");
			if (cfgIter == mapConfig.end())
			{
				logger->print(LOG_TYPE_ERROR, "get host port failed!");
				break;
			}
			m_nPort = atoi(cfgIter->second.c_str());

			if (m_nPort == 0 || m_szAddr == NULL)
			{
				logger->print(LOG_TYPE_ERROR, "socket address or port not set!");
				break;
			}

			m_sockaddrServ.sin_family = AF_INET;
			m_sockaddrServ.sin_port = htons(m_nPort);
			m_sockaddrServ.sin_addr.S_un.S_addr = inet_addr(m_szAddr);
			
			if (SOCKET_ERROR == bind(m_sockServ, (sockaddr*)&m_sockaddrServ, sizeof(m_sockaddrServ)))
			{
				logger->print(LOG_TYPE_ERROR, "bind socket failed!");
				break;
			}

			if (listen(m_sockServ, 5) == SOCKET_ERROR)
			{
				logger->print(LOG_TYPE_ERROR, "listen failed!");
				break;
			}

			logger->print(LOG_TYPE_INFO, "server listening...");

			SOCKET sockClient;
			int nAddrLen = sizeof(m_sockaddrServ);
			while (true)
			{
				if ((sockClient = accept(m_sockServ, (sockaddr*)&m_sockaddrServ, &nAddrLen)) != INVALID_SOCKET)
				{
					m_vecSockClient.push_back(sockClient);
					m_vecSockClient.shrink_to_fit();
					thread pThread(&CSocketMgr::handleMessage, this, sockClient);
					pThread.detach();
				}
			}

			return true;
		} while (false);

		logger->print(LOG_TYPE_ERROR, "lastest errCode = %d", WSAGetLastError());
		close();
		return bRet;
	}


	void CSocketMgr::close()
	{
		if (m_sockServ)
		{
			closesocket(m_sockServ);
			m_sockServ = -1;
		}
		WSACleanup();
	}

	const SOCKET CSocketMgr::getServSocket()
	{
		return m_sockServ;
	}

	void CSocketMgr::closeServSocket()
	{
		if (m_sockServ != NULL)
		{
			closesocket(m_sockServ);
		}
	}

	void CSocketMgr::closeCliSocket(SOCKET &cliSock)
	{
		if (cliSock != NULL)
		{
			closesocket(cliSock);
		}
	}
}

