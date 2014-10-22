#include "IGameState.h"
#include "GameHandler.h"
#include "main.h"

extern HGE *hge;
NetworkManager *pNetworkMgr;

IGameState::IGameState(void)
{

}


IGameState::~IGameState(void)
{

}

void IGameState::init()
{

}

void IGameState::update()
{
}

void IGameState::draw()
{
}

//**********************************Intro Class***/
Intro::Intro()
{

}

void Intro::init()
{
	mTex=hge->Texture_Load("Textures/intro.jpg",0,true);
	pSprite=new hgeSprite(mTex,0,0,1017,514);
	pFont=new hgeFont("Fonts/font1.fnt");
}

void Intro::update()
{	
	XInputGetState(0,&gXInputState);
	if(hge->Input_GetKeyState(HGEK_ESCAPE) || gXInputState.Gamepad.wButtons&XINPUT_GAMEPAD_START)
	GameHandler::getInstance()->setState(GAMESTATE_NEWGAME);
}

void Intro::draw()
{
	pSprite->RenderStretch(0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
		pFont->SetColor(0xFF000000);
	pFont->printf((SCREEN_WIDTH/2)-200,SCREEN_HEIGHT-40,HGETEXT_LEFT,"Ubisoft presents Multiplayer Race game");
}



//**********************************NewGame Class**/

NewGame::NewGame()
{
	
}

void NewGame::init()
{
	mTex=hge->Texture_Load("Textures/newgame.jpg",0,false);
	pSprite=new hgeSprite(mTex,20,20,995,534);
	pFont=new hgeFont("Fonts/font1.fnt");
}

void NewGame::update()
{
	XInputGetState(0,&gXInputState);

	if(hge->Input_GetKeyState(HGEK_E) || gXInputState.Gamepad.wButtons&XINPUT_GAMEPAD_BACK)
		GameHandler::getInstance()->setState(GAMESTATE_EXIT);

	if(hge->Input_GetKeyState(HGEK_1) || gXInputState.Gamepad.wButtons&XINPUT_GAMEPAD_A)
	{
		gsUser=0;
		GameHandler::getInstance()->setState(GAMESTATE_CONNECTION);
	}

	if(hge->Input_GetKeyState(HGEK_2) || gXInputState.Gamepad.wButtons&XINPUT_GAMEPAD_B)
	{
		gsUser=1;
		GameHandler::getInstance()->setState(GAMESTATE_CONNECTION);
	}
}

void NewGame::draw()
{
	pSprite->RenderStretch(0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
	pFont->printf(10,10,HGETEXT_MIDDLE,"Press 1 or A to Host Game\nPress 2 or B to Join your friend\nPress E or BACK to Exit");
}

/**********Connection State **********/
Connection::Connection()
{
	
}

void Connection::init()
{
	//Init Network Manager
	mTex=hge->Texture_Load("Textures/connecting.jpg",0,true);
	pSprite=new hgeSprite(mTex,0,0,1599,869);
	pNetworkMgr=new NetworkManager((EUserType)gsUser);
	pFont=new hgeFont("Fonts/font1.fnt");
}

void Connection::update()
{
	//If server, wait for a message from the client and if client sends a packet to the server to acknowledge
	if(gsUser==USERTYPE_SERVER)
	{
		char *msg=pNetworkMgr->NetworkReceiveData();
		
		if(strcmp(msg,"star")==0)
		{
			pNetworkMgr->NetworkSendData("star");
			GameHandler::getInstance()->setState(GAMESTATE_RUNNINGGAME);
		}
	}
	else
	{
		pNetworkMgr->NetworkSendData("star");
		GameHandler::getInstance()->setState(GAMESTATE_RUNNINGGAME);
	}
}

void Connection::draw()
{
	pSprite->RenderStretch(0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
	if(pNetworkMgr->getUserType()==USERTYPE_SERVER)
		pFont->printf((SCREEN_WIDTH/2)-200,SCREEN_HEIGHT/2,HGETEXT_LEFT,"Waiting for other Player to Join");
	else
		pFont->printf((SCREEN_WIDTH/2)-200,SCREEN_HEIGHT/2,HGETEXT_LEFT,"Connecting to Host");

}

//**********************************RunningGame Class**/
//The runtime of the game which also embeds the communication channel with the other players

RunningGame::RunningGame()
{

}

void RunningGame::init()
{
	//Set initial start screen
	mTex=hge->Texture_Load("Textures/startcount.jpg",0,true);
	pSprite=new hgeSprite(mTex,0,0,1597,899);

	//Create the players according to the EUserType, i.e Server or client
	pPlayer1=new Player("Textures/car.png",(SCREEN_WIDTH/2)-100,SCREEN_HEIGHT-100,0.4f,0.4f,0.0f,0,0,147,291,0.0f);
	pPlayer1->init();
	
	pPlayer2=new Player("Textures/car1.png",(SCREEN_WIDTH/2)+100,SCREEN_HEIGHT-100,0.4f,0.4f,0.0f,0,0,147,291,0.0f);
	pPlayer2->init();

	//Set virtual Server and client to simulate peer 2 peer communication
	if(pNetworkMgr->getUserType()==USERTYPE_SERVER)
	{
		pPlayer1->setUserType(USERTYPE_SERVER);
		pPlayer2->setUserType(USERTYPE_CLIENT);
	}
	else
	{
		pPlayer1->setUserType(USERTYPE_CLIENT);
		pPlayer2->setUserType(USERTYPE_SERVER);
	}

	//Init Levels
	pLevelHandler=new LevelHandler();//2000

	eRaceWinner=RACEWINNER_DRAW;
	pFont=new hgeFont("Fonts/font1.fnt");
	mTime=hge->Timer_GetTime();
	strcpy(m_Start,"3");
}

void RunningGame::update()
{
	float delta=(hge->Timer_GetTime()-mTime);

	if(delta>3)
	{

	//Based on virtual server/client set, update accordingly.
	if(eRaceWinner==RACEWINNER_DRAW)
	{
	if(pPlayer1->getUserType()==USERTYPE_SERVER)
	{
		pPlayer1->checkCollision(pLevelHandler->getAllBoundBoxes());

		char *sendBuf=pPlayer1->update(" "); //send input
		hgeVector p=pPlayer1->getOffsetPosition();

		if(p.y<(pLevelHandler->getFinishPosition()+SCREEN_HEIGHT))
		{
			eRaceWinner=RACEWINNER_PLAYER1;
			sendBuf="p1";
		}
		
		pNetworkMgr->NetworkSendData(sendBuf);
		pPlayer2->setOffsetPosition(p);

		char *recvBuf=pNetworkMgr->NetworkReceiveData();

		pPlayer2->update(recvBuf);//receive input

		pLevelHandler->setOffsetPosition(p);
		pLevelHandler->update();

		if(recvBuf[0]=='p' && recvBuf[1]=='1')
			eRaceWinner=RACEWINNER_PLAYER1;

		if(recvBuf[0]=='p' && recvBuf[1]=='2')
			eRaceWinner=RACEWINNER_PLAYER2;
		
	}
	else if(pPlayer1->getUserType()==USERTYPE_CLIENT)
	{
		pPlayer2->checkCollision(pLevelHandler->getAllBoundBoxes());
		//Update the other players on network
		char *sendBuf=pPlayer2->update(" "); //send input

		hgeVector p=pPlayer2->getOffsetPosition();

		if(p.y<(pLevelHandler->getFinishPosition()+SCREEN_HEIGHT))
		{
			eRaceWinner=RACEWINNER_PLAYER2;
			sendBuf="p2";
		}
		pNetworkMgr->NetworkSendData(sendBuf);
		pPlayer1->setOffsetPosition(p);

		char *recvBuf=pNetworkMgr->NetworkReceiveData();

		pPlayer1->update(recvBuf);//receive input
		pLevelHandler->setOffsetPosition(p);
		pLevelHandler->update();

		if(recvBuf[0]=='p' && recvBuf[1]=='1')
			eRaceWinner=RACEWINNER_PLAYER1;
		if(recvBuf[0]=='p' && recvBuf[1]=='2')
			eRaceWinner=RACEWINNER_PLAYER2;
	}
	}
	else
	{

		if(eRaceWinner==RACEWINNER_PLAYER1)
		{
			strcpy(m_FinishLine,"Finish: Player 1 is the winner");
			pNetworkMgr->NetworkSendData("p1");
		}
	else if(eRaceWinner==RACEWINNER_PLAYER2)
		{
			strcpy(m_FinishLine,"Finish: Player 2 is the winner");
			pNetworkMgr->NetworkSendData("p2");
		}
		//Move to further state
	}

	XInputGetState(0,&gXInputState);
	if(hge->Input_GetKeyState(HGEK_ESCAPE) || gXInputState.Gamepad.wButtons&XINPUT_GAMEPAD_BACK)
		GameHandler::getInstance()->setState(GAMESTATE_PAUSE);
}
else
	{
		if(delta>=0 && delta<=1)
			strcpy(m_Start,"3");
		else if(delta>1 && delta<=2)
			strcpy(m_Start,"2");
		else
			strcpy(m_Start,"1");
	}

}

void RunningGame::draw()
{
	float delta=(hge->Timer_GetTime()-mTime);

	if(delta>3)
	{
	if(eRaceWinner==RACEWINNER_DRAW)
	{
	pLevelHandler->draw();
	pPlayer1->draw();
	pPlayer2->draw();
	}
	else
	{
		pFont->SetColor(0xFFFFFFFF);
		pFont->printf(SCREEN_WIDTH/2-100,SCREEN_HEIGHT/2,HGETEXT_LEFT,m_FinishLine);
	}
	}
	else
	{
		pSprite->RenderStretch(0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
		pFont->SetColor(0xFF000000);
		pFont->printf(SCREEN_WIDTH/2,SCREEN_HEIGHT/2,HGETEXT_LEFT,m_Start);
	}
}



//**********************************PauseGame Class**/
//The sreen pauses the game and to exit press E

PauseGame::PauseGame()
{

}

void PauseGame::init()
{
	mTex=hge->Texture_Load("Textures/pause.jpg",0,true);
	pSprite=new hgeSprite(mTex,20,20,SCREEN_WIDTH,SCREEN_WIDTH);
	pFont=new hgeFont("Fonts/font1.fnt");
}

void PauseGame::update()
{
	XInputGetState(0,&gXInputState);
	if(hge->Input_GetKeyState(HGEK_ESCAPE)|| gXInputState.Gamepad.wButtons&XINPUT_GAMEPAD_START)
		GameHandler::getInstance()->setState(GAMESTATE_RUNNINGGAME);
	if(hge->Input_GetKeyState(HGEK_E)|| gXInputState.Gamepad.wButtons&XINPUT_GAMEPAD_BACK)
		GameHandler::getInstance()->setState(GAMESTATE_EXIT);
}

void PauseGame::draw()
{
	pSprite->Render(10,10);
	pFont->printf(SCREEN_WIDTH/2,SCREEN_HEIGHT/2,HGETEXT_LEFT,"Press ESC or START to Resume Game \nPress E or BACK to Exit");
}



//**********************************ExitGame Class**/

ExitGame::ExitGame()
{

}

void ExitGame::init()
{

}

void ExitGame::update()
{
	cout<<"ExitGame update";
}

void ExitGame::draw()
{
	cout<<"ExitGame draw";
}

