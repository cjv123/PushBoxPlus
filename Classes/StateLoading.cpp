#include "StateLoading.h"
#include "MapData.h"
#include "MapSearcher.h"
#include "GameData.h"
#include "StateHome.h"


StateLoading::StateLoading()
{

}

StateLoading::~StateLoading()
{

}

CCScene* StateLoading::scene()
{
	CCScene *scene = CCScene::create();
	StateLoading* layer = StateLoading::create();
	scene->addChild(layer);
	return scene;
}

bool StateLoading::init()
{
	if (!CCLayer::init())
		return false;

	CCLayerColor* bg = CCLayerColor::create(ccc4(255,255,255,255));
	addChild(bg);

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("home.plist");
	CCSprite* zflog = CCSprite::createWithSpriteFrameName("zf.png");
	addChild(zflog);
	zflog->setPosition(ccp(getContentSize().width/2,getContentSize().height/2));
	zflog->setScale(2.0f);

	CCLabelTTF* loadLabel = CCLabelTTF::create("Loading...","nokiafc22.ttf",24);
	loadLabel->setAnchorPoint(ccp(1,0));
	addChild(loadLabel);
	loadLabel->setPosition(ccp(getContentSize().width,0));
	loadLabel->setColor(ccc3(0,0,0));

	schedule(schedule_selector(StateLoading::onLoadComplete), 1.5f);

	return true;
}

void StateLoading::onLoadComplete( float d )
{
	MapData::getInstance()->initMap("map.mp");
	MapData::getInstance()->readMapSaveData();
	MapSearcher::getInstance()->initSearcher("mapanwser.txt");
	GameData::getInstance();

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("pushboxres.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("wall.png")->getTexture()->setAliasTexParameters();
	CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("box.png")->getTexture()->setAliasTexParameters();
	CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("star.png")->getTexture()->setAliasTexParameters();
	CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("vx_chara01_b.png")->getTexture()->setAliasTexParameters();

	CCScene* homescene = StateHome::scene();
	CCDirector::sharedDirector()->replaceScene(homescene);
}


