#pragma once
#ifndef GAMEOBJECT
#define GAMEOBJECT

#include<hgesprite.h>
#include<hgevector.h>
#include "main.h"
#include<hgefont.h>
#include<vector>
#include "Common.h"
#include "BoundBox.h"

extern XINPUT_STATE gXInputState;

using namespace std;
//This are the playable Game Objects that are under Gameplay

class Player
{
	hgeVector mPosition;
	hgeVector mOffsetPosition;
	float mScaleH,mScaleV;
	float mRot;
	float mXtop,mYtop;
	float mWidth,mHeight;
	char *pFilename;
	float mZindex;
	char mSendBuff[6];
	hgeSprite *pSprite;
	HTEXTURE mTex;

	hgeFont *pFont;
	char SpeedString[256];

	//Constants Set Once and use it

	float mMaxSpeed;
	float mMaxBSpeed;
	float mMaxLRotate;
	float mMaxRRotate;
	float mDamp;
	float mCurrentVelocity;
	float mCurrentBVelocity;
	ECarState eCarState;
	EUserType eUserType;
	ERaceWinner eRaceWinner;
	ECollisionType eCollisionType;
	
public :
	Player(char _filename[256],float _x,float _y,float _scaleH,float _scaleV,float _rot,float _xTop,float _yTop,float _width,float _height,float _zIndex);
	char* update(char *_input);
	void draw();
	void init();

	void setPosition(float _x,float _y);
	void setScale(float _scaleH,float _scaleV);
	void setRotation(float _angle);
	void setSpriteLocation(float _xTop,float _yTop,float _width,float _height);
	void loadTexture();
	void setTexture(char *_filename);

	void accelerate(float dy);
	void deaccelerate(float dy);
	void decAcceleration(float dy);
	void yaw(float dx,float rot);
	void damp(float dx);
	void Bdamp(float dx);
	void RLdamp();
	void RRdamp();
	void Integrator();
	hgeVector getOffsetPosition();
	void setOffsetPosition(hgeVector _p);
	void setUserType(EUserType eUser);
	EUserType getUserType();
	void checkCollision(vector<BoundBox*> _allRects);
};

#endif