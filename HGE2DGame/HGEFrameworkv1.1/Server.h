#pragma once

#ifndef SERVER_
#define SERVER_

#include<iostream>
#include<WinSock2.h>
#include<WS2tcpip.h>

using namespace std;

class Server
{
static const int DEFAULT_PORT=27015;
static const int DEFAULT_BUFLEN=512;

WSADATA mWsaData;
sockaddr_in mMyAddress,mOtherAddress;

SOCKET mMySocket;
SOCKET mOtherSocket;

char mRecvbuf[DEFAULT_BUFLEN];

int mIResult;
int mSlen;
fd_set *pFd_sock;
TIMEVAL *pTimeout;

public:
	Server(void);
	~Server(void);

	void init();
	void createSocket(char *ip);
	void bindSocket();
	void clientSocket();
	void set_fd();
	void sendData(char *msg);
	char* receiveData();
};

#endif