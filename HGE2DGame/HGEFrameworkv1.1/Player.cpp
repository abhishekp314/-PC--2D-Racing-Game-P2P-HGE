#include "Player.h"
#include "Common.h"
#include<iostream>

extern HGE *hge;

Player::Player(char _filename[256],float _x,float _y,float _scaleH,float _scaleV,float _rot,float _xTop,float _yTop,float _width,float _height,float _zIndex)
{
	//Set player attributes
	this->mPosition.x=_x;			this->mPosition.y=_y;
	this->mOffsetPosition.x=_x;		this->mOffsetPosition.y=_y;
	this->mScaleH=_scaleH;			this->mScaleV=_scaleV;
	this->mRot=_rot;
	this->mWidth=_width;			this->mHeight=_height;
	this->mXtop=_xTop;				this->mYtop=_yTop;
	this->pFilename=_filename;
	this->mZindex=_zIndex;

	//Set physics data with respect to type of car, in case if there exists future diff cars with diff attributes
	mMaxSpeed=20;
	mMaxBSpeed=-10;
	mMaxLRotate=-0.6;
	mMaxRRotate=0.6;
	mDamp=0.5;
	mCurrentVelocity=0.0;
	mCurrentBVelocity=0.0;
	pFont=new hgeFont("Fonts/font1.fnt");
	eCarState=CARSTATE_STOP;
	eRaceWinner=RACEWINNER_DRAW;
	eCollisionType==COLLISIONTYPE_NONE;

}

void Player::init()
{
	setPosition(mPosition.x,mPosition.y);
	setRotation(mRot);
	setScale(mScaleH,mScaleV);
	setTexture(pFilename);
	loadTexture();
}

char* Player::update(char *input)
{
	//p is a char pointer to a mSendBuff which is the data that will be sent to the other players, it maps the Input data

	char *p=mSendBuff;
	
	//Get Xinput State
	XInputGetState(0,&gXInputState);

	if(eUserType==USERTYPE_SERVER)
	{

		if(eCarState==CARSTATE_COLLIDED)
		{
			if(eCollisionType==COLLISIONTYPE_MUD)
			{
				decAcceleration(0.2);
				p[4]='1';
				eCarState=CARSTATE_MOVINGREVERSE;
			}
	
			if(eCollisionType==COLLISIONTYPE_OIL)
			{
				decAcceleration(0.2);
				if(mPosition.x<(SCREEN_WIDTH/2))
					yaw(-5,-0.3);
				else
					yaw(5,0.3);
			
				eCarState=CARSTATE_MOVINGREVERSE;
				p[4]='2';
			}
		

			if(eCollisionType==COLLISIONTYPE_PEDESTRIAN)
			{
				deaccelerate(15);
				eCarState=CARSTATE_MOVINGREVERSE;
				p[4]='3';
			}
		
			if(eCollisionType==COLLISIONTYPE_ROADBLOCK || eCollisionType==COLLISIONTYPE_NPCCAR)
			{
				mCurrentVelocity=0;
				mCurrentBVelocity=0;
				deaccelerate(50);
				eCarState=CARSTATE_STOP;
				p[4]='4';
			}
			
				eCollisionType=COLLISIONTYPE_NONE;
		}
		else
		{
			p[4]='0';
		}

		if(hge->Input_GetKeyState(HGEK_UP) || gXInputState.Gamepad.wButtons&XINPUT_GAMEPAD_A && eCollisionType!=COLLISIONTYPE_ROADBLOCK)
		{
			accelerate(1);
			eCarState=CARSTATE_MOVING;
			p[0]='6';
		}
		else
		{
			p[0]='0';
			damp(1);
		}

		if(hge->Input_GetKeyState(HGEK_DOWN)||gXInputState.Gamepad.wButtons&XINPUT_GAMEPAD_X)
		{
			deaccelerate(1);
			eCarState=CARSTATE_MOVING;
			p[1]='7';
		}
		else
		{
			p[1]='0';
			Bdamp(1);
		}

		if(hge->Input_GetKeyState(HGEK_LEFT) ||gXInputState.Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_LEFT&& (eCarState==CARSTATE_MOVING || eCarState==CARSTATE_DAMPING))
		{
			yaw(-5,-0.3);
			p[2]='8';
		}
		else
		{
			p[2]='0';
			RLdamp();
		}

		if(hge->Input_GetKeyState(HGEK_RIGHT)||gXInputState.Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_RIGHT && (eCarState==CARSTATE_MOVING || eCarState==CARSTATE_DAMPING))
		{
			yaw(5,0.3);
			p[3]='9';
		}
		else
		{
			RRdamp();
			p[3]='0';
		}
		/*char c[256];
		sprintf(c,"send : %s \n",mSendBuff);
		OutputDebugString(c);*/

		sprintf(SpeedString,"Speed : %d ",(int)mCurrentVelocity);
		//OutputDebugString(SpeedString);
		Integrator();
		
		p[5]='\0';
	}
		else if(eUserType==USERTYPE_CLIENT)
	{
			if(input[4]=='1')
			{
				//MUD COLLISION
				decAcceleration(0.2);
				eCarState=CARSTATE_MOVINGREVERSE;
			}

			if(input[4]=='2')
			{
				//OIL COLLISION
				decAcceleration(0.2);
				if(mPosition.x<(SCREEN_WIDTH/2))
					yaw(-5,-0.3);
				else
					yaw(5,0.3);

				eCarState=CARSTATE_MOVINGREVERSE;
			}

			if(input[4]=='3')
			{
				//PEDESTRIAN COLLISION
				deaccelerate(15);
				eCarState=CARSTATE_MOVINGREVERSE;
			}

			if(input[4]=='4')
			{
				//NPC CAR COLLISION
				mCurrentVelocity=0;
				mCurrentBVelocity=0;
				deaccelerate(50);
				eCarState=CARSTATE_STOP;
			}
		
		if(input[0]=='6' || hge->Input_GetKeyState(HGEK_W))
		{
			accelerate(1);
			eCarState=CARSTATE_MOVING;
		}
		else
		{
			damp(1);
		}

		if(input[1]=='7')
		{
			deaccelerate(1);
			eCarState=CARSTATE_MOVING;
		}
		else
		{
			Bdamp(1);
		}

		if(input[2]=='8'&&(eCarState==CARSTATE_MOVING || eCarState==CARSTATE_DAMPING))
		{
			yaw(-5,-0.3);
		}
		else
		{
			RLdamp();
		}

		if(input[3]=='9'&& (eCarState==CARSTATE_MOVING|| eCarState==CARSTATE_DAMPING))
		{
			yaw(5,0.3);
		}
		else
		{
			RRdamp();
		} 

		Integrator();

	}
	return p;
}

