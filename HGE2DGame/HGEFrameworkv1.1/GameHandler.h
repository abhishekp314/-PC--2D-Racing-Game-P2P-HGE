#pragma once
#ifndef GH
#define GH


#include<iostream>
#include "IGameState.h"
#include "Common.h"

using namespace std;


class GameHandler
{
	static GameHandler *ms_GameHandler;
	IGameState *mp_IGS;

	Intro *mp_Intro;
	NewGame *mp_NewGame;
	Connection *mp_Connection;
	RunningGame *mp_RunningGame;
	PauseGame *mp_PauseGame;
	ExitGame *mp_ExitGame;
	
	EGameState ECurrentState;
	EUserType eUserType;

	GameHandler()
	{

	}
public:
	~GameHandler(void);
	void disp();
	static GameHandler* getInstance();
	void setState(EGameState _gs);
	EGameState getCurrentState();
	void update();
	void draw();
	void init();
};

#endif
