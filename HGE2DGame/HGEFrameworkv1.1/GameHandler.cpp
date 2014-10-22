#include "GameHandler.h"


GameHandler* GameHandler::getInstance()
{
	if(!ms_GameHandler)
		ms_GameHandler=new GameHandler();

	return ms_GameHandler;
}

void GameHandler::disp()
{
	cout<<"Works:";
}

GameHandler::~GameHandler(void)
{
}

void GameHandler::setState(EGameState _gs)
{
	switch(_gs)
	{
	case GAMESTATE_INTRO:
		//mp_IGS=new Intro();
		mp_IGS=mp_Intro;
		break;

	case GAMESTATE_NEWGAME:
		//mp_IGS=new NewGame();
		mp_IGS=mp_NewGame;
		break;

	case GAMESTATE_CONNECTION:
		mp_IGS=mp_Connection;
		break;

	case GAMESTATE_RUNNINGGAME:
		mp_IGS=mp_RunningGame;
		break;

	case GAMESTATE_PAUSE:
		//mp_IGS=new PauseGame();
		mp_IGS=mp_PauseGame;
		break;

	case GAMESTATE_EXIT:
		//mp_IGS=new ExitGame();
		mp_IGS=mp_ExitGame;
		break;

	default:break;
	}
	mp_IGS->init();
	ECurrentState=_gs;
}

EGameState GameHandler::getCurrentState()
{
	return ECurrentState;
}

void GameHandler::init()
{
	mp_Intro=new Intro();
	mp_NewGame=new NewGame();
	mp_Connection=new Connection();
	mp_RunningGame=new RunningGame();
	mp_PauseGame=new PauseGame();
	mp_ExitGame=new ExitGame();

	setState(GAMESTATE_INTRO);
}

void GameHandler::update()
{
	mp_IGS->update();
}

void GameHandler::draw()
{
	mp_IGS->draw();
}
