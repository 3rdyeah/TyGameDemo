// SelectClient.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "windows.h"
#include "winsock.h"
#include <iostream>
#include <thread>
#include <mutex>

#pragma comment(lib, "ws2_32.lib")

#define MSG_RECVD_FLAG "#rt#"

using namespace std;

SOCKET sockCli;
mutex mtx;

bool initWsa()
{
	do
	{
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		{
			cout << "WSA启动失败, 错误代码: " << GetLastError() << endl;
			WSACleanup();
			break;
		}

		sockCli = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (sockCli == NULL)
		{
			cout << "创建socket失败, 错误代码: " << GetLastError() << endl;
			break;
		}
		u_long mode = 1;
		ioctlsocket(sockCli, FIONBIO, (u_long*)&mode);

		SOCKADDR_IN sockAddrInServ;
		sockAddrInServ.sin_family = AF_INET;
		sockAddrInServ.sin_port = htons(9527);
		sockAddrInServ.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

		connect(sockCli, (sockaddr*)&sockAddrInServ, sizeof(sockAddrInServ));
		cout << "连接服务器成功!" << endl;

		return true;
	} while (false);

	return false;
}

void recvmsg()
{
	char buffer[1024] = { 0 };
	while (true)
	{
		int nRet = 0;
		memset(buffer, 0, 1024);
		nRet = recv(sockCli, buffer, 1024, 0);
		if (nRet != 0 && strlen(buffer) > 0)
		{
			mtx.try_lock();
			cout << "来自服务器的消息: " << buffer << endl;
			memset(buffer, 0, 1024);
			mtx.unlock();
		}
	}
}

void sendmsg()
{
	char buffer[1024] = { 0 };
	while (true)
	{
		int nRet = 0;
		cout << "向服务器发送消息: ";
		cin >> buffer;
		if (strlen(buffer) > 0)
		{
			mtx.try_lock();
			send(sockCli, buffer, strlen(buffer) + 1, 0);
			memset(buffer, 0, 1024);
			mtx.unlock();
		}
	}
}

int main()
{
	char buffer[1024] = { 0 };
	if (!initWsa())
	{
		return 0;
	}

	thread pThreadRecv(&recvmsg);
	pThreadRecv.detach();

	while (true)
	{
		int nRet = 0;
		cout << "向服务器发送消息: ";
		cin.getline(buffer, 1024);
		mtx.try_lock();
		send(sockCli, buffer, strlen(buffer) + 1, 0);
		memset(buffer, 0, 1024);
		mtx.unlock();
	}

	closesocket(sockCli);
	WSACleanup();
	return 0;
}

