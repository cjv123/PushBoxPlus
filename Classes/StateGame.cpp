#include "StateGame.h"

#ifdef WIN32
#include <windows.h>  
#define KEY_DOWN(vk_code) (GetAsyncKeyState(vk_code) & 0x8000 ? 1 : 0)  
#define KEY_UP(vk_code) (GetAsyncKeyState(vk_code) & 0x8000 ? 0 : 1)  
#endif

StateGame::StateGame()
{

}

StateGame::~StateGame()
{
	
}

CCScene* StateGame::scene()
{
	CCScene *scene = CCScene::create();
	StateGame* layer = StateGame::create();
	scene->addChild(layer);
	return scene;
}

bool StateGame::init()
{
	if (!CCLayer::init())
	{
		return false;
	}

	scheduleUpdate();

	MapData::getInstance()->initMap("map.mp");


	MapInfo* mapinfo = MapData::getInstance()->getMapLvDatas().at(0);
	mMapData = new MapInfo(*mapinfo);

	mPusher = PusherSprite::create();
	addChild(mPusher);
	mPusher->setPosition(ccp(300,300));

	vector<string>& mapdata = mapinfo->getMapData();
	for (int i=0;i<(int)mapdata.size();i++)
	{
		for (int j=0;j<(int)mapdata[i].size();j++)
		{
			if (mapdata[i][j]=='*')
			{

			}
		}
	}


	return true;
}

void StateGame::update( float delta )
{
#ifdef WIN32
	if (KEY_DOWN(VK_UP))
	{
		mPusher->playUpMoveAnim();
	}
	else if (KEY_DOWN(VK_DOWN))
	{
		mPusher->playDownMoveAnim();
	}
	else if (KEY_DOWN(VK_LEFT))
	{
		mPusher->playLeftMoveAnim();
	}
	else if (KEY_DOWN(VK_RIGHT))
	{
		mPusher->playRightMoveAnim();
	}
#endif
}



