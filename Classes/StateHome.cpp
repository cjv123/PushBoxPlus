#include "StateHome.h"
#include "MapData.h"
#include "MapSearcher.h"
#include "StateSelectLv.h"
#include "StateGame.h"


StateHome::StateHome()
{

}

StateHome::~StateHome()
{

}

CCScene* StateHome::scene()
{
	CCScene *scene = CCScene::create();
	StateHome* layer = StateHome::create();
	scene->addChild(layer);
	return scene;
}

bool StateHome::init()
{
	if (!CCLayer::init())
		return false;

	MapData::getInstance()->initMap("map.mp");
	MapSearcher::getInstance()->initSearcher("mapanwser.txt");

	CCTexture2D* textureWall = CCTextureCache::sharedTextureCache()->addImage("wall.png");
	CCTexture2D* textureBox = CCTextureCache::sharedTextureCache()->addImage("box.png");
	CCTexture2D* textureFloor = CCTextureCache::sharedTextureCache()->addImage("floor.png");

	return true;
}

void StateHome::onEnter()
{
	CCDirector::sharedDirector()->replaceScene(StateSelectLv::scene());
}
