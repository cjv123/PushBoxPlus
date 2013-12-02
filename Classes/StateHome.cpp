#include "StateHome.h"
#include "MapData.h"
#include "MapSearcher.h"
#include "StateSelectLv.h"
#include "StateGame.h"
#include "GameData.h"
#include "cocos-ext.h"
using namespace cocos2d::extension;


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
	GameData::getInstance();

	CCTextureCache::sharedTextureCache()->addImage("wall.png");
	CCTextureCache::sharedTextureCache()->addImage("box.png");
	CCTextureCache::sharedTextureCache()->addImage("floor.png");
	CCTextureCache::sharedTextureCache()->addImage("vx_chara01_b.png");

	UILayer* ul =UILayer::create();
	ul->addWidget(GUIReader::shareReader()->widgetFromJsonFile("SceneHomeUIEdit_1.json"));
	addChild(ul);
	

	return true;
}

void StateHome::onEnter()
{
	CCDirector::sharedDirector()->replaceScene(StateSelectLv::scene());
}

