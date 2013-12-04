#include "StateHome.h"
#include "MapData.h"
#include "MapSearcher.h"
#include "StateSelectLv.h"
#include "StateGame.h"
#include "GameData.h"


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

	CCTextureCache::sharedTextureCache()->addImage("wall.png")->setAliasTexParameters();
	CCTextureCache::sharedTextureCache()->addImage("box.png")->setAliasTexParameters();
	CCTextureCache::sharedTextureCache()->addImage("floor.png")->setAliasTexParameters();
	CCTextureCache::sharedTextureCache()->addImage("vx_chara01_b.png")->setAliasTexParameters();

	UILayer* ul =UILayer::create();
	UIWidget* uiwidget = GUIReader::shareReader()->widgetFromJsonFile("SceneHomeUIEdit_1.json");
	ul->addWidget(uiwidget);
	addChild(ul);
	UIButton* startButton = (UIButton*)uiwidget->getChildByName("Start_Button");
	startButton->addReleaseEvent(this,coco_releaseselector(StateHome::onButtonClick));
	startButton->setTouchEnable(true);
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(ul,ul->getTouchPriority(),false);
	return true;
}

void StateHome::onEnter()
{
	
}

void StateHome::onButtonClick( CCObject* pObj)
{
	CCDirector::sharedDirector()->replaceScene(StateSelectLv::scene());
}