void Player::draw()
{
	if(eUserType==USERTYPE_SERVER)
	{
	pSprite->RenderEx(mPosition.x,mPosition.y,mRot,mScaleH,mScaleV);
	pFont->printf(0,SCREEN_HEIGHT-100,HGETEXT_LEFT,SpeedString);
	/*char c[256];
	sprintf(c,"%f,%f\n%f,%f\n------->rot:%f",mPosition.x,mPosition.y,mOffsetPosition.x,mOffsetPosition.y,mRot);
	pFont->printf(mPosition.x,mPosition.y,HGETEXT_LEFT,c);*/
	}
	else if(eUserType==USERTYPE_CLIENT)
	{
		pSprite->RenderEx(mPosition.x,mPosition.y+mOffsetPosition.y,mRot,mScaleH,mScaleV);
	}
}

void Player::setPosition(float _x,float _y)
{
	this->mPosition.x=_x;
	this->mPosition.y=_y;
}

void Player::setScale(float _scaleH,float _scaleV)
{
	this->mScaleH=_scaleH;
	this->mScaleV=_scaleV;
}

void Player::setRotation(float angle)
{
	this->mRot=angle;
}

void Player::setSpriteLocation(float _xTop,float _yTop,float _width,float _height)
{
	this->mXtop=_xTop;
	this->mYtop=_yTop;
	this->mWidth=_width;
	this->mHeight=_height;
}

void Player::setTexture(char *_filename)
{
	mTex=hge->Texture_Load(_filename,0,true);
}

void Player::loadTexture()
{
	pSprite=new hgeSprite(mTex,mXtop,mYtop,mWidth,mHeight);
	pSprite->SetZ(mZindex);
}

void Player::accelerate(float dy)
{
	if(mCurrentVelocity<mMaxSpeed)
		mCurrentVelocity+=(dy*0.08);
}

