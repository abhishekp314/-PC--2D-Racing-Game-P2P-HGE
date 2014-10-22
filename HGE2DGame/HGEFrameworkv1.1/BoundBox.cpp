#include "BoundBox.h"


BoundBox::BoundBox(void)
{
	mRect=new hgeRect();
}


BoundBox::~BoundBox(void)
{
}

void BoundBox::setGameAssetType(EGameAsset _type)
{
	this->mGameAssetType=_type;
}

void BoundBox::setRect(hgeRect *_rect)
{
	this->mRect=_rect;
}

hgeRect* BoundBox::getRect()
{
	return mRect;
}

EGameAsset BoundBox::getGameAssetType()
{
	return mGameAssetType;
}
