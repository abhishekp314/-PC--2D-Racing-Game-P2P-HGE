#pragma once
#ifndef NETWORK
#define NETWORK

#include "Server.h"
#include "Client.h"
#include "Common.h"




class NetworkManager
{
	Server *pServe;
	Client *pClient;
	EUserType eUserType;

public:
	NetworkManager(EUserType _type);
	~NetworkManager(void);

	void NetworkSendData(char *_data);
	char* NetworkReceiveData();
	EUserType getUserType();
};

#endif