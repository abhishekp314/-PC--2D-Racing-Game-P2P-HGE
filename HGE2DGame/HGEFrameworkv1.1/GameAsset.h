#pragma once

#ifndef GAMEASSETS
#define GAMEASSETS
#include<hgesprite.h>
#include<hgevector.h>
#include<hgefont.h>
#include<iostream>
#include "Common.h"

class GameAsset
{
	hgeVector mPosition,mInitialPosition,mOffsetPosition,mNPCMovement;
	float mScaleH,mScaleV;
	float mRot;
	float mXtop,mYtop;
	float mWidth,mHeight;
	char pFilename[256];
	hgeSprite *pSprite;
	float mZindex;
	HTEXTURE mTex;
	hgeFont *pFont;
	EGameAsset eGameAssetType;

public:
	GameAsset(int _type,char _filename[256],float _x,float _y,float _scaleH,float _scaleV,float _rot,float _xTop,float _yTop,float _width,float _height,float _zIndex);
	~GameAsset(void);

	void update();
	void draw();

	char* getFilename();
	void setPosition(float _x,float _y);
	void setScale(float _scaleH,float _scaleV);
	void setRotation(float _angle);
	void setSpriteLocation(float _xTop,float _yTop,float _width,float _height);
	void LoadTexture();
	void setTexture(char *_filename);
	void render();
	void setOffsetPosition(hgeVector _p);
	hgeRect* getBoundBox();
	hgeVector getInitialPosition();
	EGameAsset getGameAssetType();
	void setGameAssetType(EGameAsset _type);
};

#endif