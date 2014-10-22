
//Including server first because winsock2.h needs to be included before the windows.h to avoid re-definition
#include "Server.h"
#include "main.h"
#include "GameHandler.h"
#include "Common.h"

XINPUT_STATE gXInputState;
HGE *hge=0;

GameHandler *GameHandler::ms_GameHandler=0;

bool FrameFunc()
{
	//This function contains the updated data of the current state in the game scene. It's also the update function called from HGE's Game Loop
	GameHandler *pGm=GameHandler::getInstance();

	pGm->update();

	if(pGm->getCurrentState()==GAMESTATE_EXIT)
		return true;
		
	return false;
}

bool RenderFunc()
{
	//This function contains all the draw data which is to be rendered in the scene. It's also the draw function called from HGE's Game Loop

	GameHandler *pGm=GameHandler::getInstance();
	hge->Gfx_BeginScene();
	hge->Gfx_Clear(0);
	pGm->draw();
	hge->Gfx_EndScene();
	return false;
}

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE prevInstance,LPSTR cl,int cmdShow)
{
	//Create a Game Handler that manages the states the of the lifetime of the game
	GameHandler *pGm=GameHandler::getInstance();

	//Set up XInput Controller
	ZeroMemory(&gXInputState,sizeof(XINPUT_STATE));
	XInputGetState(0,&gXInputState);

	//Set HGE Engine Attributes
	hge=hgeCreate(HGE_VERSION);

	hge->System_SetState(HGE_FRAMEFUNC,FrameFunc);
	hge->System_SetState(HGE_RENDERFUNC,RenderFunc);
	hge->System_SetState(HGE_WINDOWED,true);
	hge->System_SetState(HGE_USESOUND,false);
	hge->System_SetState(HGE_TITLE,"HGE Framework");
	hge->System_SetState(HGE_FPS,100);
	hge->System_SetState(HGE_WINDOWED,true);
	hge->System_SetState(HGE_SCREENWIDTH,SCREEN_WIDTH);
	hge->System_SetState(HGE_SCREENHEIGHT,SCREEN_HEIGHT);
	hge->System_SetState(HGE_SCREENBPP,32);
	hge->System_SetState(HGE_ZBUFFER,true);

	if(hge->System_Initiate())
	{
		//Start the HGE Engine and Game Handler
		pGm->init();
		hge->System_Start();
	}
	else
		MessageBox(NULL,hge->System_GetErrorMessage(),"Error",MB_OK|MB_ICONERROR|MB_APPLMODAL);
	
	hge->System_Shutdown();
	hge->Release();

	return 0;
}