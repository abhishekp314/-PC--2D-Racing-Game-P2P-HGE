#include "GameAsset.h"
#include "Common.h"

extern HGE *hge;

GameAsset::GameAsset(int _type,char _filename[256],float _x,float _y,float _scaleH,float _scaleV,float _rot,float _xTop,float _yTop,float _width,float _height,float _zIndex)
{
	this->mPosition.x=_x;			this->mPosition.y=_y;
	this->mInitialPosition=mPosition;

	this->mScaleH=_scaleH;	this->mScaleV=_scaleV;
	this->mRot=_rot;
	this->mWidth=_width;	this->mHeight=_height;
	this->mXtop=_xTop;		this->mYtop=_yTop;
	strcpy(pFilename,_filename);
	this->mZindex=_zIndex;
	mTex=hge->Texture_Load(pFilename,0,true);
	pSprite=new hgeSprite(mTex,mXtop,mYtop,mWidth,mHeight);
	pSprite->SetZ(mZindex);
	pFont=new hgeFont("Fonts/font1.fnt");
	this->eGameAssetType=(EGameAsset)_type;
	mNPCMovement.y=0;
}


GameAsset::~GameAsset(void)
{
}

void GameAsset::update()
{
	mPosition.y=mInitialPosition.y+mOffsetPosition.y+mNPCMovement.y;
		//Update the dynamic data which are pedestrian that walk on x-axis
		if(eGameAssetType==GAMEASSET_PEDESTRIAN_RIGHT)
		{
			if(mPosition.x<=SCREEN_WIDTH)
				mPosition.x++;
			else
				mPosition.x=mInitialPosition.x;
		}
		else if(eGameAssetType==GAMEASSET_PEDESTRIAN_LEFT)
		{
			if(mPosition.x>=0)
				mPosition.x--;
			else
				mPosition.x=mInitialPosition.x;
		}

		if(eGameAssetType==GAMEASSET_NPCCAR)
		{
			mNPCMovement.y--;

		/*if(mPosition.x>=-100)
			mPosition.x-=0.5;
		else 
			mPosition.x=SCREEN_WIDTH;*/
		}

}

void GameAsset::draw()
{
	pSprite->RenderEx(mPosition.x,mPosition.y,mRot,mScaleH,mScaleV);
}

void GameAsset::setPosition(float _x,float _y)
{
	this->mPosition.x=_x;
	this->mPosition.y=_y;
}

char * GameAsset::getFilename()
{
	return pFilename;
}

void GameAsset::setOffsetPosition(hgeVector _p)
{
	this->mOffsetPosition=_p;
}

void GameAsset::setScale(float _scaleH,float _scaleV)
{
	this->mScaleH=_scaleH;
	this->mScaleV=_scaleV;
}

void GameAsset::setRotation(float angle)
{
	this->mRot=angle;
}

void GameAsset::setSpriteLocation(float _xTop,float _yTop,float _width,float _height)
{
	this->mXtop=_xTop;
	this->mYtop=_yTop;
	this->mWidth=_width;
	this->mHeight=_height;
}

void GameAsset::setTexture(char *_filename)
{
	mTex=hge->Texture_Load(_filename,0,false);
}

void GameAsset::setGameAssetType(EGameAsset _type)
{
	this->eGameAssetType=_type;
}

hgeVector GameAsset::getInitialPosition()
{
	return mInitialPosition;
}

EGameAsset GameAsset::getGameAssetType()
{
	return eGameAssetType;
}

void GameAsset::LoadTexture()
{
	pSprite=new hgeSprite(mTex,mXtop,mYtop,mWidth,mHeight);
}

void GameAsset::render()
{
	pSprite->RenderEx(mPosition.x,mPosition.y,mRot,mScaleH,mScaleV);
}

hgeRect* GameAsset::getBoundBox()
{
	hgeRect *rect=new hgeRect();
	rect->Set(mPosition.x,mPosition.y,mPosition.x+(mWidth*mScaleH),mPosition.y+(mHeight*mScaleV));
	return rect;
}
