#pragma once

#ifndef CLIENT
#define CLIENT

#include<WinSock2.h>
#include<WS2tcpip.h>
#include<iostream>

using namespace std;

class Client
{
	static const int DEFAULT_PORT=27015;
	static const int DEFAULT_BUFLEN=512;

int mIResult;
sockaddr_in mAddressInfo;
WSADATA mWsaData;

char mRecvBuf[DEFAULT_BUFLEN];

int mSlen;

SOCKET mMySocket;

fd_set *pFdset;
TIMEVAL *pTimeOut;


public:
	Client(void);
	~Client(void);
	void init(char *ip);
	void createSocket();
	void set_fd();
	void sendData(char *);
	char* receiveData();
};

#endif