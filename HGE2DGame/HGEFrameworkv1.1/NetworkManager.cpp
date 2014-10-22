#include "NetworkManager.h"

#define IP_ADDRESS "10.206.8.27"

NetworkManager::NetworkManager(EUserType _type)
{
	if(_type==USERTYPE_SERVER)
	{
		eUserType=USERTYPE_SERVER;
		pServe=new Server();
		pServe->init();
		pServe->createSocket((char*)IP_ADDRESS);
		pServe->bindSocket();
		pServe->clientSocket();
	}
	else
	{
		eUserType=USERTYPE_CLIENT;
		pClient=new Client();
		pClient->init((char*)IP_ADDRESS);
		pClient->createSocket();
	}

}

NetworkManager::~NetworkManager(void)
{
}

void NetworkManager::NetworkSendData(char *_data)
{
	if(eUserType==USERTYPE_SERVER)
		pServe->sendData(_data);
	else
		pClient->sendData(_data);
}

char* NetworkManager::NetworkReceiveData()
{
	char *pValue;
	if(eUserType==USERTYPE_CLIENT)
		pValue=pClient->receiveData();
	else
		pValue=pServe->receiveData();
	return pValue;
}

EUserType NetworkManager::getUserType()
{
	return eUserType;
}