void Player::decAcceleration(float dy)
{
	if(mCurrentVelocity>0)
		mCurrentVelocity-=(dy);
	if(mCurrentVelocity<0)
		mCurrentVelocity=0;
}

void Player::deaccelerate(float dy)
{
	if(mCurrentBVelocity>mMaxBSpeed)
		mCurrentBVelocity-=(dy*0.1);
}

void Player::Integrator()
{
	if(eUserType==USERTYPE_SERVER)
		mOffsetPosition.y-=(mCurrentVelocity+mCurrentBVelocity);
	else if(eUserType==USERTYPE_CLIENT)
		mPosition.y-=(mCurrentVelocity+mCurrentBVelocity);
}

void Player::damp(float dx)
{
	if(mCurrentVelocity>0.0f)
	{
		mCurrentVelocity-=mDamp;
	}
	if(mCurrentVelocity<0.0f)
	{
		mCurrentVelocity=0.0f;
		eCarState=CARSTATE_STOP;
		mRot=0.0f;
	}
}

void Player::RLdamp()
{
	if(mRot<0)
		mRot+=0.1;

}

void Player::RRdamp()
{
	if(mRot>0)
		mRot=mRot-0.1;
}

void Player::Bdamp(float dx)
{
	if(mCurrentBVelocity<0.0f)
	{
		mCurrentBVelocity+=mDamp;
	}
	if(mCurrentBVelocity>0.0f)
	{
		mCurrentBVelocity=0.0f;
		eCarState=CARSTATE_STOP;
	}
}

void Player::yaw(float dx,float rot)
{
	mPosition.x+=dx;
	if(mPosition.x>(SCREEN_WIDTH-mWidth))
		mPosition.x=SCREEN_WIDTH-mWidth;
	if(mPosition.x<0)
		mPosition.x=0;
	if(mRot<=mMaxLRotate)
		mRot=mMaxLRotate;

	if(mRot>=mMaxRRotate)
		mRot=mMaxRRotate;

	mRot=mRot+(rot);
}

hgeVector Player::getOffsetPosition()
{
	if(eUserType==USERTYPE_SERVER)
		return mOffsetPosition;
}

void Player::setOffsetPosition(hgeVector _p)
{
	if(eUserType==USERTYPE_CLIENT)
	{
		mOffsetPosition.y=-_p.y+686; //Initially worked 668
	}
}

void Player::setUserType(EUserType _eUser)
{
	eUserType=_eUser;
}

EUserType Player::getUserType()
{
	return eUserType;
}

void Player::checkCollision(vector<BoundBox* > _allRects)
{
	hgeRect *playerRect=new hgeRect();
	playerRect->Set(mPosition.x,mPosition.y,mPosition.x+(mWidth*mScaleH),mPosition.y+(mHeight*mScaleV));

	for(int i=1;i<_allRects.size()-1;i++)
	{
			if(playerRect->Intersect(_allRects.at(i)->getRect()))
			{
				EGameAsset type=_allRects.at(i)->getGameAssetType();
				if(type==GAMEASSET_MUD)
				{
					eCarState=CARSTATE_COLLIDED;
					eCollisionType=COLLISIONTYPE_MUD;
				}
				else if(type==GAMEASSET_OIL)
				{
					eCollisionType=COLLISIONTYPE_OIL;
					eCarState=CARSTATE_COLLIDED;
				}
				else if(type==GAMEASSET_PEDESTRIAN_LEFT||type==GAMEASSET_PEDESTRIAN_RIGHT)
				{
					eCollisionType=COLLISIONTYPE_PEDESTRIAN;
					eCarState=CARSTATE_COLLIDED;
				}
				else if(type==GAMEASSET_ROADBLOCK)
				{
					eCollisionType=COLLISIONTYPE_ROADBLOCK;
					eCarState=CARSTATE_COLLIDED;
				}
				else if(type==GAMEASSET_FINISHLINE)
				{
					eCollisionType=COLLISIONTYPE_FINISHLINE;
					eCarState=CARSTATE_COLLIDED;
				}
				else if(type==GAMEASSET_NPCCAR)
				{
					eCollisionType=COLLISIONTYPE_NPCCAR;
					eCarState=CARSTATE_COLLIDED;
				}
			}
		
	}
/*	if(playerRect->Intersect(_allRects.at(i)))
	{
		eRaceWinner=RACEWINNER_PLAYER1;
	}*/
}



