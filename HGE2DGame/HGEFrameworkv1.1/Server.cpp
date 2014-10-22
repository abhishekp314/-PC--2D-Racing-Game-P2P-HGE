#include "Server.h"


Server::Server(void)
{
	

}

void Server::init()
{
	mIResult=WSAStartup(MAKEWORD(2,2),&mWsaData);

	if(mIResult!=0)
	{
		cout<<"WSASTARTUP FAILED "<<mIResult;
	}
	pFd_sock=new fd_set();
	pTimeout=new TIMEVAL();
}

Server::~Server(void)
{
	closesocket(mMySocket);
	WSACleanup();
}

void Server::createSocket(char *ip)
{
	mMySocket=socket(AF_INET,SOCK_DGRAM,0);

	if(mMySocket==INVALID_SOCKET)
	{
		printf("Error at socket %d",WSAGetLastError());	
	}
	else
		printf("\nSocket successful");

	mMyAddress.sin_addr.s_addr=inet_addr(ip);
	mMyAddress.sin_family=AF_INET;
	mMyAddress.sin_port=htons(DEFAULT_PORT);

	set_fd();
	pTimeout->tv_sec=0;
	pTimeout->tv_usec=0;
}

void Server::set_fd()
{
	pFd_sock->fd_count=1;
	pFd_sock->fd_array[0]=mMySocket;
}

void Server::bindSocket()
{
	//Bind the socket
	mIResult=bind(mMySocket,(SOCKADDR*)&mMyAddress,sizeof(mMyAddress));

	if(mIResult==SOCKET_ERROR)
	{
		printf("bind failed %d",WSAGetLastError());
		closesocket(mMySocket);
	}
	else
		printf("Bind successful");

}

void Server::clientSocket()
{
	mOtherSocket=INVALID_SOCKET;
}

char* Server::receiveData()
{
	mSlen=sizeof(mOtherAddress);

	set_fd();

	mIResult=select(0,pFd_sock,0,0,pTimeout);

	if(mIResult>0)
		mIResult=recvfrom(mMySocket,mRecvbuf,DEFAULT_BUFLEN,0,(sockaddr*)&mOtherAddress,&mSlen);

	if(mIResult>0)
	{
		mRecvbuf[mIResult] = '\0';
	}
	if(mIResult==-1 || mIResult==0)
		mRecvbuf[0]='!';

	
	return mRecvbuf;
}

void Server::sendData(char *msg)
{
	cout<<"Sending";
	mIResult=sendto(mMySocket,msg,DEFAULT_BUFLEN,0,(sockaddr*)&mOtherAddress,mSlen);

	if(mIResult>0)
	{
		cout<<"sent bytes: "<<mIResult<<endl;
	}

}
