#include "LevelHandler.h"
#include<stdio.h>

extern HGE *hge;

LevelHandler::LevelHandler()
{
	//Level Attributes and data
	m_pOffsetPosition.x=0;
	m_pOffsetPosition.y=0;

	//Load font
	pFont=new hgeFont("Fonts/font1.fnt");
	
	//Load the level data
	init();
}

void LevelHandler::init()
{
	FILE *pLevelFile;
	pLevelFile=fopen("Level1.txt","r");


	char filename[256];
	float x=0,y=0,scalex=0,scaley=0,rot=0,xTop=0,yTop=0,width=0,height=0,zIndex=0;
	int type=0;;
	while(fscanf(pLevelFile,"%d %s %f %f %f %f %f %f %f %f %f %f",&type,filename,&x,&y,&scalex,&scaley,&rot,&xTop,&yTop,&width,&height,&zIndex)!=EOF)
	{
		m_pGameAssetVector.push_back(new GameAsset(type,filename,x,y,scalex,scaley,rot,xTop,yTop,width,height,zIndex));
	}
	int i=m_pGameAssetVector.size()-1;
	mFinishPosition=m_pGameAssetVector.at(i)->getInitialPosition().y;

	fclose(pLevelFile);
}

void LevelHandler::update()
{
	for(int i=0;i<m_pGameAssetVector.size();i++)
	{
		m_pGameAssetVector.at(i)->update();
	}
}

void LevelHandler::draw()
{
	for(int i=0;i<m_pGameAssetVector.size();i++)
	{
		m_pGameAssetVector.at(i)->draw();
	}
	char c[256];
	sprintf(c,"Y: %f",m_pOffsetPosition.y);
	pFont->printf(0,0,HGETEXT_LEFT,c);
}

//Making use of offset position calculated from the players physics movement
void LevelHandler::setOffsetPosition(hgeVector _p)
{
		this->m_pOffsetPosition.y=-_p.y+1400;
		
		for(int i=0;i<m_pGameAssetVector.size();i++)
			m_pGameAssetVector.at(i)->setOffsetPosition(m_pOffsetPosition);
}

float LevelHandler::getFinishPosition()
{
	return mFinishPosition;
}

vector<BoundBox*> LevelHandler::getAllBoundBoxes()
{
	vector<BoundBox*> boundBox;
	for(int i=0;i<m_pGameAssetVector.size();i++)
	{
		hgeRect *rect=m_pGameAssetVector.at(i)->getBoundBox();
		boundBox.push_back(new BoundBox());
		boundBox.at(i)->setRect(rect);
		boundBox.at(i)->setGameAssetType(m_pGameAssetVector.at(i)->getGameAssetType());
	}
	return boundBox;
}