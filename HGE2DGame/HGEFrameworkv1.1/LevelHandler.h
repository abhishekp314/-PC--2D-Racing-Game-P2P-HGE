#pragma once

#ifndef LEVEL
#define LEVEL

#include<hgefont.h>
#include<iostream>
#include<vector>
#include "BoundBox.h"

#include "GameAsset.h"

using namespace std;
//This Level Manager loads the scene behind the playable characters.

class LevelHandler
{
	hgeVector m_pOffsetPosition;
	vector<GameAsset*> m_pGameAssetVector;
	hgeFont *pFont;
	float mFinishPosition;

public:
	LevelHandler();
	void update();
	void draw();
	void init();
	void setOffsetPosition(hgeVector _p);
	vector<BoundBox*> getAllBoundBoxes();
	float getFinishPosition();
};

#endif