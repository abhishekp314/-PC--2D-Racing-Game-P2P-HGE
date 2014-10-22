#include "Client.h"


Client::Client(void)
{
	//init Winsock
	mIResult=WSAStartup(MAKEWORD(2,2),&mWsaData);
	if(mIResult!=0)
	{
		cout<<"WSASTARTUP FAILED "<<mIResult;
	}

}


Client::~Client(void)
{
	closesocket(mMySocket);
	WSACleanup();
}

void Client::init(char *ip)
{
	memset(&mAddressInfo,0,sizeof(sockaddr_in));

	mAddressInfo.sin_addr.s_addr = inet_addr(ip);
	mAddressInfo.sin_family = AF_INET;
	mAddressInfo.sin_port = htons(27015);

	pFdset=new fd_set();
	pTimeOut=new TIMEVAL();
}
void Client::createSocket()
{
	mMySocket=socket(AF_INET,SOCK_DGRAM,0);		

	if(mMySocket==INVALID_SOCKET)
	{
		cout<<"Socket error "<<WSAGetLastError();
		
	}
	else
		cout<<"success";

	set_fd();
	pTimeOut->tv_sec=0;
	pTimeOut->tv_usec=0;
}

void Client::set_fd()
{
	pFdset->fd_count=1;
	pFdset->fd_array[0]=mMySocket;
}

char* Client::receiveData()
{
	mSlen=sizeof(mAddressInfo);

	set_fd();
	mIResult=select(0,pFdset,0,0,pTimeOut);

	if(mIResult>0)
	mIResult=recvfrom(mMySocket,mRecvBuf,DEFAULT_BUFLEN,0,(sockaddr*)&mAddressInfo,&mSlen);

	if(mIResult>0)
	{
		mRecvBuf[mIResult] = '\0';
	}
	if(mIResult==-1)
		mRecvBuf[0]='!';
	
	return mRecvBuf;
}

void Client::sendData(char *msg)
{

	mIResult=sendto(mMySocket,msg,DEFAULT_BUFLEN,0,(sockaddr*)&mAddressInfo,sizeof(mAddressInfo));
	if(mIResult==SOCKET_ERROR)
	{
		printf("Send failed %d",WSAGetLastError());
	}
	printf("Bytes sent: %d\n",mIResult);
}
