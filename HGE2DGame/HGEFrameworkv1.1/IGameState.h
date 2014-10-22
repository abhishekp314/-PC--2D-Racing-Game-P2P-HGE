#pragma once
#ifndef GS
#define GS

#include "NetworkManager.h"
#include<iostream>
#include "Player.h"
#include<hgefont.h>
#include "LevelHandler.h"

using namespace std;


//This Interface IGameState has various different states which are overriden by inheriting states
//Defines the state functionality within the game

class IGameState
{
	
public:
	IGameState(void);
	~IGameState(void);

	virtual void update();
	virtual void draw();
	virtual void init();
};

class Intro:public IGameState
{
	hgeSprite *pSprite;
	HTEXTURE mTex;
	hgeFont *pFont;
public:
	Intro();
	void update();
	void draw();
	void init();

};

class NewGame:public IGameState
{
	hgeSprite *pSprite;
	HTEXTURE mTex;
	hgeFont *pFont;

public:
	NewGame();
	void update();
	void draw();
	void init();
};


class Connection:public IGameState
{
	hgeFont *pFont;
	HTEXTURE mTex;
	hgeSprite *pSprite;
public:
	Connection();
	void update();
	void draw();
	void init();
};


class RunningGame:public IGameState
{
	Player *pPlayer1;
	Player *pPlayer2;
	LevelHandler *pLevelHandler;
	char m_FinishLine[1024];
	hgeFont *pFont;
	float mTime;
	char m_Start[1024];
	hgeSprite *pSprite;
	HTEXTURE mTex;
public:
	RunningGame();
	void update();
	void draw();
	void init();
};


class PauseGame:public IGameState
{
	hgeSprite *pSprite;
	HTEXTURE mTex;
	hgeFont *pFont;
public:
	PauseGame();
	void update();
	void draw();
	void init();
};

class ExitGame:public IGameState
{
public:
	ExitGame();
	void update();
	void draw();
	void init();
};


#endif