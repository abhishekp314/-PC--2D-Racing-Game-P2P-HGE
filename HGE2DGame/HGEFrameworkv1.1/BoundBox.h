#pragma once
#ifndef BOUNDBOX
#define BOUNDBOX

#include "Common.h"
#include<hgerect.h>

class BoundBox
{
	hgeRect *mRect;
	EGameAsset mGameAssetType;
public:
	BoundBox(void);
	~BoundBox(void);
	void setRect(hgeRect *_rect);
	void setGameAssetType(EGameAsset _type);
	hgeRect* getRect();
	EGameAsset getGameAssetType();
};

#endif